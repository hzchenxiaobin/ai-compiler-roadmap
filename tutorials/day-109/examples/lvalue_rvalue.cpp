#include <iostream>
#include <string>
#include <utility>
#include <vector>

// Day 109：左值/右值/将亡值辨析——写日志观察临时对象（计划指定）
class Traced {
public:
    Traced() { std::cout << "  [构造] 默认" << std::endl; }
    Traced(int v) : v_(v) { std::cout << "  [构造] Traced(" << v << ")" << std::endl; }
    Traced(const Traced& o) : v_(o.v_) { std::cout << "  [拷贝] Traced(" << o.v_ << ") → 新对象" << std::endl; }
    Traced(Traced&& o) noexcept : v_(o.v_) { std::cout << "  [移动] Traced(" << o.v_ << ") → 新对象（偷资源）" << std::endl; }
    ~Traced() { std::cout << "  [析构] ~Traced(" << v_ << ")" << std::endl; }
    int value() const { return v_; }
private:
    int v_;
};

Traced make_traced(int v) {
    return Traced(v);                                  // 返回临时（右值）
}

void take_by_value(Traced t) {                         // 按值收——拷贝或移动
    std::cout << "  [函数] take_by_value 收到 " << t.value() << std::endl;
}

int main() {
    std::cout << "== 左值 vs 右值：谁能取地址 ==" << std::endl;
    int a = 10;
    int* pa = &a;                                      // a 是左值——有地址
    std::cout << "  int a = 10; &a ✔（左值：可取地址、有名字）" << std::endl;
    // int* pb = &(10);   // ✘ 字面量 10 是右值——没地址
    std::cout << "  &(10) ✘（右值：字面量/临时——没地址）" << std::endl;
    std::cout << "  a + 1 也是右值（表达式结果——临时的）" << std::endl;
    (void)pa;

    std::cout << "\n== 计划指定：写日志观察临时对象 ==" << std::endl;
    std::cout << "语句：Traced t1(1);" << std::endl;
    Traced t1(1);
    std::cout << "语句：Traced t2 = t1;（拷贝初始化——左值源）" << std::endl;
    { Traced t2 = t1; (void)t2; }
    std::cout << "语句：Traced t3 = Traced(3);（右值源——直接构造）" << std::endl;
    { Traced t3 = Traced(3); (void)t3; }

    std::cout << "\n== 函数返回的临时 ==" << std::endl;
    std::cout << "语句：Traced t4 = make_traced(4);" << std::endl;
    { Traced t4 = make_traced(4); (void)t4; }
    std::cout << "  （返回的临时被'接收'——现代编译器直接在 t4 的位置构造）" << std::endl;

    std::cout << "\n== 临时对象的生死线（完整语句） ==" << std::endl;
    std::cout << "语句：take_by_value(Traced(5));" << std::endl;
    take_by_value(Traced(5));
    std::cout << "  （临时构造 → 进函数（移动/拷贝）→ 函数结束析构 → 临时析构——语句末统一回收）" << std::endl;

    std::cout << "\n== 用完临时就死的经典坑 ==" << std::endl;
    const std::string& ref = std::string("临时串");
    std::cout << "  const& 延长临时生命：" << ref << "（引用绑定临时——活到引用作用域）" << std::endl;
    // std::string& bad = std::string("x");  // ✘ 非 const 引用不能绑右值
    std::cout << "  （非 const 引用绑右值 ✘——const& 可以并延长生命）" << std::endl;

    std::cout << "\n== 三分法速览（左值/纯右值/将亡值） ==" << std::endl;
    std::cout << "  左值（lvalue）：有名字有地址（a、t1）" << std::endl;
    std::cout << "  纯右值（prvalue）：字面量/临时（10、Traced(3)、a+1）" << std::endl;
    std::cout << "  将亡值（xvalue）：可被移动的'临终'表达式（std::move(a)）" << std::endl;
    std::cout << "  （后两者合称右值——能绑 && 引用）" << std::endl;
    return 0;
}
