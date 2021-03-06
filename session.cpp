#include <memory>
#include <vector>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/system/error_code.hpp>

#include "parser.hpp"
#include "session.hpp"


namespace Server
{
    session::session(ba::ip::tcp::socket _socket, ba::io_context & _context, size_t buf_size)
        : socket(std::move(_socket)),
          context(_context),
          buffer_in(buf_size)
    {}


    void session::go()
    {
        std::cout << "Connect...\n";
        auto self(shared_from_this());
        ba::spawn(self->context,
            [this, self](ba::yield_context yield)
            {
              boost::system::error_code ec;

              // Чтение запроса
              std::size_t n = socket.async_read_some(ba::buffer(buffer_in), yield);
              if (ec) return;
              std::cout << "Get a request, size: " << n << "\n";

              // Парсинг и вывод полученного запроса
              try
              {
                  Request_params Params = request_parsing(buffer_in);
                  print_request_params(Params);
              }
              catch (...)
              {
                  std::cerr << "Parsing error: invalid request format\n";
              }

              // Формирование ответа
              Response_params RParams;
              RParams.status = "200 OK";
              RParams.content_type = "text/plain";
              RParams.content_length = 23;
              RParams.content = "Hello from SCGI server!";
              std::string response = create_response(RParams);

              // Посылка ответа
              std::size_t n1 = response.length();
              std::size_t res= ba::async_write(socket, ba::buffer(response, n1), yield);
              if (ec) return;
              std::cout << "Sent a responce, size: " << res << '\n';

              // Закрытие соединения
              socket.close();
              std::cout << "Connection closed\n";
            });
    }
}
