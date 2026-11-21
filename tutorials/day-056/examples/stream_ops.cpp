#include <iostream>
#include <sstream>
#include <string>

// 流运算符 << >>：输出侧链式 + 输入侧格式检查
class Fraction {
private:
    int num_, den_;

    int gcd(int a, int b) const {
        while (b != 0) { int t = a % b; a = b; b = t; }
        return a;
    }
    void reduce() {
        if (den_ < 0) { num_ = -num_; den_ = -den_; }
        int g = gcd(num_ < 0 ? -num_ : num_, den_);
        if (g > 1) { num_ /= g; den_ /= g; }
    }

public:
    Fraction(int n = 0, int d = 1) : num_(n), den_(d) {
        if (d == 0) { den_ = 1; }
        reduce();
    }

    // 输出运算符：友元非成员版（左边是 ostream）
    friend std::ostream& operator<<(std::ostream& os, const Fraction& f) {
        os << f.num_ << "/" << f.den_;
        return os;                             // 返回流引用——链式的命脉
    }

    // 输入运算符：istream 版（格式检查是重点）
    friend std::istream& operator>>(std::istream& is, Fraction& f) {
        int n, d;
        char slash;
        is >> n >> slash >> d;                 // 期望格式："1/2"（数字/数字）
        if (!is) {                             // 流状态坏了（非数字输入等）
            std::cout << "  [输入格式错误：流已置错]" << std::endl;
            return is;
        }
        if (slash != '/') {                    // 分隔符不对
            std::cout << "  [格式错误：期望 / 却读到 '" << slash << "']" << std::endl;
            return is;
        }
        f = Fraction(n, d);                    // 走构造函数：约分 + 分母检查
        return is;
    }
};

int main() {
    std::cout << "== 输出侧链式 ==" << std::endl;
    Fraction a(1, 2), b(3, 4);
    std::cout << "a=" << a << " b=" << b << "（一句连打）" << std::endl;
    // 链式机制：operator<<(operator<<(cout, a), b)——里层的返回值成为外层的左操作数

    std::cout << "\n== 输入侧（正常格式） ==" << std::endl;
    Fraction c;
    std::istringstream good("5/10");
    good >> c;
    std::cout << "读到 c = " << c << "（5/10 约分成 1/2）" << std::endl;

    std::cout << "\n== 输入侧（格式错误） ==" << std::endl;
    Fraction d;
    std::istringstream bad("hello");           // 非数字
    bad >> d;
    std::cout << "失败后 d = " << d << "（保持默认值 0/1）" << std::endl;

    Fraction e;
    std::istringstream bad2("3x4");            // 分隔符不是 /
    bad2 >> e;
    std::cout << "失败后 e = " << e << std::endl;
    return 0;
}
