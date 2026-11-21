#include <iostream>

// mutable：const 成员函数里也允许改的成员（缓存的合法后门）
class Circle {
private:
    double radius_;
    mutable int area_calls_;           // mutable：计数器不算"逻辑状态"
    mutable bool cached_ok_;
    mutable double cached_area_;

public:
    explicit Circle(double r) : radius_(r), area_calls_(0), cached_ok_(false), cached_area_(0) {}

    double area() const {              // const：不改 radius_（逻辑状态）
        ++area_calls_;                 // 但 mutable 计数器可以改
        if (!cached_ok_) {
            cached_area_ = 3.14159265 * radius_ * radius_;
            cached_ok_ = true;
            std::cout << "  （首次计算并缓存）" << std::endl;
        }
        return cached_area_;
    }

    int area_calls() const { return area_calls_; }
};

int main() {
    const Circle c(2);                 // const 对象——area 必须是 const 成员函数

    std::cout << "第一次 area() = " << c.area() << std::endl;
    std::cout << "第二次 area() = " << c.area() << std::endl;
    std::cout << "第三次 area() = " << c.area() << std::endl;
    std::cout << "area 被调了 " << c.area_calls() << " 次，但只算了一遍" << std::endl;
    return 0;
}
