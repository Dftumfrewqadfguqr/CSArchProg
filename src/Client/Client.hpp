
#ifndef CLIENT_CLIENT_HPP
#define CLIENT_CLIENT_HPP

#include <string>
#include <array>
#include <functional>
#include <hv/requests.h>

class Client {
private:
    std::array<std::string, 14> _commandsID = {
            "exit",
            "clear",
            "help",
            "login",
            "logout",
            "register",
            "add user",
            "get self",
            "get user",
            "get users",
            "change self",
            "change user",
            "delete self",
            "delete user"


    };

    std::array<std::string, 14> _commandsOutlines = {
            "exit from the program",
            "clear console",
            "command description output",
            "entering and verifying",
            "clear verify data",
            "add new common user",
            "add new user with params",
            "getting current user data",
            "getting user data by ID",
            "getting all users data",
            "change current user",
            "change user data by ID",
            "delete current user",
            "delete user by ID"


    };

    bool _running = false;

    std::string _uuid;
    std::string _login;
    std::string _password;
    std::string _access;

    http_headers getHeaders();

    void exit(const std::string &url);
    void clear(const std::string &url);
    void help(const std::string &url);

    void login(const std::string &url);
    void logout(const std::string &url);
    void registration(const std::string &url);
    void addUser(const std::string &url);
    void getSelf(const std::string &url);
    void getUser(const std::string &url);
    void getUsers(const std::string &url);
    void changeSelf(const std::string &url);
    void changeUser(const std::string &url);
    void deleteSelf(const std::string &url);
    void deleteUser(const std::string &url);

    std::array<std::function<void(const std::string&)>, 14> _commandsFun = {
           [&](const std::string &s) { exit(s); },
           [&](const std::string &s) { clear(s); },
           [&](const std::string &s) { help(s); },
           [&](const std::string &s) { login(s); },
           [&](const std::string &s) { logout(s); },
           [&](const std::string &s) { registration(s); },
           [&](const std::string &s) { addUser(s); },
           [&](const std::string &s) { getSelf(s); },
           [&](const std::string &s) { getUser(s); },
           [&](const std::string &s) { getUsers(s); },
           [&](const std::string &s) { changeSelf(s); },
           [&](const std::string &s) { changeUser(s); },
           [&](const std::string &s) { deleteSelf(s); },
           [&](const std::string &s) { deleteUser(s); },
    };

public:
    Client() = default;
    Client(const Client &) = default;
    Client &operator=(const Client &) = default;
    Client &operator=(Client &&) noexcept = default;
    Client(Client &&) noexcept = default;
    ~Client() = default;

    void run(const std::string &url);

    void start();

    void stop();

};


#endif //CLIENT_CLIENT_HPP
