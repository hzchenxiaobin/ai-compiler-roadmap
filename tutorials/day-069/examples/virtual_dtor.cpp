#include <iostream>
#include <string>

// 虚析构的主角实验：delete 基类指针时，派生类析构执行不执行？
class Base {
public:
    Base() { std::cout << "  [Base 构造]" << std::endl; }
    ~Base() { std::cout << "  [Base 析构]" << std::endl; }        // ← 没标 virtual
};

class Derived : public Base {
public:
    Derived() { std::cout << "  [Derived 构造]" << std::endl; }
    ~Derived() { std::cout << "  [Derived 析构]" << std::endl; }
};

class VBase {
public:
    VBase() { std::cout << "  [VBase 构造]" << std::endl; }
    virtual ~VBase() { std::cout << "  [VBase 析构]" << std::endl; }  // ← 标了 virtual
};

class VDerived : public VBase {
public:
    VDerived() { std::cout << "  [VDerived 构造]" << std::endl; }
    ~VDerived() { std::cout << "  [VDerived 析构]" << std::endl; }
};

int main() {
    std::cout << "场景一：非虚析构 + delete 基类指针" << std::endl;
    {
        Base* p = new Derived();      // 基类指针指向堆上的派生对象
        delete p;                     // 只调了 ~Base！~Derived 被跳过
    }

    std::cout << "\n场景二：虚析构 + delete 基类指针" << std::endl;
    {
        VBase* vp = new VDerived();
        delete vp;                    // 两个析构都执行（逆序）
    }
    return 0;
}
