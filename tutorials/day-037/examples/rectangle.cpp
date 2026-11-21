#include <iostream>

// 第 37 天主角：Rectangle 类——数据和行为打包，数据藏起来
class Rectangle {
private:                        // 私有：外界看不见（默认就是 private）
    double width_;
    double height_;

public:                         // 公有：外界能用的接口
    void set_size(double w, double h) {
        width_ = w;             // 成员函数可以直接摸私有数据
        height_ = h;
    }

    double area() const {       // const：承诺不修改成员（Day 22 的锁）
        return width_ * height_;
    }

    double perimeter() const {
        return 2 * (width_ + height_);
    }
};

int main() {
    Rectangle r;                // 类类型声明对象（就像 int x 一样自然）
    r.set_size(3, 4);           // 用接口设置尺寸
    std::cout << "3×4 矩形：面积 = " << r.area()
              << "，周长 = " << r.perimeter() << std::endl;

    r.set_size(2, 5);
    std::cout << "2×5 矩形：面积 = " << r.area()
              << "，周长 = " << r.perimeter() << std::endl;
    return 0;
}
