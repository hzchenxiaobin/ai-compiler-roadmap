#include <iostream>
#include <memory>
#include <string>
#include <vector>

// 多态容器 + 智能指针：Shape 家族的现代管理方式（Day 71 深讲，今天先看一眼）
class Shape {
protected:
    std::string name_;

public:
    explicit Shape(const std::string& name) : name_(name) {}
    virtual double area() const = 0;              // 纯虚函数：本行末尾 = 0
    virtual ~Shape() = default;
    void describe() const {
        std::cout << name_ << " 的面积 = " << area() << std::endl;
    }
};

class Circle : public Shape {
private:
    double r_;

public:
    explicit Circle(double r) : Shape("圆"), r_(r) {}
    double area() const override { return 3.14159265 * r_ * r_; }
};

class Rect : public Shape {
private:
    double w_, h_;

public:
    Rect(double w, double h) : Shape("矩形"), w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
};

class Triangle : public Shape {
private:
    double base_, height_;

public:
    Triangle(double b, double h) : Shape("三角形"), base_(b), height_(h) {}
    double area() const override { return base_ * height_ / 2; }
};

int main() {
    // 抽象类不能实例化：Shape s("x"); → 编译错误！
    std::vector<std::unique_ptr<Shape>> zoo;      // 多态容器
    zoo.push_back(std::make_unique<Circle>(2));
    zoo.push_back(std::make_unique<Rect>(3, 4));
    zoo.push_back(std::make_unique<Triangle>(6, 5));

    double total = 0;
    for (const auto& s : zoo) {
        s->describe();                            // 各自的 area 被调用
        total += s->area();
    }
    std::cout << "总面积 = " << total << std::endl;
    return 0;
}
