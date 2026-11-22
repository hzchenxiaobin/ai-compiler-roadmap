#include <iostream>
#include <string>
#include <utility>

// Day 113：完美转发——万能引用与 std::forward（计划指定：emplace_back 简化版）
class Widget {
public:
    Widget()                    { std::cout << "  [构造] 默认\n"; }
    Widget(const std::string& s) { std::cout << "  [构造] 从 const string&：\"" << s << "\"（拷贝路径）\n"; }
    Widget(std::string&& s)     { std::cout << "  [构造] 从 string&&：\"" << s << "\"（移动路径！）\n"; }
};

void process(const std::string& s);
void process(std::string&& s);

// ===== 万能引用 + 引用折叠的转发中间层 =====
// T&& 在模板推导语境 = 万能引用（左值来 T=string& → string& && 折叠成 string&；右值来 T=string → string&&）
template <typename T>
void relay(T&& arg) {
    std::cout << "  [relay] 收到 → 转发：";
    process(std::forward<T>(arg));              // forward 保持原类别！
}

void process(const std::string& s) { std::cout << "[process&] 左值 \"" << s << "\"\n"; }
void process(std::string&& s)      { std::cout << "[process&&] 右值 \"" << s << "\"（可移动）\n"; }

// ===== 不转发版 vs 转发版的对照 =====
template <typename T>
void relay_wrong(T&& arg) {
    std::cout << "  [relay_wrong] arg 本身是左值（有名字！）→ ";
    process(arg);                               // 永远走左值版（Day 110 的坑）
}

// ===== 计划指定：emplace_back 简化版 =====
// 用 string 参数构造 Widget——看 const&（拷贝路径）还是 &&（移动路径）
template <typename T>
class MiniVector {
public:
    MiniVector() : n_(0) {}                      // 预留槽位（不预构造——演示从简）
    template <typename U>
    void emplace_back(U&& v) {                   // 万能引用 + 转发
        new (slot(n_)) T(std::forward<U>(v));    // 就地构造：怎么来的就怎么传
        ++n_;
    }
    void push_back(const T& v) {                 // 对照：传统 push 的拷贝路径
        new (slot(n_)) T(v);
        ++n_;
    }
    int size() const { return n_; }
private:
    void* slot(int) { return raw_; }
    alignas(T) char raw_[4 * sizeof(T)];
    int n_;
};

int main() {
    std::cout << "== 万能引用：左值右值都收 ==" << std::endl;
    std::string s = "我是左值";
    relay(s);                    // 左值 → T=string& → 折叠 string&
    relay(std::string("我是右值"));  // 右值 → T=string
    std::cout << "  （同一个 T&&——两种来路都收 = 万能）" << std::endl;

    std::cout << "\n== 不转发的坑（Day 110 复习） ==" << std::endl;
    relay_wrong(s);                              // 左值 → 左值（碰巧对）
    relay_wrong(std::string("右值"));            // 右值也走左值版！（arg 有名字）
    std::cout << "  （转发层必须 forward——不然后半程类别丢失）" << std::endl;

    std::cout << "\n== 计划指定：emplace_back 简化版 ==" << std::endl;
    MiniVector<Widget> mv;
    std::string material = "原材料";
    std::cout << "emplace_back(左值 material)：" << std::endl;
    mv.emplace_back(material);                   // 左值 → const& 构造（拷贝路径）
    std::cout << "emplace_back(move(material))：" << std::endl;
    mv.emplace_back(std::move(material));        // 右值 → && 构造（移动路径！）
    std::cout << "  （同一个 emplace——forward 保持来路的类别）" << std::endl;
    std::cout << "  （这就是 Day 79 vector::emplace_back 快的原理）" << std::endl;

    std::cout << "\n== 引用折叠速查 ==" << std::endl;
    std::cout << "  T=string  时 T&& = string&&（右值引用）" << std::endl;
    std::cout << "  T=string& 时 T&& = string& && → string&（左值引用！）" << std::endl;
    std::cout << "  （折叠规则：有 & 就 &——只有 &&+&& 才 &&）" << std::endl;
    std::cout << "  （万能引用 = 模板 T&& + 推导语境——auto&& 同理）" << std::endl;
    return 0;
}
