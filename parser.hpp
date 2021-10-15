#ifndef PARSER_H
#define PARSER_H

#include <memory>
#include <vector>

#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/system/error_code.hpp>
#include <iostream>

namespace EchoServer
{
    struct Request_params
    {
        size_t length;
        size_t content_lentgh;
        std::string request_method;
        std::string  request_uri;
        std::string query_string;
        std::string content_type;
        std::string document_uri;
        std::string document_root;
        std::string scgi;
        std::string server_protocol;
        std::string request_scheme;
        std::string remote_addr;
        std::string remote_port;
        std::string server_port;
        std::string server_name;

        std::string request_method2;
        std::string http_host;
        std::string http_user_agent;
        std::string http_accept;
        std::string http_accept_language;
        std::string http_accept_encoding;
        std::string http_connection;
        std::string http_upgrade_insecure_request;
        std::string http_sec_fetch_dest;
        std::string http_sec_fetch_mode;
        std::string http_sec_fetch_site;
        std::string http_sec_fetch_user;
        std::string http_cache_control;

        std::string content;
    };


    struct Request_params request_parsing(std::vector<unsigned char> query_vec);


    void print_request_params(struct Request_params Request);


    struct Response_params
    {
        std::string status;
        std::string content_type;
        size_t content_length;
        std::string content;
    };


    std::string create_response(struct Response_params Params);

}


#endif
