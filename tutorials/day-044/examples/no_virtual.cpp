#include <iostream>
#include <string>

// 对比实验：去掉 virtual 会怎样？（静态绑定 vs 动态绑定）
class Base {
public:
    std::string who() const { return "基类版本"; }
};

class Derived : public Base {
public:
    std::string who() const { return "派生类版本"; }   // 同名但没 virtual
};

class VBase {
public:
    virtual std::string who() const { return "虚基类版本"; }
};

class VDerived : public VBase {
public:
    std::string who() const override { return "虚派生类版本"; }
};

int main() {
    Derived d;
    Base* bp = &d;                 // 基类指针指向派生类对象

    std::cout << "对象直调：d.who() = " << d.who() << std::endl;     // 派生类版本
    std::cout << "指针调用：bp->who() = " << bp->who() << std::endl; // 基类版本！（静态绑定）

    VDerived vd;
    VBase* vbp = &vd;
    std::cout << "\n对象直调：vd.who() = " << vd.who() << std::endl;       // 虚派生类版本
    std::cout << "指针调用：vbp->who() = " << vbp->who() << std::endl;     // 虚派生类版本！（动态绑定）

    std::cout << "\n结论：没有 virtual，指针看声明类型（谁声明的调谁的）；" << std::endl;
    std::cout << "　　　有了 virtual，指针看实际对象（指谁调谁的）。" << std::endl;
    return 0;
}
