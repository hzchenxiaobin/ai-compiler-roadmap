#include <iostream>

// 类型转换运算符：Fraction → double 的隐式通道
class Fraction {
private:
    int num_, den_;

public:
    Fraction(int n, int d) : num_(n), den_(d) {}

    // 类型转换运算符：operator 目标类型()
    operator double() const {
        return static_cast<double>(num_) / den_;
    }
};

int main() {
    Fraction f(1, 2);

    std::cout << "== 显式转换 ==" << std::endl;
    double d = static_cast<double>(f);           // 明写转换
    std::cout << "static_cast<double>(f) = " << d << std::endl;

    std::cout << "\n== 隐式转换（看不见但发生了） ==" << std::endl;
    double d2 = f;                               // 隐式：编译器自动调 operator double
    std::cout << "double d2 = f → " << d2 << std::endl;

    double sum = f + 1.5;                        // 隐式：f 变 double 再加
    std::cout << "f + 1.5 = " << sum << std::endl;

    std::cout << "\n== 隐式转换的意外现场 ==" << std::endl;
    // 本意可能是想打印分数"1/2"，结果 f 悄悄变 0.5 打印了！
    std::cout << "cout << f 打出的是：" << f << std::endl;   // 0.5，不是 1/2！
    std::cout << "（Fraction 没有 operator<<，cout 找到 double 通道就走了）" << std::endl;
    return 0;
}
