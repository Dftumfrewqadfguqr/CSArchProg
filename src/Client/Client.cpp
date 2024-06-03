
#include <iostream>
#include <json.hpp>
#include <Utility.hpp>

#include "Client.hpp"

#ifdef _WIN32 //|| _WIN64
#define _clear system("cls");
#else
#define _clear std::cout << "\x1B[2J\x1B[H";
#endif

http_headers Client::getHeaders() {
    http_headers headers;

    headers["Content-Type"] = "application/json";
    if (_login.empty()) headers["Authorization"] = "";
    else headers["Authorization"] = "Basic " + Utility::EncodeBase64(_login + ":" + _password);

    return headers;
}

void Client::exit(const std::string &url) {
    stop();
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "readability-convert-member-functions-to-static"
void Client::clear(const std::string &url) {
    _clear
}
#pragma clang diagnostic pop

void Client::help(const std::string &url) {
    for (size_t i = 0; i < _commandsID.size(); i++) {
        std::cout << _commandsID[i];
        std::cout << " - ";
        std::cout << _commandsOutlines[i];
        std::cout << std::endl;
    }
}

void Client::login(const std::string &url) {
    std::string login, password;
    login = _login;
    password = _password;

    std::cout << "Login: ";
    std::cin >> _login;
    std::cin.ignore();
    std::cout << "Password: ";
    std::cin >> _password;
    std::cin.ignore();

    auto resp = requests::get((url + "/login").c_str(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else {
        std::cout << "Authorization successful" << std::endl;
        _uuid = resp->GetString("id");
        _access = resp->GetString("access");
        return;
    }
    _login = login;
    _password = password;
}

void Client::logout(const std::string &url) {
    clear("");
    _login = "";
    _password = "";
    _uuid = "";
    _access = "";
}

void Client::registration(const std::string &url) {
    std::string login, password, access;

    std::cout << "Login: ";
    std::cin >> login;
    std::cin.ignore();
    std::cout << "Password: ";
    std::cin >> password;
    std::cin.ignore();
    access = "user";

    hv::Json data;
    data["login"] = login;
    data["password"] = password;
    data["access"] = access;

    auto resp = requests::post((url + "/user").c_str(), data.dump(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else std::cout << "Registration successful" << std::endl;
}

void Client::addUser(const std::string &url) {
    std::string login, password, access;

    std::cout << "Login: ";
    std::cin >> login;
    std::cin.ignore();
    std::cout << "Password: ";
    std::cin >> password;
    std::cin.ignore();
    std::cout << "Access[user|moderator|admin]: ";
    std::cin >> access;
    std::cin.ignore();

    hv::Json data;
    data["login"] = login;
    data["password"] = password;
    data["access"] = access;

    auto resp = requests::post((url + "/user").c_str(), data.dump(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else std::cout << "Registration successful" << std::endl;
}

void Client::getSelf(const std::string &url) {
    auto resp = requests::get((url + "/user/" + _uuid).c_str(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else {
        std::cout << "\tuuid: " << resp->GetString("id") << std::endl;
        std::cout << "\t\tlogin: " << resp->GetString("login") << std::endl;
        std::cout << "\t\taccess: " << resp->GetString("access") << std::endl;
        return;
    }
}

void Client::getUser(const std::string &url) {
    std::string uuid;
    std::cout << "uuid: ";
    std::cin >> uuid;
    std::cin.ignore();

    auto resp = requests::get((url + "/user/" + uuid).c_str(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else {
        std::cout << "\tuuid: " << resp->GetString("id") << std::endl;
        std::cout << "\t\tlogin: " << resp->GetString("login") << std::endl;
        std::cout << "\t\taccess: " << resp->GetString("access") << std::endl;
        return;
    }
}

void Client::getUsers(const std::string &url) {
    nlohmann::json j;
    auto resp = requests::get((url + "/users").c_str(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->body.c_str() << std::endl;
    }
#endif
    else {
        j = nlohmann::json::parse(resp->body);

        for (const auto& item : j) {
            std::cout << "\tuuid: " << item["id"] << std::endl;
            std::cout << "\t\tlogin: " << item["login"] << std::endl;
            std::cout << "\t\taccess: " << item["access"] << std::endl;
        }
    }
}

void Client::changeSelf(const std::string &url) {
    std::string newLogin, newPassword, newAccess;

    std::cout << "New login: ";
    std::cin >> newLogin;
    std::cin.ignore();
    std::cout << "New password: ";
    std::cin >> newPassword;
    std::cin.ignore();
    std::cout << "Access[user|moderator|admin]: ";
    std::cin >> newAccess;
    std::cin.ignore();

    hv::Json data;
    data["login"] = newLogin;
    data["password"] = newPassword;
    data["access"] = newAccess;

    auto resp = requests::patch((url + "/user/" + _uuid).c_str(), data.dump(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else std::cout << "Changes saved successful" << std::endl;

    _login = resp->GetString("login");
    _password = resp->GetString("password");
    _access = resp->GetString("access");
}

void Client::changeUser(const std::string &url) {
    std::string uuid, newLogin, newPassword, newAccess;

    std::cout << "User uuid: ";
    std::cin >> uuid;
    std::cin.ignore();
    std::cout << "New login: ";
    std::cin >> newLogin;
    std::cin.ignore();
    std::cout << "New password: ";
    std::cin >> newPassword;
    std::cin.ignore();
    std::cout << "Access[user|moderator|admin]: ";
    std::cin >> newAccess;
    std::cin.ignore();

    hv::Json data;
    data["login"] = newLogin;
    data["password"] = newPassword;
    data["access"] = newAccess;

    auto resp = requests::patch((url + "/user/" + uuid).c_str(), data.dump(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else std::cout << "Changes saved successful" << std::endl;
}

void Client::deleteSelf(const std::string &url) {
    auto resp = requests::Delete((url + "/user/" + _uuid).c_str(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
        else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else std::cout << "Deleted successful" << std::endl;
}

void Client::deleteUser(const std::string &url) {
    std::string uuid;

    std::cout << "User uuid: ";
    std::cin >> uuid;
    std::cin.ignore();

    auto resp = requests::Delete((url + "/user/" + uuid).c_str(), getHeaders());

    if (resp == nullptr)
        std::cout << "request failed!" << std::endl;
#ifdef DEBUG_PROCESS
    else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->status_code << " " << resp->status_message() << std::endl;
        std::cout << resp->body.c_str() << std::endl;
    }
#else
        else if (resp->status_code != HTTP_STATUS_OK) {
        std::cout << resp->GetString("message") << std::endl;
    }
#endif
    else std::cout << "Deleted successful" << std::endl;
}

void Client::run(const std::string &url) {
    char buf[256];
    int bi;
    std::string input;
    bool handled;

    help("");
    std::cout << std::endl;

    while (_running) {
        std::cout << "CSA " << (_login.empty()? "Undefined" : "@" + _login) << "/";
        fgets(buf, 256, stdin);
        for(bi = 0; buf[bi] != '\n'; bi++);
        buf[bi] = '\0';
        input = buf;
        if (input.empty()) continue;
        handled = false;

        for (size_t i = 0; i < _commandsID.size(); i++) {
            if (input == _commandsID[i]) {
                _commandsFun[i](url);
                handled = true;
                break;
            }
        }
        if (!handled) std::cout << "Not recognized" << std::endl;
    }
}

void Client::start() {
    _running = true;
}

void Client::stop() {
    _running = false;
}
