
#ifndef SERVER_JSONCAST_HPP
#define SERVER_JSONCAST_HPP

#include <json.hpp>

#include "Users/User.hpp"
#include "GlobalErrors.hpp"

void to_json(nlohmann::json &j, const User &user);

void from_json(const nlohmann::json &j, User &user);

#endif //SERVER_JSONCAST_HPP
