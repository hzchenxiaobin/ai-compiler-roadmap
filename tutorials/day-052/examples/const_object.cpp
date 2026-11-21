#include <iostream>

// const 对象只能调 const 成员函数（Day 22 的锁升级到对象级）
class Rectangle {
private:
    double width_;
    double height_;

public:
    Rectangle(double w, double h) : width_(w), height_(h) {}

    double area() const {              // const 成员函数：承诺不改成员
        return width_ * height_;
    }

    void scale(double k) {             // 非 const：会改成员
        width_ *= k;
        height_ *= k;
    }

    double width() const { return width_; }    // 只读访问器天然是 const
};

int main() {
    Rectangle normal(3, 4);
    const Rectangle frozen(10, 20);    // const 对象：整体只读（"冻结"）

    std::cout << "normal 面积 = " << normal.area() << std::endl;
    std::cout << "frozen 面积 = " << frozen.area() << std::endl;   // ✔ const 函数可调

    normal.scale(2);
    std::cout << "normal 放大后宽 = " << normal.width() << std::endl;

    // frozen.scale(2);                // ✘ 取消注释：编译错误！const 对象不能调非 const
    std::cout << "frozen 宽仍是 " << frozen.width() << std::endl;
    return 0;
}
