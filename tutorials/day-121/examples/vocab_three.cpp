#include <any>
#include <iostream>
#include <map>
#include <optional>
#include <string>
#include <variant>
#include <vector>

// Day 121：optional / variant / any（计划指定：解析函数返回 optional<int> 表达失败）

// ===== 计划指定：安全解析函数 =====
// 旧 C 风格：bool parse_int(const char*, int& out) —— 输出参数 + 返回码（丑）
// 现代风格：std::optional<int> parse_int(std::string_view) —— 值语义的"可能没有"
std::optional<int> parse_int(const std::string& s) {
    if (s.empty()) { return std::nullopt; }                  // 没有
    int value = 0;
    bool neg = false;
    std::size_t i = 0;
    if (s[0] == '-') { neg = true; i = 1; if (s.size() == 1) return std::nullopt; }
    for (; i < s.size(); ++i) {
        if (s[i] < '0' || s[i] > '9') { return std::nullopt; }   // 非数字 → 失败
        value = value * 10 + (s[i] - '0');
    }
    return neg ? -value : value;                             // 有（包装进 optional）
}

// ===== variant：类型安全的联合体 =====
struct Circle { double r; };
struct Rect { double w, h; };
using Shape = std::variant<Circle, Rect>;                    // 只能是其中一种

double area(const Shape& s) {
    return std::visit([](const auto& sh) -> double {          // 按实际类型分派
        using T = std::decay_t<decltype(sh)>;
        if constexpr (std::is_same_v<T, Circle>) { return 3.14159 * sh.r * sh.r; }
        else { return sh.w * sh.h; }
    }, s);
}

// ===== any：能装任何东西的盒子（运行期类型） =====
void print_any(const std::any& box) {
    if (auto* p = std::any_cast<int>(&box)) { std::cout << "  int：" << *p << "\n"; }
    else if (auto* p = std::any_cast<std::string>(&box)) { std::cout << "  string：\"" << *p << "\"\n"; }
    else if (auto* p = std::any_cast<double>(&box)) { std::cout << "  double：" << *p << "\n"; }
    else { std::cout << "  其他类型（has_value=" << box.has_value() << "）\n"; }
}

int main() {
    std::cout << "== 计划指定：optional 表达失败 ==" << std::endl;
    std::vector<std::string> inputs = {"42", "-7", "3.14", "abc", "", "12x"};
    for (const auto& s : inputs) {
        if (auto r = parse_int(s)) {                           // 有值 → bool 上下文为 true
            std::cout << "  \"" << s << "\" → " << *r << "（成功）" << std::endl;
        } else {
            std::cout << "  \"" << s << "\" → 失败（nullopt）" << std::endl;
        }
    }
    std::cout << "  （对照旧风格：int out; if (parse(s, out))——输出参数的丑）" << std::endl;
    std::cout << "  （optional 把'可能失败'写进返回类型——调用方必须面对）" << std::endl;

    auto r = parse_int("100");
    std::cout << "  r.value_or(0) = " << r.value_or(0) << "（失败时的默认值）" << std::endl;
    auto bad = parse_int("xx");
    std::cout << "  bad.value_or(-1) = " << bad.value_or(-1) << std::endl;

    std::cout << "\n== variant：类型安全的联合 ==" << std::endl;
    std::vector<Shape> shapes = { Circle{2}, Rect{3, 4}, Circle{1} };
    for (const auto& s : shapes) {
        std::cout << "  面积 = " << area(s) << std::endl;
    }
    std::cout << "  （visit 按实际存的那种分派——穷尽检查）" << std::endl;

    std::cout << "\n== any：运行期的万能盒 ==" << std::endl;
    std::any b1 = 42;
    std::any b2 = std::string("hello");
    std::any b3 = 3.14;
    print_any(b1); print_any(b2); print_any(b3);
    std::cout << "  （any_cast 错类型 → 得 nullptr（指针版）或抛 bad_any_cast）" << std::endl;

    std::cout << "\n== 三者选型 ==" << std::endl;
    std::cout << "  optional<T>：可能没有的 T（失败/缺席）——静态一种类型" << std::endl;
    std::cout << "  variant<A,B,…>：只能是列出的几种——visit 穷尽" << std::endl;
    std::cout << "  any：真·什么都行（运行期才知道）——最不安全最灵活" << std::endl;
    std::cout << "  （能 optional 别 variant；能 variant 别 any——越具体越安全）" << std::endl;
    return 0;
}
