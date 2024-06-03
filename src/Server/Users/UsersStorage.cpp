
#include <Utility.hpp>

#include "UsersStorage.hpp"

bool UsersStorage::loginIsExist(const std::string &login) {
    for (const auto &item : _storage) if (item.second.login == login) return true;
    return false;
}

std::optional<User> UsersStorage::getByLogin(const std::string &login) {
    for (const auto &item : _storage) if (item.second.login == login) return item.second;
    return std::nullopt;
}

UsersStorage::UsersStorage() {
    User default_admin;
    default_admin.uuid = Utility::GenerateUUID();
    default_admin.login = "admin";
    default_admin.password = "admin";
    default_admin.access = User::AccessLevel::admin;

    _storage.emplace(default_admin.uuid, default_admin);
}

std::optional<errors::Error> UsersStorage::AddUser(User &user) {

    user.uuid = Utility::GenerateUUID();

    LockGuard lk(_storage_mtx);

    if (loginIsExist(user.login))
        return errors::Error(LOGIN_CONFLICT, 409);

    _storage.emplace(user.uuid, user);

    return std::nullopt;
}

std::optional<errors::Error> UsersStorage::DeleteUser(const std::string &uuid) {
    LockGuard lk(_storage_mtx);

    auto it = _storage.find(uuid);
    if (it == _storage.end())
        return errors::Error(UNKNOWN_USER, 404);

    _storage.erase(it);

    return std::nullopt;
}

std::variant<errors::Error, User> UsersStorage::UpdateUser(const std::string &uuid, const nlohmann::json &j) {
    LockGuard lk(_storage_mtx);

    User user;

    auto it = _storage.find(uuid);
    if (it == _storage.end()) return errors::Error(UNKNOWN_USER, 404);
    user = it->second;

    if (j.contains("password") && j.at("password").is_string())
        user.password = j["password"];

    if (j.contains("login") && j.at("login").is_string()) {
        std::string value = j["login"];
        if (user.login != value) {
            if (loginIsExist(value))
                return errors::Error(LOGIN_CONFLICT, 409);

            user.login = value;
        }
    }

    if (j.contains("access") && j.at("access").is_string()) {
        auto access = User::StrToAccess(j["access"]);
        if (access == User::AccessLevel::undef)
            return errors::Error(UNKNOWN_ACCESS, 400);

        user.access = access;
    }

    _storage.erase(_storage.find(uuid));
    _storage.emplace(user.uuid, user);

    return user;
}

std::optional<User> UsersStorage::GetByUuid(const std::string &uuid) {
    LockGuard lk(_storage_mtx);

    auto it = _storage.find(uuid);

    if (it == _storage.end())
        return std::nullopt;

    auto value = it->second;
    return value;
}

std::vector<User> UsersStorage::GetAll() {
    LockGuard lk(_storage_mtx);

    std::vector<User> users;
    for (const auto &item : _storage) users.push_back(item.second);

    return users;
}

std::optional<User> UsersStorage::Auth(const std::string &login, const std::string &password) {
    LockGuard lk(_storage_mtx);

    auto user = getByLogin(login);

    if (!user.has_value())
        return std::nullopt;

    if (user.value().password != password)
        return std::nullopt;

    return user.value();
}
