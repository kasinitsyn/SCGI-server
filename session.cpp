#include <memory>
#include <vector>
#include <string> // for replace
#include <algorithm> // for replace
#include <boost/algorithm/string/replace.hpp> // for replace


#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio.hpp>
#include <boost/asio/io_context.hpp>
#include <boost/asio/spawn.hpp>
#include <boost/system/error_code.hpp>

#include "session.hpp"
#include "parser.hpp"


/*
struct EchoServer::Request_params
{
    std::string length;
    std::string content_lentgh;
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
*/

/*
struct EchoServer::Request_params EchoServer::request_parsing(std::vector<unsigned char> query_vec)
{
    struct EchoServer::Request_params Request;

    std::string query;

    for (char i: query_vec)
        query += i;

    // длина блока с параметрами
    Request.length = query.substr(0, query.find(":"));

    // ----- GET ----- //
    Request.content_lentgh = query.substr(query.find("CONTENT_LENGTH") + 15,
                                              query.find("REQUEST_METHOD") - query.find("CONTENT_LENGTH") - 15);
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
        Request.content = query.substr(query.find(",", query.find("HTTP_CACHE_CONTROL")), std::stoi(Request.content_lentgh));
    }
    else
    {
        Request.http_sec_fetch_user = query.substr(query.find("HTTP_SEC_FETCH_USER") + 19,
                                                 query.find(",", query.find("HTTP_SEC_FETCH_USER")) - query.find("HTTP_SEC_FETCH_USER") - 19);
        Request.http_cache_control = "none";
        Request.content = query.substr(query.find(",", query.find("HTTP_SEC_FETCH_USER")), std::stoi(Request.content_lentgh));
    }

    return Request;
}
*/

/*
void EchoServer::print_request_params(struct EchoServer::Request_params Request)
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
*/


EchoServer::session::session(ba::ip::tcp::socket socket, ba::io_context & _context, size_t buf_size)
    : socket_(std::move(socket)),
      context(_context),
      timer_(socket_.get_executor()),
      buffer_in(buf_size)//,
      //strand_(socket_.get_executor())
{
    //std::cout << "!in session: buf size = " << buf_size << "\n";
}

void EchoServer::session::go()
{
    std::cout << "Connect...\n";
    auto self(shared_from_this());
    ba::spawn(self->context,
        [this, self](ba::yield_context yield)
        {
          try
          {
            char data[1024];
            for (;;)
            {
              timer_.expires_from_now(std::chrono::seconds(1));
              std::size_t n = socket_.async_read_some(ba::buffer(buffer_in), yield);
              std::cout << data;
              std::cout << "Get a request, size: " << n << '\n';
              /*
              for (char i: buffer_in)
                  std::cout << i;
              std::cout << '\n';
              */
              //std::string data1 = "Status: 200 OK\r\nContent−Type: text/plain\r\n\r\n\r\nALLO!!!";
              std::string data1 = "Status: 200 OK\r\nContent−Type: image/bmp\r\nContent-Length: 236\r\n\r\nR0lGODlhEAAOALMAAOazToeHh0tLS/7LZv/0jvb29t/f3//Ub//ge8WSLf/rhf/3kdbW1mxsbP//mf///yH5BAAAAAAALAAAAAAQAA4AAARe8L1Ekyky67QZ1hLnjM5UUde0ECwLJoExKcppV0aCcGCmTIHEIUEqjgaORCMxIC6e0CcguWw6aFjsVMkkIr7g77ZKPJjPZqIyd7sJAgVGoEGv2xsBxqNgYPj/gAwXEQA7";

              //char data_cringe[1204] = "707:CONTENT_LENGTH0REQUEST_METHODGETREQUEST_URI/test.htmlQUERY_STRINGCONTENT_TYPEDOCUMENT_URI/test.htmlDOCUMENT_ROOT/usr/share/nginx/htmlSCGI1SERVER_PROTOCOLHTTP/1.1REQUEST_SCHEMEhttpREMOTE_ADDR127.0.0.1REMOTE_PORT54114SERVER_PORT80SERVER_NAMEHTTP_HOST127.0.0.1HTTP_USER_AGENTMozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:93.0) Gecko/20100101 Firefox/93.0HTTP_ACCEPTtext/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,*/*;q=0.8HTTP_ACCEPT_LANGUAGEen-US,en;q=0.5HTTP_ACCEPT_ENCODINGgzip, deflateHTTP_CONNECTIONkeep-aliveHTTP_UPGRADE_INSECURE_REQUESTS1HTTP_SEC_FETCH_DESTdocumentHTTP_SEC_FETCH_MODEnavigateHTTP_SEC_FETCH_SITEnoneHTTP_SEC_FETCH_USER?1,";
              //char data_cringe = "Content−Type: application/octet−stream"; //wiki example

              struct Response_params RParams;
              RParams.status = "200 OK";
              RParams.content_type = "text/plain";
              RParams.content_length = 23;
              RParams.content = "Hello from SCGI server!";
              std::string response = create_response(RParams);

              std::size_t n1 = response.length();
              std::size_t res = ba::async_write(socket_, ba::buffer(response, n1), yield); //data1, n1
              std::cout << "Sent a responce, size: " << res << '\n';

              struct Request_params Params = request_parsing(buffer_in);
              print_request_params(Params);

              std::cout << "Connection closed\n";

              socket_.close();
              timer_.cancel();
            }
          }
          catch (std::exception& e)
          {
            socket_.close();
            timer_.cancel();
          }
        });

    ba::spawn(self->context,
        [this, self](ba::yield_context yield)
        {
          while (socket_.is_open())
          {
            boost::system::error_code ignored_ec;
            timer_.async_wait(yield[ignored_ec]);
            if (timer_.expires_from_now() <= std::chrono::seconds(0))
              socket_.close();
          }
        });
}

