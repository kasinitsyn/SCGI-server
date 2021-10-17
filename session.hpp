#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <vector>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>


namespace Server
{
    namespace ba = boost::asio;

    class session : public std::enable_shared_from_this<session>
    {
    public:
        session(ba::ip::tcp::socket _socket, ba::io_context & _context, size_t buf_size);

        void go();

    private:
        ba::ip::tcp::socket socket;

        std::vector<unsigned char> buffer_in;

        ba::io_context & context;
    };

}

#endif
