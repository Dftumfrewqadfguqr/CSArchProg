#ifndef SERVER_HTTPSERVER_HPP
#define SERVER_HTTPSERVER_HPP

#include <HttpServer.h>
#include <HttpService.h>

#include "Routers.hpp"

class HttpServer final {
private:
    std::unique_ptr<hv::HttpServer> _server;
    HttpService _router;

public:
    using UPtr = std::unique_ptr<HttpServer>;

    HttpServer();
    HttpServer(const HttpServer &) = delete;
    HttpServer(HttpServer &&) = delete;
    ~HttpServer();

    void Start(int port);
};

#endif //SERVER_HTTPSERVER_HPP
