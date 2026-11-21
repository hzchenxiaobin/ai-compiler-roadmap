// json_tests —— CMake + GoogleTest 的集成验证
#include <gtest/gtest.h>

TEST(CMakeIntegration, GTestFound) {         // find_package 找到了真库
    EXPECT_EQ(1 + 1, 2);
}

TEST(CMakeIntegration, Standard) {           // C++20 特性可用
    constexpr int v = []{ return 42; }();
    EXPECT_EQ(v, 42);
}
