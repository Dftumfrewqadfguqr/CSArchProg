
#include <hv/requests.h>
#include <gtest/gtest.h>
#include <Utility.hpp>

TEST(ServerTests, TestResponse) {

    std::string login="admin", password="password";

    http_headers headers;

    headers["Content-Type"] = "application/json";
    headers["Authorization"] = "Basic " + Utility::EncodeBase64(login + ":" + password);

    EXPECT_EQ(headers["Authorization"], "Basic YWRtaW46cGFzc3dvcmQ=");
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
