#include <iostream>

// 初始化列表 vs 构造体内赋值：const 成员只认前者
class WithConst {
private:
    const int id_;          // const 成员：出生后不许改
    int value_;

public:
    // 正确姿势：初始化列表——成员"出生即带值"
    WithConst(int i, int v) : id_(i), value_(v) {}

    // 错误姿势（注释着）：构造体内赋值——id_ 已经出生为"某值"，再赋值 = 改 const
    // WithConst(int i, int v) { id_ = i; value_ = v; }   // 取消注释：编译错误！

    void print() const {
        std::cout << "id = " << id_ << "，value = " << value_ << std::endl;
    }
};

int main() {
    WithConst obj(7, 100);
    obj.print();
    return 0;
}
