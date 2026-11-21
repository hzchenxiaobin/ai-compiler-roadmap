#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Day 91：非修改算法——find/find_if/count_if/all_of 全配 lambda（计划指定）
int main() {
    std::vector<int> v = {3, 7, 2, 9, 4, 8, 1};

    std::cout << "== find：找值 ==" << std::endl;
    auto it = std::find(v.begin(), v.end(), 9);
    if (it != v.end()) {
        std::cout << "  find(9) → 位置 " << (it - v.begin())
                  << "（随机访问迭代器可减——Day 88 的能力）" << std::endl;
    }
    auto miss = std::find(v.begin(), v.end(), 100);
    std::cout << "  find(100) → " << (miss == v.end() ? "end()（没找到）" : "找到了") << std::endl;

    std::cout << "\n== find_if：找满足条件的第一个 ==" << std::endl;
    auto big = std::find_if(v.begin(), v.end(), [](int x) { return x > 6; });
    std::cout << "  第一个 >6 的：" << *big << "（位置 " << (big - v.begin()) << "）" << std::endl;

    std::cout << "\n== count_if：数满足条件的个数 ==" << std::endl;
    int evens = std::count_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; });
    std::cout << "  偶数个数：" << evens << std::endl;
    int bigs = std::count_if(v.begin(), v.end(), [](int x) { return x >= 7; });
    std::cout << "  >=7 的个数：" << bigs << std::endl;

    std::cout << "\n== all_of / any_of / none_of：整体判断三兄弟 ==" << std::endl;
    bool all_pos = std::all_of(v.begin(), v.end(), [](int x) { return x > 0; });
    std::cout << "  全部为正？" << (all_pos ? "是" : "否") << std::endl;
    bool any_ten = std::any_of(v.begin(), v.end(), [](int x) { return x == 10; });
    std::cout << "  有 10 吗？" << (any_ten ? "有" : "没有") << std::endl;
    bool none_neg = std::none_of(v.begin(), v.end(), [](int x) { return x < 0; });
    std::cout << "  没有负数？" << (none_neg ? "是" : "否") << std::endl;

    std::cout << "\n== lambda 的捕获：条件可以是外部变量 ==" << std::endl;
    int threshold = 5;
    int above = std::count_if(v.begin(), v.end(), [threshold](int x) { return x > threshold; });
    std::cout << "  >" << threshold << " 的个数：" << above << "（threshold 被捕获进 lambda）" << std::endl;
    threshold = 8;
    above = std::count_if(v.begin(), v.end(), [threshold](int x) { return x > threshold; });
    std::cout << "  阈值改 " << threshold << " 后：" << above << "（同一 lambda 模板、不同捕获值）" << std::endl;

    std::cout << "\n== 字符串版实战 ==" << std::endl;
    std::vector<std::string> words = {"apple", "banana", "cherry", "avocado"};
    auto a_word = std::find_if(words.begin(), words.end(),
                               [](const std::string& s) { return s[0] == 'a'; });
    std::cout << "  第一个 a 开头的：" << *a_word << std::endl;
    int long_words = std::count_if(words.begin(), words.end(),
                                   [](const std::string& s) { return s.size() >= 6; });
    std::cout << "  长度 >=6 的：" << long_words << " 个" << std::endl;
    return 0;
}
