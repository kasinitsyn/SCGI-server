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


    struct Request_params request_parsing(std::vector<unsigned char> query_vec)
    {
        struct Request_params Request;

        std::string query;

        for (char i: query_vec)
            query += i;

        // длина блока с параметрами
        Request.length = std::stoi(query.substr(0, query.find(":")));

        // ----- GET ----- //
        Request.content_lentgh = std::stoi(query.substr(query.find("CONTENT_LENGTH") + 15,
                                                  query.find("REQUEST_METHOD") - query.find("CONTENT_LENGTH") - 15));
        Request.request_method = query.substr(query.find("REQUEST_METHOD") + 14,
                                                  query.find("REQUEST_URI") - query.find("REQUEST_METHOD") - 14);
        Request.request_uri = query.substr(query.find("REQUEST_URI") + 11,
                                                  query.find("QUERY_STRING") - query.find("REQUEST_URI") - 11);
        Request.query_string  = query.substr(query.find("QUERY_STRING") + 12,
                                                 query.find("CONTENT_TYPE") - query.find("QUERY_STRING") - 12);
        Request.content_type = query.substr(query.find("CONTENT_TYPE") + 12,
                                                 query.find("DOCUMENT_URI") - query.find("CONTENT_TYPE") - 12);
        Request.document_uri = query.substr(query.find("DOCUMENT_URI") + 12,
                                                 query.find("DOCUMENT_ROOT") - query.find("DOCUMENT_URI") - 12);
        Request.document_root = query.substr(query.find("DOCUMENT_ROOT") + 13,
                                                 query.find("SCGI") - query.find("DOCUMENT_ROOT") - 13);
        Request.scgi = query.substr(query.find("SCGI") + 4,
                                                 query.find("SERVER_PROTOCOL") - query.find("SCGI") - 4);
        Request.server_protocol = query.substr(query.find("SERVER_PROTOCOL") + 15,
                                                 query.find("REQUEST_SCHEME") - query.find("SERVER_PROTOCOL") - 15);
        Request.request_scheme = query.substr(query.find("REQUEST_SCHEME") + 14,
                                                 query.find("REMOTE_ADDR") - query.find("REQUEST_SCHEME") - 14);
        Request.remote_addr = query.substr(query.find("REMOTE_ADDR") + 11,
                                                 query.find("REMOTE_PORT") - query.find("REMOTE_ADDR") - 11);
        Request.remote_port = query.substr(query.find("REMOTE_PORT") + 11,
                                                 query.find("SERVER_PORT") - query.find("REMOTE_PORT") - 11);
        Request.server_port = query.substr(query.find("SERVER_PORT") + 11,
                                                 query.find("SERVER_NAME") - query.find("SERVER_PORT") - 11);
        Request.server_name = query.substr(query.find("SERVER_NAME") + 11,
                                                 query.find("REQUEST_METHOD", query.find("SERVER_NAME")) - query.find("SERVER_NAME") - 11);

        // ----- POST ------ //
        Request.request_method2 = query.substr(query.find("REQUEST_METHOD", query.find("SERVER_NAME")) + 14,
                                                 query.find("HTTP_HOST") - query.find("REQUEST_METHOD", query.find("SERVER_NAME")) - 14);
        Request.http_host = query.substr(query.find("HTTP_HOST") + 9,
                                                 query.find("HTTP_USER_AGENT") - query.find("HTTP_HOST") - 9);
        Request.http_user_agent = query.substr(query.find("HTTP_USER_AGENT") + 15,
                                                 query.find("HTTP_ACCEPT") - query.find("HTTP_USER_AGENT") - 15);
        Request.http_accept = query.substr(query.find("HTTP_ACCEPT") + 11,
                                                 query.find("HTTP_ACCEPT_LANGUAGE") - query.find("HTTP_ACCEPT") - 11);
        Request.http_accept_language = query.substr(query.find("HTTP_ACCEPT_LANGUAGE") + 20,
                                                 query.find("HTTP_ACCEPT_ENCODING") - query.find("HTTP_ACCEPT_LANGUAGE") - 20);
        Request.http_accept_encoding = query.substr(query.find("HTTP_ACCEPT_ENCODING") + 20,
                                                 query.find("HTTP_CONNECTION") - query.find("HTTP_ACCEPT_ENCODING") - 20);
        Request.http_connection = query.substr(query.find("HTTP_CONNECTION") + 15,
                                                 query.find("HTTP_UPGRADE_INSECURE_REQUEST") - query.find("HTTP_CONNECTION") - 15);
        Request.http_upgrade_insecure_request = query.substr(query.find("HTTP_UPGRADE_INSECURE_REQUEST") + 29,
                                                 query.find("HTTP_SEC_FETCH_DEST") - query.find("HTTP_UPGRADE_INSECURE_REQUEST") - 29);
        Request.http_sec_fetch_dest = query.substr(query.find("HTTP_SEC_FETCH_DEST") + 19,
                                                 query.find("HTTP_SEC_FETCH_MODE") - query.find("HTTP_SEC_FETCH_DEST") - 19);
        Request.http_sec_fetch_mode = query.substr(query.find("HTTP_SEC_FETCH_MODE") + 19,
                                                 query.find("HTTP_SEC_FETCH_SITE") - query.find("HTTP_SEC_FETCH_MODE") - 19);
        Request.http_sec_fetch_site = query.substr(query.find("HTTP_SEC_FETCH_SITE") + 19,
                                                 query.find("HTTP_SEC_FETCH_USER") - query.find("HTTP_SEC_FETCH_SITE") - 19);

        if (query.find("HTTP_CACHE_CONTROL") != -1)
        {
            Request.http_sec_fetch_user = query.substr(query.find("HTTP_SEC_FETCH_USER") + 19,
                                                     query.find("HTTP_CACHE_CONTROL") - query.find("HTTP_SEC_FETCH_USER") - 19);
            Request.http_cache_control = query.substr(query.find("HTTP_CACHE_CONTROL") + 18,
                                                     query.find(",", query.find("HTTP_CACHE_CONTROL")) - query.find("HTTP_CACHE_CONTROL") - 18);
            Request.content = query.substr(query.find(",", query.find("HTTP_CACHE_CONTROL")), Request.content_lentgh);
        }
        else
        {
            Request.http_sec_fetch_user = query.substr(query.find("HTTP_SEC_FETCH_USER") + 19,
                                                     query.find(",", query.find("HTTP_SEC_FETCH_USER")) - query.find("HTTP_SEC_FETCH_USER") - 19);
            Request.http_cache_control = "none";
            Request.content = query.substr(query.find(",", query.find("HTTP_SEC_FETCH_USER")), Request.content_lentgh);
        }

        return Request;
    }


    void print_request_params(struct Request_params Request)
    {
        std::cout << "Request params:\n";
        std::cout << "  len = " << Request.length << "\n";
        std::cout << "  content_lentgh = " << Request.content_lentgh << "\n"; //<< query.find("CONTENT_LENGTH") << "   " << query.find("REQUEST_METHOD") << "\n";
        std::cout << "  request_method = " << Request.request_method << "\n";
        std::cout << "  request_uri = " << Request.request_uri << "\n";
        std::cout << "  query_string = " << Request.query_string << "\n";
        std::cout << "  content_type = " << Request.content_type << "\n";
        std::cout << "  document_uri = " << Request.document_uri << "\n";
        std::cout << "  document_uri = " << Request.document_root << "\n";
        std::cout << "  scgi = " << Request.scgi << "\n";
        std::cout << "  server_protocol = " << Request.server_protocol << "\n";
        std::cout << "  request_scheme = " << Request.request_scheme << "\n";
        std::cout << "  remote_addr = " << Request.remote_addr << "\n";
        std::cout << "  remote_port = " << Request.remote_port << "\n";
        std::cout << "  server_port = " << Request.server_port << "\n";
        std::cout << "  server_name = " << Request.server_name << "\n";
        std::cout << "  request_method2 = " << Request.request_method2 << "\n";
        std::cout << "  http_host = " << Request.http_host << "\n";
        std::cout << "  http_user_agent = " << Request.http_user_agent << "\n";
        std::cout << "  http_accept = " << Request.http_accept << "\n";
        std::cout << "  http_accept_language = " << Request.http_accept_language << "\n";
        std::cout << "  http_accept_encoding = " << Request.http_accept_encoding << "\n";
        std::cout << "  http_connection = " << Request.http_connection << "\n";
        std::cout << "  http_upgrade_insecure_request = " << Request.http_upgrade_insecure_request << "\n";
        std::cout << "  http_sec_fetch_dest = " << Request.http_sec_fetch_dest << "\n";
        std::cout << "  http_sec_fetch_mode = " << Request.http_sec_fetch_mode << "\n";
        std::cout << "  http_sec_fetch_site = " << Request.http_sec_fetch_site << "\n";
        std::cout << "  http_sec_fetch_user = " << Request.http_sec_fetch_user << "\n";
        std::cout << "  http_cache_control = " << Request.http_cache_control << "\n";
        std::cout << "  content = " << Request.content << "\n";
    }


    struct Response_params
    {
        std::string status;
        std::string content_type;
        size_t content_length;
        std::string content;
    };

    std::string create_response(struct Response_params Params)
    {
        // Example:
        // "Status: 200 OK\r\nContent−Type: text/plain\r\nContent-Length: 23\r\n\r\nHello from SCGI server!";
        //

        std::string response;
        response = ("Status: " + Params.status + "\r\nContent-Type: " + Params.content_type +
                    "\r\nContent-Length: " + std::to_string(Params.content_length) + "\r\n\r\n" + Params.content);

        return response;
    }
}


#endif
