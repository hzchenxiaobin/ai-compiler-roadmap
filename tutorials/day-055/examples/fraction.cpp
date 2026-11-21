#include <iostream>

// 运算符重载：Fraction 分数类（计划指定练习）
class Fraction {
private:
    int num_;    // 分子（numerator）
    int den_;    // 分母（denominator）

    int gcd(int a, int b) const {               // 辗转相除（Day 33 的老朋友）
        while (b != 0) { int t = a % b; a = b; b = t; }
        return a;
    }

    void reduce() {                             // 约分：保持不变量"最简分数"
        if (den_ < 0) { num_ = -num_; den_ = -den_; }   // 分母恒正
        int g = gcd(num_ < 0 ? -num_ : num_, den_);
        if (g > 1) { num_ /= g; den_ /= g; }
    }

public:
    Fraction(int num, int den) : num_(num), den_(den) {
        if (den == 0) {                         // 不变量：分母非零
            std::cout << "  错误：分母为零！换成分母 1" << std::endl;
            den_ = 1;
        }
        reduce();
    }

    // 算术运算符：成员函数版
    Fraction operator+(const Fraction& rhs) const {
        return Fraction(num_ * rhs.den_ + rhs.num_ * den_, den_ * rhs.den_);
    }

    Fraction operator-(const Fraction& rhs) const {
        return Fraction(num_ * rhs.den_ - rhs.num_ * den_, den_ * rhs.den_);
    }

    Fraction operator*(const Fraction& rhs) const {
        return Fraction(num_ * rhs.num_, den_ * rhs.den_);
    }

    Fraction operator/(const Fraction& rhs) const {
        if (rhs.num_ == 0) {
            std::cout << "  错误：除以零！返回 0/1" << std::endl;
            return Fraction(0, 1);
        }
        return Fraction(num_ * rhs.den_, den_ * rhs.num_);
    }

    // 比较运算符
    bool operator==(const Fraction& rhs) const {
        return num_ == rhs.num_ && den_ == rhs.den_;     // 最简形式下可直接比
    }

    // 输出运算符：友元版（要访问私有成员 + 左操作数是 ostream）
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        os << f.num_ << "/" << f.den_;
        return os;                              // 返回流支持链式（Day 37 的 this 思想）
    }

    double to_double() const { return static_cast<double>(num_) / den_; }
};

int main() {
    Fraction a(1, 2), b(3, 4);

    std::cout << "a = " << a << "，b = " << b << std::endl;   // << 重载

    std::cout << "a + b = " << (a + b) << std::endl;
    std::cout << "a - b = " << (a - b) << std::endl;
    std::cout << "a * b = " << (a * b) << std::endl;
    std::cout << "a / b = " << (a / b) << std::endl;

    Fraction c(2, 4);                            // 构造时自动约分
    std::cout << "c(2,4) = " << c << "（自动约分）" << std::endl;
    std::cout << "a == c ? " << (a == c ? "相等" : "不等") << std::endl;
    std::cout << "a 的浮点值 = " << a.to_double() << std::endl;

    Fraction bad(1, 0);                          // 分母为零被拦
    (void)(a / Fraction(0, 1));                 
    return 0;
}
