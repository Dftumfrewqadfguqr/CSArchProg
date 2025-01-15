
#ifndef SERVER_USERSSTORAGE_HPP
#define SERVER_USERSSTORAGE_HPP

#include <string>
#include <map>
#include <optional>
#include <variant>
#include <json.hpp>

#include "User.hpp"
#include "../GlobalDefinitions.hpp"
#include "../GlobalErrors.hpp"

class UsersStorage final {
private:
    Mutex _storage_mtx;
    typedef typename std::map<std::string, User> Users;
    Users _storage GUARDED_BY(_storage_mtx) = Users();

    bool loginIsExist(const std::string &login);

    std::optional<User> getByLogin(const std::string &login);

    UsersStorage();
    ~UsersStorage() = default;

public:
    static UsersStorage &Instance()
    {
        static UsersStorage inst;
        return inst;
    }

    std::optional<errors::Error> AddUser(User &user);

    std::optional<errors::Error> DeleteUser(const std::string &uuid);

    std::variant<errors::Error, User> UpdateUser(const std::string &uuid, const nlohmann::json &j);

    std::optional<User> GetByUuid(const std::string &uuid);

    std::vector<User> GetAll();

    std::optional<User> Auth(const std::string &login, const std::string &password);
};


#endif //SERVER_USERSSTORAGE_HPP
