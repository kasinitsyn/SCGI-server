#ifndef ASYNC_SERVER_H
#define ASYNC_SERVER_H

#include <boost/asio/io_context.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/system/error_code.hpp>

#include <boost/asio.hpp>

#include "session.hpp"

namespace EchoServer
{
    class echo_server
    {
    public:
        echo_server(ba::io_context & _context, unsigned short port)
            : context(_context),
              acceptor(context, ba::ip::tcp::endpoint(ba::ip::tcp::v4(), port))
        {}

        void accept(ba::ip::tcp::socket & socket, size_t buf_size)
        {
            ba::spawn(context, [this, &socket, buf_size](ba::yield_context yield)
                {
                  for (;;)
                  {
                    std::cout << "Accept...\n";

                    boost::system::error_code ec;
                    ba::ip::tcp::socket socket(context);
                    acceptor.async_accept(socket, yield[ec]);

                    if (!ec)
                    {
                        std::make_shared<session>(std::move(socket), context, buf_size)->go();
                    }
                    else
                    {
                        std::cout << "Error";
                        return;
                    }
                  }

                });
        }

    private:
        ba::io_context & context;
        ba::ip::tcp::acceptor acceptor;
    };
}


#endif
