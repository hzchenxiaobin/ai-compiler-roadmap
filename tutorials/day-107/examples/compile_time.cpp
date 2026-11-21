#include <iostream>
#include <string>
#include <type_traits>
#include <vector>

// ===== Day 107：if constexpr 与编译期计算——编译期阶乘（计划指定） =====

// 计划指定：编译期阶乘（Day 105 的函数 + 今天的 static_assert 验证）
constexpr int factorial(int n) {
    return (n <= 1) ? 1 : n * factorial(n - 1);
}
static_assert(factorial(5) == 120, "编译期断言：5! = 120");        // 编译不过 = 断言失败！
static_assert(factorial(10) == 3628800, "10! 也在编译期验证");

// if constexpr：按类型裁剪分支（false 的分支根本不编译！）
template <typename T>
auto describe(const T& value) {
    if constexpr (std::is_integral_v<T>) {             // 整数类型？
        return value + 1;                              // 整数版：+1
    } else {
        return value + 0.5;                            // 浮点版：+0.5
    }
}

// 经典应用：同一段代码对 int/double 各取所需（不用重载两个函数）
template <typename T>
T safe_half(const T& value) {
    if constexpr (std::is_integral_v<T>) {
        return value / 2;                               // 整数除法（截断——整数语义）
    } else {
        return value * 0.5;                             // 浮点乘（精确——浮点语义）
    }
}

// 运行期 if 的对照：两个分支都要能编译（类型不匹配就崩）
int main() {
    std::cout << "== 计划指定：编译期阶乘 ==" << std::endl;
    constexpr int f5 = factorial(5);
    constexpr int f8 = factorial(8);
    std::cout << "  constexpr 5! = " << f5 << "（编译期算好）" << std::endl;
    std::cout << "  constexpr 8! = " << f8 << std::endl;
    std::cout << "  static_assert(5! == 120) 已在编译期通过——编译成功即证明 ✔" << std::endl;
    std::cout << "  （写错的版本编译不过——编译期测试的形态）" << std::endl;

    std::cout << "\n== if constexpr：编译期分支裁剪 ==" << std::endl;
    std::cout << "  describe(10) = " << describe(10) << "（int → +1 分支）" << std::endl;
    std::cout << "  describe(2.0) = " << describe(2.0) << "（double → +0.5 分支）" << std::endl;
    std::cout << "  （false 的分支根本不实例化——类型错误也不报）" << std::endl;

    std::cout << "\n== safe_half：同码不同语义 ==" << std::endl;
    std::cout << "  safe_half(9) = " << safe_half(9) << "（整数除法——4）" << std::endl;
    std::cout << "  safe_half(9.0) = " << safe_half(9.0) << "（浮点乘——4.5）" << std::endl;
    std::cout << "  （一个模板两种语义——按类型裁剪）" << std::endl;

    std::cout << "\n== 对照：运行期 if 两个分支都要合法 ==" << std::endl;
    int i = 10;
    if (i > 5) {
        std::cout << "  运行期 if：走这分支" << std::endl;
        // 若这里写了 value+0.5 而 value 是 int——照编译不误（隐式转换）
    } else {
        std::cout << "  另一分支（没走但也要编译过）" << std::endl;
    }
    std::cout << "  （运行期 if 的两个分支都进二进制；if constexpr 只进真的那个）" << std::endl;

    std::cout << "\n== Day 88 的标签分发——if constexpr 版重写 ==" << std::endl;
    std::vector<int> v = {1, 2, 3, 4, 5};
    auto sum_if_constexpr = [](auto first, auto last) {
        auto total = *first; ++first;                   // 第一个当初值
        for (; first != last; ++first) { total += *first; }
        return total;
    };
    std::cout << "  求和（if constexpr 风格的泛型版）= "
              << sum_if_constexpr(v.begin(), v.end()) << std::endl;
    std::cout << "  （Day 88 的 advance 按标签选路——if constexpr 是更直白的现代写法）" << std::endl;
    return 0;
}
