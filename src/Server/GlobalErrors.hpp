
#ifndef SERVER_GLOBALERRORS_HPP
#define SERVER_GLOBALERRORS_HPP

#include <string>

#define LOGIN_CONFLICT 1
#define INVALID_AUTH   2
#define ACCESS_DENIED  3
#define UNAUTH         4
#define UNKNOWN_ACCESS 5
#define UNKNOWN_USER   6

#define ERROR_MAP(XX)                                 \
    XX(LOGIN_CONFLICT, "This login is already taken") \
    XX(INVALID_AUTH,   "Wrong login or password")     \
    XX(ACCESS_DENIED,  "Access denied")               \
    XX(UNAUTH,         "Unauthorized")                \
    XX(UNKNOWN_ACCESS, "Unknown access")              \
    XX(UNKNOWN_USER,   "User not found")

namespace errors {
    class ErrorException : public std::exception {
    public:
        explicit ErrorException(int error_code, int http_code = -1)
        : _error_code(error_code), _http_code(http_code) {}

        ~ErrorException() override = default;

        [[nodiscard]] int error_code() const
        {
            return _error_code;
        }

        [[nodiscard]] int http_code() const
        {
            return _http_code;
        }

    private:
        int _error_code;
        int _http_code;
    };

    enum ERROR {
#define XX(code, _) ERR_##code = code,
        ERROR_MAP(XX)
#undef XX
        ERRNO_MAX = -1
    };

    const char *ErrName(int err);
    const char *StrError(int err);

    struct Error {
        int err_code;
        int http_code;

        Error() : err_code(-1), http_code(500) {}

        Error(int err_code_, int http_code_) : err_code(err_code_), http_code(http_code_) {}

        explicit Error(int err_code_) : err_code(err_code_), http_code(500) {}

        [[nodiscard]] std::string ToJson() const
        {
            std::string json;

            json += R"({"key":")" + std::string(ErrName(err_code)) + "\",";
            json += R"("message":")" + std::string(StrError(err_code)) + "\"}";

            return json;
        }
    };
}

#endif //SERVER_GLOBALERRORS_HPP
