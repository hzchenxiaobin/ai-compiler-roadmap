#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Day 110：右值引用与 std::move 的本质——只是类型转换（计划指定：打日志验证）
class Traced {
public:
    Traced(int v) : v_(v)      { std::cout << "  [构造] Traced(" << v << ")\n"; }
    Traced(const Traced& o) : v_(o.v_) { std::cout << "  [拷贝] Traced(" << o.v_ << ")\n"; }
    Traced(Traced&& o) noexcept : v_(o.v_) { std::cout << "  [移动] Traced(" << o.v_ << ")（偷资源）\n"; }
    ~Traced()                   { std::cout << "  [析构] ~Traced(" << v_ << ")\n"; }
    int value() const { return v_; }
private:
    int v_;
};

// 重载分辨：左值走拷贝版、右值（含 move 过的）走移动版
void sink(Traced& t)  { std::cout << "  [sink&]  左值版收到 " << t.value() << "\n"; }
void sink(Traced&& t) { std::cout << "  [sink&&] 右值版收到 " << t.value() << "\n"; }

int main() {
    std::cout << "== 右值引用：能绑右值的引用 ==" << std::endl;
    Traced&& r = Traced(42);                 // && 绑临时（延长生命同 const&）
    std::cout << "  Traced&& r = Traced(42); r.value() = " << r.value() << std::endl;
    // 注意：r 本身是左值（有名字）！用 r 传参会走左值版——除非再 move
    sink(r);                                  // 左值版！（named rvalue 是左值——经典反直觉）
    sink(std::move(r));                       // 右值版（move 重新贴标签）

    std::cout << "\n== std::move 不移动任何东西（计划指定验证） ==" << std::endl;
    Traced a(7);
    std::cout << "语句：Traced b = a;（不 move——拷贝）" << std::endl;
    { Traced b = a; (void)b; }
    std::cout << "语句：Traced c = std::move(a);（move 后——移动）" << std::endl;
    { Traced c = std::move(a); (void)c; }
    std::cout << "  （move 本身零日志——它只是把 a '标记'成右值）" << std::endl;
    std::cout << "  （真正的移动发生在移动构造函数里）" << std::endl;
    std::cout << "  a 还活着（值可能被掏空）——析构照常：" << std::endl;

    std::cout << "\n== move 后对象的状态（诚实检查） ==" << std::endl;
    std::string s1 = "重要数据重要数据重要数据";
    std::string s2 = std::move(s1);
    std::cout << "  s2 = \"" << s2 << "\"（资源到手）" << std::endl;
    std::cout << "  s1.size() = " << s1.size()
              << "（被掏空——但对象'有效但未指定'状态）" << std::endl;
    std::cout << "  （可以赋新值复活："; s1 = "复活"; std::cout << "s1 = \"" << s1 << "\"）" << std::endl;

    std::cout << "\n== move 的正确场景：要丢弃源时 ==" << std::endl;
    std::vector<std::string> v;
    std::string tmp = "一批数据";
    v.push_back(tmp);                         // 左值——拷贝进容器
    std::cout << "  push_back(tmp)：拷贝（tmp 还要用）" << std::endl;
    std::string tmp2 = "另一批";
    v.push_back(std::move(tmp2));             // 右值——移进容器
    std::cout << "  push_back(move(tmp2))：移动（tmp2 不要了）" << std::endl;
    std::cout << "  容量 " << v.size() << " 条；tmp2 = \"" << tmp2 << "\"（空了）" << std::endl;

    std::cout << "\n== move 的误用警示 ==" << std::endl;
    std::cout << "  const 对象 move 无效：move(const T) → 还是走拷贝！" << std::endl;
    const std::string cs = "常量";
    auto&& moved = std::move(cs);             // 类型是 const&&——不能绑移动构造
    (void)moved;
    std::cout << "  （const&& 找不到移动构造——静默退回拷贝——坑！）" << std::endl;
    std::cout << "  还有：move 后继续用源 = 未指定行为（除了赋新值/析构）" << std::endl;
    return 0;
}
