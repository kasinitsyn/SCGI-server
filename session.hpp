#ifndef SESSION_H
#define SESSION_H

#include <memory>
#include <vector>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

//#include "parser.hpp"

namespace EchoServer
{
    namespace ba = boost::asio;

    //struct Request_params;

    //struct Request_params request_parsing(std::vector<unsigned char> query_vec);

    //void print_request_params(struct Request_params Request);

    class session : public std::enable_shared_from_this<session>
    {
    public:
        session(ba::ip::tcp::socket socket, ba::io_context & _context, size_t buf_size);

        void go();

    private:
        ba::ip::tcp::socket socket_;
        ba::steady_timer timer_;
        //boost::asio::io_service::strand strand_;

        std::vector<unsigned char> buffer_in;
        std::vector<unsigned char> buffer_out;

        ba::io_context & context;
    };



}

#endif
