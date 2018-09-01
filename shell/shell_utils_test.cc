//
// Created by Liam Heeger on 8/30/18.
//

#include "gtest/gtest.h"
#include "shell_utils.h"

using namespace shell;

class ShellUtilsTest : public ::testing::Test {
    protected:
        void SetUp() override {
    
        };

        Command command_;
};

TEST_F(ShellUtilsTest, ParseCommand_Success1) {
    ASSERT_TRUE(ShellUtils::ParseCommand("ps aux", &command_));
}

TEST_F(ShellUtilsTest, ParseCommand_Success2) {
    ASSERT_TRUE(ShellUtils::ParseCommand("ps aux ", &command_));
}

TEST_F(ShellUtilsTest, ParseCommand_Success3) {
    ASSERT_TRUE(ShellUtils::ParseCommand(" ps aux", &command_));
}

TEST_F(ShellUtilsTest, ParseCommand_Success4) {
    // TODO, we will implement later.
    ASSERT_FALSE(true);
}

TEST_F(ShellUtilsTest, IsInternalCommand) {
}