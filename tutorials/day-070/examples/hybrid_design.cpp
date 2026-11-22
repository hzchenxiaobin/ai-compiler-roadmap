#include <iostream>
#include <memory>
#include <vector>

// Shape 抽象体系完善：纯虚 + 提供默认实现的混合设计
class Shape {
public:
    virtual double area() const = 0;
    virtual std::string name() const = 0;

    // 进阶特性：纯虚函数也可以有函数体（本文件未用到）——派生类可用 Shape::describe() 显式调用基类版
    virtual std::string describe() const = 0;

    virtual ~Shape() = default;
};

// 中间层：给 describe 一个通用默认实现（模板方法模式的雏形）
class NamedShape : public Shape {
private:
    std::string label_;

public:
    explicit NamedShape(const std::string& label) : label_(label) {}

    std::string name() const override final {     // final：不许再覆盖（Day 74 讲）
        return label_;
    }

    std::string describe() const override {        // 通用实现：复用纯虚接口
        return name() + "（面积 " + std::to_string(area()) + "）";
    }
};

class Square : public NamedShape {
private:
    double side_;

public:
    explicit Square(double s) : NamedShape("正方形"), side_(s) {}
    double area() const override { return side_ * side_; }
};

class Circle : public NamedShape {
private:
    double r_;

public:
    explicit Circle(double r) : NamedShape("圆"), r_(r) {}
    double area() const override { return 3.14159265 * r_ * r_; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Circle>(3));

    for (const auto& s : shapes) {
        std::cout << s->describe() << std::endl;   // 全部走 NamedShape 的通用实现
    }
    return 0;
}
