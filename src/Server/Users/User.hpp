
#ifndef SERVER_USER_HPP
#define SERVER_USER_HPP

#include <string>
#include <json.hpp>

struct User final {
    enum AccessLevel {
        undef = -1,
        user,
        moderator,
        admin
    };

    std::string uuid;
    std::string login;
    std::string password;
    AccessLevel access = AccessLevel::undef;

    User() = default;
    User(const User &) = default;
    User &operator=(const User &) = default;
    User &operator=(User &&) noexcept = default;
    User(User &&) noexcept = default;
    ~User() = default;

    [[maybe_unused]] static std::string AccessToStr(User::AccessLevel lvl);

    [[maybe_unused]] static User::AccessLevel StrToAccess(const std::string &lvl_str);
};


#endif //SERVER_USER_HPP
