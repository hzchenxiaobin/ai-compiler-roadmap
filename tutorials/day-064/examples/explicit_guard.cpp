#include <iostream>
#include <string>

// explicit：把关单参构造的隐式通道
class Meters {
private:
    double value_;

public:
    explicit Meters(double v) : value_(v) {}     // explicit：不许隐式转换
    double value() const { return value_; }
};

class Width {
private:
    double value_;

public:
    Width(double v) : value_(v) {}               // 非 explicit：隐式通道开着
    double value() const { return value_; }
};

void print_meters(const Meters& m) {
    std::cout << "  " << m.value() << " m" << std::endl;
}

void print_width(const Width& w) {
    std::cout << "  " << w.value() << " 宽" << std::endl;
}

int main() {
    std::cout << "== 非 explicit 版（隐式通道开着） ==" << std::endl;
    print_width(3.14);                 // 3.14 悄悄变 Width！——一步隐式转换
    std::cout << "（print_width(3.14) 编译通过——double 暗地里走了 Width 构造）" << std::endl;

    Width w = 2.5;                     // 同样：悄悄构造
    std::cout << "Width w = 2.5 → " << w.value() << std::endl;

    std::cout << "\n== explicit 版（通道上锁） ==" << std::endl;
    // print_meters(3.14);             // ✘ 编译错误：单参构造被 explicit 锁了
    print_meters(Meters(3.14));        // ✔ 必须明写构造
    std::cout << "（必须 Meters(3.14) 明写——一步都逃不掉）" << std::endl;

    Meters m = static_cast<Meters>(2.0);   // static_cast 也可（明写）
    std::cout << "static_cast<Meters>(2.0) → " << m.value() << std::endl;
    return 0;
}
