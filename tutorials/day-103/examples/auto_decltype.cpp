#include <iostream>
#include <map>
#include <string>
#include <vector>

// Day 103：auto / decltype——规则与陷阱（计划指定：用 auto 重写旧代码）
int main() {
    std::cout << "== auto 的基本推导 ==" << std::endl;
    auto i = 42;                     // int
    auto d = 3.14;                   // double
    auto s = std::string("hello");   // std::string
    std::cout << "  auto i=42 → int（" << sizeof(i) << " 字节）" << std::endl;
    std::cout << "  auto d=3.14 → double（" << sizeof(d) << " 字节）" << std::endl;
    std::cout << "  auto s=\"hello\" → string（" << sizeof(s) << " 字节）" << std::endl;

    std::cout << "\n== 最常见的坑：auto 拷贝语义 ==" << std::endl;
    std::vector<int> v = {1, 2, 3};
    auto x = v[0];                   // 拷贝！v[0] 是 int&，auto 退化为 int
    (void)x;
    x = 999;
    std::cout << "  auto x = v[0]; x=999 后 v[0] 仍 = " << v[0]
              << "（x 是拷贝不是引用）" << std::endl;
    auto& r = v[0];                  // 要引用必须写 &
    r = 777;
    std::cout << "  auto& r = v[0]; r=777 后 v[0] = " << v[0]
              << "（& 才是引用）" << std::endl;
    std::cout << "  （规则：auto = 值语义（拷贝）；auto& = 左值引用；auto&& = 万能引用）" << std::endl;

    std::cout << "\n== 代理类型陷阱（vector<bool> 经典） ==" << std::endl;
    std::vector<bool> flags = {true, false, true};
    auto f = flags[0];               // vector<bool>::reference —— 代理！不是 bool
    std::cout << "  auto f = flags[0]; f = " << f
              << "（推导出的是代理类型不是 bool！）" << std::endl;
    bool real_b = flags[0];          // 显式 bool 才拿到真值类型
    (void)real_b;
    std::cout << "  bool real_b = flags[0]; → 真正的 bool" << std::endl;
    std::cout << "  （Effective Modern C++ 条款 6：auto 推出代理时宁可显式写类型）" << std::endl;

    std::cout << "\n== decltype：查类型不求值 ==" << std::endl;
    int a = 10;
    decltype(a) another = 20;        // another 的类型 = a 的类型（int）
    decltype(a + 1) sum_type = 30;   // 表达式类型（int）
    std::cout << "  decltype(a) another = " << another
              << "；decltype(a+1) sum_type = " << sum_type << std::endl;
    std::cout << "  （decltype 只查类型不执行表达式——零运行时）" << std::endl;

    std::cout << "\n== 尾置返回类型（auto + decltype 组合） ==" << std::endl;
    std::cout << "  auto add(T a, U b) -> decltype(a + b) 的形态：" << std::endl;
    std::cout << "  add(3, 4.5) = " << [](auto p, auto q) { return p + q; }(3, 4.5)
              << "（3+4.5=7.5——不同类型相加）" << std::endl;
    std::cout << "  （Day 97 的 myMax2 窄化坑 → 今天 auto 返回的正确解）" << std::endl;

    std::cout << "\n== 计划指定：用 auto 重写旧代码 ==" << std::endl;
    // 重写前（Day 85 风格的冗长迭代器）
    std::map<std::string, int> ages = {{"小明", 15}, {"小红", 14}};
    std::cout << "  重写前：std::map<std::string, int>::iterator it = m.begin();" << std::endl;
    std::cout << "  重写后：auto it = m.begin();（短 32 个字符）" << std::endl;
    for (auto it = ages.begin(); it != ages.end(); ++it) {
        std::cout << "    " << it->first << " = " << it->second << std::endl;
    }
    std::cout << "  结构化绑定（C++17）更进一步：for (auto& [k, v] : m)" << std::endl;
    for (auto& [k, v] : ages) { std::cout << "    " << k << " = " << v << std::endl; }
    return 0;
}
