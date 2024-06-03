
#include "GlobalErrors.hpp"

#include "User.hpp"

std::string User::AccessToStr(User::AccessLevel lvl) {
    switch (lvl)
    {
        case User::AccessLevel::user:
            return "user";
        case User::AccessLevel::moderator:
            return "moderator";
        case User::AccessLevel::admin:
            return "admin";
        case User::AccessLevel::undef:
            return "undef";
    }
#if __GNUC__ || __clang__
    __builtin_unreachable();
#endif
}

User::AccessLevel User::StrToAccess(const std::string &lvl_str) {
    if (lvl_str == "user")
        return User::AccessLevel::user;
    else if (lvl_str == "moderator")
        return User::AccessLevel::moderator;
    else if (lvl_str == "admin")
        return User::AccessLevel::admin;

    return User::AccessLevel::undef;
}
