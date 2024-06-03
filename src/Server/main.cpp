
#include <iostream>
#include "Api/HTTPServer.hpp"

int main() {

    try{
        HttpServer::UPtr server = std::make_unique<HttpServer>();
        server->Start(7777);
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        std::cout << "Press any for exit." << std::endl;
        getchar();
        return 1;
    }

    return 0;
}
