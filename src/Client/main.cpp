
#include <iostream>
#include "Client.hpp"

int main() {
    try{
        auto client = Client();
        client.start();
        client.run("http://localhost:7777");
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        std::cout << "Press any for exit." << std::endl;
        getchar();
        return 1;
    }

    return 0;
}
