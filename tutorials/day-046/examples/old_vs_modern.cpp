#include <iostream>
#include <string>
#include <vector>

// Day 46：auto 与范围 for——旧写法 vs 现代写法逐项对照（计划指定练习）
// 对照一：遍历 vector
void old_style_vector(const std::vector<int>& v) {
    std::cout << "  旧：";
    for (std::vector<int>::const_iterator it = v.begin(); it != v.end(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << std::endl;
}

void modern_style_vector(const std::vector<int>& v) {
    std::cout << "  新：";
    for (int x : v) {                    // 范围 for：只说"要什么"
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}

// 对照二：遍历下标 vs 范围 for（带 auto 的引用版——可写）
void double_all(std::vector<int>& v) {
    for (auto& x : v) {                  // auto& ——可写引用
        x *= 2;
    }
}

// 对照三：遍历字符串
void modern_string(const std::string& s) {
    std::cout << "  新：";
    for (char c : s) {
        std::cout << c << ' ';
    }
    std::cout << std::endl;
}

// 对照四：遍历数组（原生数组也能范围 for！）
void modern_array() {
    int arr[] = {3, 1, 4, 1, 5};
    std::cout << "  新：";
    for (int x : arr) {
        std::cout << x << ' ';
    }
    std::cout << std::endl;
}

int main() {
    std::cout << "== 对照一：迭代器 vs 范围 for ==" << std::endl;
    std::vector<int> v = {1, 2, 3, 4, 5};
    old_style_vector(v);
    modern_style_vector(v);

    std::cout << "\n== 对照二：可写遍历（auto&） ==" << std::endl;
    double_all(v);
    std::cout << "  翻倍后：";
    for (int x : v) { std::cout << x << ' '; }
    std::cout << std::endl;

    std::cout << "\n== 对照三：字符串 ==" << std::endl;
    modern_string("hello");

    std::cout << "\n== 对照四：原生数组 ==" << std::endl;
    modern_array();

    std::cout << "\n== auto 的三个面孔 ==" << std::endl;
    auto i = 42;                         // int
    auto d = 3.14;                       // double
    auto s = std::string("hi");          // std::string
    auto& r = i;                         // int&（引用）
    std::cout << "  auto i = 42        → " << sizeof(i) << " 字节（int）" << std::endl;
    std::cout << "  auto d = 3.14      → " << sizeof(d) << " 字节（double）" << std::endl;
    std::cout << "  auto s = string    → " << sizeof(s) << " 字节（string 对象）" << std::endl;
    std::cout << "  auto& r = i        → 引用（改 r 就是改 i）" << std::endl;
    r = 100;
    std::cout << "  r = 100 后 i = " << i << std::endl;
    std::cout << "  const auto&（函数参数常见）→ 只读引用省拷贝——Day 43 的 view 同思路" << std::endl;
    return 0;
}
