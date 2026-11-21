#include <iostream>

// 成员版 vs 非成员版：operator+ 的两种写法对照
class Vec2 {
private:
    double x_, y_;

public:
    Vec2(double x, double y) : x_(x), y_(y) {}

    double x() const { return x_; }
    double y() const { return y_; }

    // 成员版：左操作数必须是 Vec2 本类
    Vec2 operator+(const Vec2& rhs) const {
        return Vec2(x_ + rhs.x_, y_ + rhs.y_);
    }

    // 成员版 operator* 只接受右操作数为 double：
    Vec2 operator*(double k) const {
        return Vec2(x_ * k, y_ * k);
    }
};

// 非成员版：左操作数可以是任何类型——这是它的独门优势
Vec2 operator*(double k, const Vec2& v) {         // 2 * v 走这里
    return Vec2(k * v.x(), k * v.y());
}

std::ostream& operator<<(std::ostream& os, const Vec2& v) {
    os << "(" << v.x() << ", " << v.y() << ")";
    return os;
}

int main() {
    Vec2 v(1, 2);

    std::cout << "v + v = " << (v + v) << std::endl;
    std::cout << "v * 3 = " << (v * 3) << std::endl;    // 成员版：Vec2 * double ✔
    std::cout << "3 * v = " << (3 * v) << std::endl;    // 非成员版：double * Vec2 ✔

    // 若只有成员版 operator*：3 * v 编译错误！（成员版要求左边是 Vec2）
    std::cout << "\n（对称场景要非成员版——交换律的代码化）" << std::endl;
    return 0;
}
