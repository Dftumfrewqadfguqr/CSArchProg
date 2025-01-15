
#include "../Server/Users/User.hpp"
#include "../Server/Users/UsersStorage.hpp"

#include <hv/requests.h>
#include <gtest/gtest.h>
#include <Utility.hpp>

TEST(ServerTests, TestResponse) {
    struct User user;
    user.login = "admin";
    user.password = "password";
    user.access = User::admin;

    UsersStorage::Instance().AddUser(user);

    auto t = UsersStorage::Instance().GetAll();
    EXPECT_EQ(t.size(), 1);
}


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}