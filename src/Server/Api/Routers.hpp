#ifndef SERVER_ROUTERS_HPP
#define SERVER_ROUTERS_HPP

#include <HttpService.h>

namespace route {
    void RegisterResources(hv::HttpService &router);
}

#endif //SERVER_ROUTERS_HPP
