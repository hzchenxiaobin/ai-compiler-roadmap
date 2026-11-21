#include <iostream>
#include <memory>
#include <vector>

// Day 73：虚表机制的目击实验——一次虚函数调用的四步走
class Base {
public:
    virtual void who() const { std::cout << "  Base::who（基类版本）" << std::endl; }
    virtual void extra() const { std::cout << "  Base::extra" << std::endl; }
    virtual ~Base() = default;
};

class Derived : public Base {
public:
    void who() const override { std::cout << "  Derived::who（派生类覆盖版）" << std::endl; }
    void extra() const override { std::cout << "  Derived::extra" << std::endl; }
};

// 目击一：同类的对象共享虚表（vptr 相同）
void witness_shared_vtable() {
    std::cout << "== 目击一：同类对象共享虚表 ==" << std::endl;
    Derived d1, d2, d3;
    // vptr 在对象的头 8 字节（常见布局）——读出来对比
    auto vptr_of = [](const void* obj) {
        return *reinterpret_cast<const void* const*>(obj);
    };
    std::cout << "  d1 的 vptr = " << vptr_of(&d1) << std::endl;
    std::cout << "  d2 的 vptr = " << vptr_of(&d2) << std::endl;
    std::cout << "  d3 的 vptr = " << vptr_of(&d3) << std::endl;
    std::cout << "  （三个地址相同——全类一张虚表！每个对象只带一个指针）" << std::endl;

    Base b;
    std::cout << "  b  的 vptr = " << vptr_of(&b) << std::endl;
    std::cout << "  （与 Derived 的不同——Base 有自己的虚表）" << std::endl;
}

// 目击二：虚调用与非虚调用的行为差
void witness_dispatch() {
    std::cout << "\n== 目击二：一次虚调用的分发 ==" << std::endl;
    Derived d;
    Base* p = &d;

    p->who();        // 虚调用：走虚表 → Derived::who
    p->extra();      // 虚调用：走虚表 → Derived::extra
    d.who();         // 对象直调：编译期可知（结果相同但路径不同）
}

// 目击三：虚表里到底有什么（探针——把前两个函数指针打出来）
using WhoFn = void(*)(const Base*);     // 简化的函数指针类型（成员函数指针有 this 调整，教学探针版）

void witness_table_dump() {
    std::cout << "\n== 目击三：虚表槽位探针 ==" << std::endl;
    Derived d;
    Base* p = &d;

    void* const* vptr = *reinterpret_cast<void* const**>(p);   // 对象头 → 虚表
    std::cout << "  虚表地址 = " << vptr << std::endl;
    std::cout << "  槽位[0] = " << vptr[0] << "（who 的入口——Itanium ABI 下带 this 偏移，教学仅看地址）" << std::endl;
    std::cout << "  槽位[1] = " << vptr[1] << "（extra 的入口）" << std::endl;
    std::cout << "  槽位[2] = " << vptr[2] << "（析构等后续槽）" << std::endl;
    std::cout << "  （槽位连续排列——'表'是真的表）" << std::endl;
}

int main() {
    witness_shared_vtable();
    witness_dispatch();
    witness_table_dump();

    std::cout << "\n== 四步走总结 ==" << std::endl;
    std::cout << "  ①拿对象的 vptr　②到虚表　③按声明的槽位取函数地址　④跳转执行" << std::endl;
    return 0;
}
