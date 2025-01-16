
#include "Users/UsersStorage.hpp"
#include "JsonCast/JsonCast.hpp"
#include "Utility.hpp"

#include "hv/requests.h"
#include "Routers.hpp"

std::optional<User> Auth(const std::string &header)
{
    std::optional<User> user_opt;

    if (header.empty())
        return std::nullopt;

    auto split_header = Utility::Split(header, " ");
    if (split_header.size() == 2 && split_header.front() == "Basic") {
        auto decode = Utility::DecodeBase64(split_header.back());
        auto split_auth = Utility::Split(decode, ":");

        if (split_auth.size() != 2)
            throw errors::ErrorException(UNAUTH, 401);

        user_opt = UsersStorage::Instance().Auth(split_auth.front(), split_auth.back());

        if (!user_opt.has_value())
            throw errors::ErrorException(INVALID_AUTH, 401);
    }
    else {
        throw errors::ErrorException(UNAUTH, 401);
    }

    return user_opt.value();
}

void route::RegisterResources(hv::HttpService &router) {

    router.GET("/greet/{user_id}", [](HttpRequest *req, HttpResponse *resp)
    {
        std::string user_id = req->GetParam("user_id");
        auto res = requests::get(("http://microservice:5001/greet/" + user_id).c_str());

        if (res == nullptr)
        {
            std::cout << "request failed!" << std::endl;
            return 404;
        }
        std::cout << res->status_code << " " << res->status_message() << std::endl;
        std::cout << res->body.c_str() << std::endl;
        resp->status_code = res->status_code;
        resp->body = res->body;

        return 200; });

    // попытка входа
    router.GET("/login", [](HttpRequest *req, HttpResponse *resp) {
        nlohmann::json response;

        try {
            auto current_user = Auth(req->GetHeader("Authorization"));

            response = current_user.value();
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
        }
        catch(const errors::ErrorException& e) {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e) {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        return 200;
    });

    // добавление пользователя
    router.POST("/user", [](HttpRequest *req, HttpResponse *resp) {
        nlohmann::json request;
        nlohmann::json response;

        try {
            auto current_user = Auth(req->GetHeader("Authorization"));

            User user = nlohmann::json::parse(req->body);

            if (!current_user.has_value() && user.access > User::AccessLevel::user) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.has_value() && current_user.value().access < user.access) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            auto err = UsersStorage::Instance().AddUser(user);

            if (err.has_value()) {
                resp->SetBody(err.value().ToJson());
                resp->content_type = APPLICATION_JSON;
                return err.value().http_code;
            }
            else {
                response = user;
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
            }
        }
        catch(const errors::ErrorException& e) {
            errors::Error err(e.error_code(), e.http_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e) {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        return 200;
    });

    // пользователь по uuid
    router.GET("/user/{id}", [](HttpRequest *req, HttpResponse *resp) {
        auto user_id = req->GetParam("id");
        nlohmann::json response;

        try {
            auto current_user = Auth(req->GetHeader("Authorization"));

            if (!current_user.has_value()) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::moderator && current_user.value().uuid != user_id) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            auto res = UsersStorage::Instance().GetByUuid(user_id);

            if (res.has_value()) {
                response = res.value();
                resp->SetBody(response.dump());
            }
            else {
                errors::Error err(UNKNOWN_USER);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 404;
            }
        }
        catch(const errors::ErrorException& e) {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e) {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        resp->content_type = APPLICATION_JSON;
        return 200;
    });

    // список всех пользователей
    router.GET("/users", [](HttpRequest *req, HttpResponse *resp) {
        nlohmann::json response;

        try {
            auto current_user = Auth(req->GetHeader("Authorization"));

            if (!current_user.has_value()) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::moderator) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            response = UsersStorage::Instance().GetAll();
        }
        catch(const errors::ErrorException& e) {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e) {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        resp->SetBody(response.dump());
        resp->content_type = APPLICATION_JSON;

        return 200;
    });

    // изменение пользователя
    router.PATCH("/user/{id}", [](HttpRequest *req, HttpResponse *resp) {
        nlohmann::json request;
        nlohmann::json response;

        auto user_id = req->GetParam("id");

        try {
            auto current_user = Auth(req->GetHeader("Authorization"));
            if (!current_user.has_value()) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::admin && current_user.value().uuid != user_id) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            request = nlohmann::json::parse(req->body);

            auto res = UsersStorage::Instance().UpdateUser(user_id, request);

            if (std::holds_alternative<errors::Error>(res)) {
                auto err = std::get<errors::Error>(res);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return err.http_code;
            }
            else if (std::holds_alternative<User>(res)) {
                auto user = std::get<User>(res);
                response = user;
                resp->SetBody(response.dump());
                resp->content_type = APPLICATION_JSON;
            }
        }
        catch(const errors::ErrorException& e) {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e) {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        return 200;
    });

    // удаление пользователя
    router.Delete("/user/{id}", [](HttpRequest *req, HttpResponse *resp) {
        nlohmann::json response;

        auto user_id = req->GetParam("id");

        try {
            auto current_user = Auth(req->GetHeader("Authorization"));
            if (!current_user.has_value()) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }
            else if (current_user.value().access < User::AccessLevel::admin && current_user.value().uuid != user_id) {
                errors::Error err(ACCESS_DENIED);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return 403;
            }

            auto res = UsersStorage::Instance().DeleteUser(user_id);
            if (res.has_value()) {
                errors::Error err(res.value().err_code);
                resp->SetBody(err.ToJson());
                resp->content_type = APPLICATION_JSON;
                return res.value().http_code;
            }
        }
        catch(const errors::ErrorException& e) {
            errors::Error err(e.error_code());
            resp->SetBody(err.ToJson());
            resp->content_type = APPLICATION_JSON;
            return e.http_code();
        }
        catch(const std::exception& e) {
            response["error"] = "Invalid JSON";
            resp->SetBody(response.dump());
            resp->content_type = APPLICATION_JSON;
            return 400;
        }

        return 200;
    });

}
