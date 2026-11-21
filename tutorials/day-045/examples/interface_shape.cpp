#include <iostream>
#include <string>

// 纯虚函数深入：接口类 Shape 的完整设计（Day 44 动物园的正式版）
class Shape {
public:
    virtual double area() const = 0;               // 纯虚：必须实现
    virtual double perimeter() const = 0;          // 第二个纯虚：周长
    virtual std::string name() const = 0;          // 第三个纯虚：名字

    void report() const {                           // 非虚的公共逻辑（复用纯虚接口）
        std::cout << name() << "：面积 " << area()
                  << "，周长 " << perimeter() << std::endl;
    }

    virtual ~Shape() = default;                    // 虚析构（Day 46 专场，今天照写）
};

class Circle : public Shape {
private:
    double r_;

public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159265 * r_ * r_; }
    double perimeter() const override { return 2 * 3.14159265 * r_; }
    std::string name() const override { return "圆(r=" + std::to_string(r_) + ")"; }
};

class Rect : public Shape {
private:
    double w_, h_;

public:
    Rect(double w, double h) : w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2 * (w_ + h_); }
    std::string name() const override { return "矩形(" + std::to_string(w_) + "×" + std::to_string(h_) + ")"; }
};

int main() {
    Circle c(2);
    Rect r(3, 4);

    c.report();      // report 是基类的——但调的是派生类的实现
    r.report();

    // Shape s;                      // 抽象类：不能实例化（取消注释编译错误）
    // class Blob : public Shape {}; // 漏实现任何一个纯虚 → Blob 也是抽象类
    return 0;
}
