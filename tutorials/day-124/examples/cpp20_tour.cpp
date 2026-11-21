#include <concepts>
#include <iostream>
#include <ostream>
#include <ranges>
#include <string>
#include <vector>

// Day 124：C++20 概览——concepts / ranges（认知级·跑通示例）
// 本机 Apple clang 21 对 C++20 的支持已足够跑通 concepts 和 ranges

// ===== ① concepts：模板要求的"写在脸上" =====
template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;   // 数值概念

// 用法一：requires 子句（Day 97 的 duck typing 升级——要求显式）
template <Numeric T>
T add20(T a, T b) { return a + b; }

// 用法二：requires 表达式（更复杂的检查）
template <typename T>
concept Printable = requires(std::ostream& os, const T& v) {
    os << v;                                                    // 能 << 吗？
};

template <Printable T>
void print_line(const T& v) { std::cout << "  " << v << std::endl; }

// ===== ② ranges：管道风格的算法 =====
int main() {
    std::cout << "== ① concepts：要求写在脸上 ==" << std::endl;
    std::cout << "  add20(3, 4) = " << add20(3, 4) << "（int 满足 Numeric ✔）" << std::endl;
    std::cout << "  add20(2.5, 1.5) = " << add20(2.5, 1.5) << "（double ✔）" << std::endl;
    // add20(std::string("a"), std::string("b"));  // ✘ 编译错——concept 拦截（报错友好！）
    std::cout << "  （string 版会被 concept 拦截——报错信息直指'不满足 Numeric'）" << std::endl;
    print_line(42);
    print_line("hello concept");
    std::cout << "  （Printable：有 operator<< 就能印——要求写在签名上）" << std::endl;

    std::cout << "\n== ② ranges：管道风格 ==" << std::endl;
    std::vector<int> v = {8, 3, 10, 1, 7, 5, 9, 2};
    // C++20 前：sort + 拷贝筛选（三步、中间容器）
    // C++20 后：一个管道（读——改——用）
    auto result = v
        | std::views::filter([](int x) { return x > 3; })       // 筛
        | std::views::transform([](int x) { return x * 2; })    // 变换
        | std::views::take(3);                                   // 取前三
    std::cout << "  原始：8 3 10 1 7 5 9 2" << std::endl;
    std::cout << "  管道(>3 → ×2 → 前3)：";
    for (int x : result) { std::cout << x << ' '; }
    std::cout << std::endl;
    std::cout << "  （views 是惰性的——不产生中间容器）" << std::endl;

    std::cout << "\n== ranges 的惰性实证 ==" << std::endl;
    auto evens = std::views::iota(1)                            // 1,2,3,… 无穷序列
        | std::views::filter([](int x) { return x % 2 == 0; })
        | std::views::take(5);                                  // 只取 5 个
    std::cout << "  无穷序列取前 5 个偶数：";
    for (int x : evens) { std::cout << x << ' '; }
    std::cout << "（无穷也能玩——惰性的威力）" << std::endl;

    std::cout << "\n== C++20 三大件的定位 ==" << std::endl;
    std::cout << "  concepts：模板要求的类型系统（Day 97 隐式 → 显式）" << std::endl;
    std::cout << "  ranges：算法的管道组合（Day 91 的迭代器对升级）" << std::endl;
    std::cout << "  协程：co_await/co_yield 的挂起恢复（本编译器支持有限——认知即可）" << std::endl;
    std::cout << "  （今天的两个跑通；协程等工具链成熟再战）" << std::endl;
    return 0;
}
