#include <boost/asio/io_context.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/asio/write.hpp>
#include <iostream>
#include <memory>

#include "session.hpp"
#include "async_server.hpp"

using namespace EchoServer;

// Код основан на примере из документации boost::asio::spawn
// - https://www.boost.org/doc/libs/1_55_0/doc/html/boost_asio/example/cpp11/spawn/echo_server.cpp
// Расхождение версии из документации и актуальной библиотеки:
// - https://www.boost.org/doc/libs/1_74_0/doc/html/boost_asio/net_ts.html

namespace ba = boost::asio;

int main(int argc, char* argv[])
{
    std::cout << "Start\n";

    unsigned short port;
    size_t bufsize;
    size_t thread_num;

    //port = 8080;
    //bufsize = 1024;
    //thread_num = 2;

    // Проверка аргументов командной строки
    if (argc != 4)
    {
      std::cerr << "Invalid arguments number\n";
      return 1;
    }

    try
    {
        port = std::stoi(argv[1]);
    }
    catch (std::invalid_argument e)
    {
        std::cerr << "Invalid port";
        return 1;
    }

    try
    {
        bufsize = std::stoi(argv[2]);
    }
    catch (std::invalid_argument e)
    {
        std::cerr << "Invalid buffer size";
        return 1;
    }

    try
    {
        thread_num = std::stoi(argv[3]);
    }
    catch (std::invalid_argument e)
    {
        std::cerr << "Invalid thread number";
        return 1;
    }


    try
    {
        ba::io_context context;

        ba::thread_pool pool(thread_num);

        echo_server server(context, port);

        ba::ip::tcp::socket socket(context);

        server.accept(socket, bufsize);

        for (int i = 0; i < thread_num; i++)
        {
            ba::post(pool, [&context]()
            {
                context.run();
            });
        }

        pool.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception: " << e.what() << "\n";
    }

    return 0;
}
