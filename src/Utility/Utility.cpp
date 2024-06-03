
#include <memory>
#include <cstring>
#include <random>
#include <sstream>

#include "Utility.hpp"

#define FF_ARRAY_ELEMS(a) (sizeof(a) / sizeof((a)[0]))
#define AV_BASE64_SIZE(x) (((x) + 2) / 3 * 4 + 1)

static std::random_device              rd;
static std::mt19937                    gen(rd());
static std::uniform_int_distribution<> dis(0, 15);
static std::uniform_int_distribution<> dis2(8, 11);

static const uint8_t map2[] = {
        0x3e, 0xff, 0xff, 0xff, 0x3f, 0x34, 0x35, 0x36,
        0x37, 0x38, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0xff,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x00, 0x01,
        0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09,
        0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f, 0x10, 0x11,
        0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19,
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x1a, 0x1b,
        0x1c, 0x1d, 0x1e, 0x1f, 0x20, 0x21, 0x22, 0x23,
        0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2a, 0x2b,
        0x2c, 0x2d, 0x2e, 0x2f, 0x30, 0x31, 0x32, 0x33
};

int av_base64_decode(uint8_t *out, const char *in, int out_size) {
    int i, v;
    uint8_t *dst = out;

    v = 0;
    for (i = 0; in[i] && in[i] != '='; i++)
    {
        unsigned int index = in[i] - 43;
        if (index >= FF_ARRAY_ELEMS(map2) || map2[index] == 0xff)
            return -1;
        v = (v << 6) + map2[index];
        if (i & 3)
        {
            if (dst - out < out_size)
            {
                *dst++ = v >> (6 - 2 * (i & 3));
            }
        }
    }

    return (int)(dst - out);
}

char *av_base64_encode_l(char *out, int *out_size, const uint8_t *in, int in_size) {
    static const char b64[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    char *ret, *dst;
    unsigned i_bits = 0;
    int i_shift = 0;
    int bytes_remaining = in_size;

    if ((size_t)in_size >= std::numeric_limits<unsigned int>::max() / 4 || *out_size < AV_BASE64_SIZE(in_size))
    {
        return nullptr;
    }
    ret = dst = out;
    while (bytes_remaining)
    {
        i_bits = (i_bits << 8) + *in++;
        bytes_remaining--;
        i_shift += 8;

        do
        {
            *dst++ = b64[(i_bits << 6 >> i_shift) & 0x3f];
            i_shift -= 6;
        } while (i_shift > 6 || (bytes_remaining == 0 && i_shift > 0));
    }
    while ((dst - ret) & 3)
        *dst++ = '=';
    *dst = '\0';

    *out_size = (int)(dst - out);
    return ret;
}

char *av_base64_encode(char *out, int out_size, const uint8_t *in, int in_size) {
    return av_base64_encode_l(out, &out_size, in, in_size);
}

namespace Utility {

    std::string EncodeBase64(const std::string &txt) {
        if (txt.empty()) {
            return "";
        }
        int size = AV_BASE64_SIZE(txt.size()) + 10;
        std::string ret;
        ret.resize(size);

        if (!av_base64_encode_l((char *)ret.data(), &size, (const uint8_t *)txt.data(), (int)txt.size())) {
            return "";
        }
        ret.resize(size);
        return ret;
    }

    std::string DecodeBase64(const std::string &txt) {
        if (txt.empty()) {
            return "";
        }
        std::string ret;
        ret.resize(txt.size() * 3 / 4 + 10);
        auto size = av_base64_decode((uint8_t *)ret.data(), txt.data(), (int)ret.size());

        if (size <= 0) {
            return "";
        }
        ret.resize(size);
        return ret;
    }

    std::vector<std::string> Split(const std::string &s, const std::string &delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back (token);
        }

        res.push_back (s.substr (pos_start));
        return res;
    }

    std::string GenerateUUID() {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 4; i++) {
            ss << dis(gen);
        }
        ss << "-4";
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++) {
            ss << dis(gen);
        }
        ss << "-";
        for (i = 0; i < 12; i++) {
            ss << dis(gen);
        };
        return ss.str();
    }

} // Utility