#include <gtest/gtest.h>

// Day 136：GoogleTest 入门——第一个测试用例（计划指定）
// 被测函数：一个简单的 add（教学用——明天起测 JSON 解析器）
int add(int a, int b) { return a + b; }

// ===== TEST 宏：测试用例的基本单元 =====
// TEST(套件名, 用例名) —— 套件分组、用例各跑各的
TEST(AddTest, TwoPlusTwo) {
    EXPECT_EQ(add(2, 2), 4);
}

TEST(AddTest, NegativeNumbers) {
    EXPECT_EQ(add(-3, -4), -7);
}

TEST(AddTest, Zero) {
    EXPECT_EQ(add(0, 0), 0);
}

// ===== 断言家族的体验 =====
TEST(Assertions, Family) {
    EXPECT_EQ(1 + 1, 2);              // 相等
    EXPECT_NE(1 + 1, 3);              // 不等
    EXPECT_LT(1, 2);                  // 小于
    EXPECT_LE(2, 2);                  // 小于等于
    EXPECT_GT(3, 2);                  // 大于
    EXPECT_GE(3, 3);                  // 大于等于
    EXPECT_TRUE(1 == 1);              // 真
    EXPECT_FALSE(1 == 2);             // 假
    EXPECT_FLOAT_EQ(0.1 + 0.2, 0.3);  // 浮点近似相等（4 位 ULP）
    EXPECT_DOUBLE_EQ(1.0 / 3.0 * 3.0, 1.0);
}

// ===== EXPECT vs ASSERT 的差异（核心概念） =====
TEST(CheckPointer, ExpectContinues) {
    int* p = new int(42);
    EXPECT_NE(p, nullptr);    // EXPECT 失败继续跑
    EXPECT_EQ(*p, 42);
    delete p;                 // 这行会执行——清理不漏
}

TEST(CheckPointer, AssertStops) {
    int* p = nullptr;
    if (p == nullptr) {
        // ASSERT 在某些位置（非 void 返回的函数）不能 return——
        // 但 TEST 宏的环境里 ASSERT 失败 = 立即 return
        ASSERT_NE(p, nullptr);   // 这里失败 → 本用例立即终止
        EXPECT_EQ(*p, 42);       // 不会执行（避免空指针解引用）
    }
}

// ===== 一个演示失败的用例（故意失败——看输出形态） =====
TEST(FailDemo, ThisOneFails) {
    EXPECT_EQ(add(2, 2), 5);   // 故意错——看失败报告的样子
}

// ===== main：gtest_main 库提供（也可手写带初始化） =====
// 链接 -lgtest_main 时不需要写 main
