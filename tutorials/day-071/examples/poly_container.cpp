#include <cmath>
#include <iostream>
#include <string>
#include <memory>
#include <vector>

// Day 71：多态容器——vector<Shape*> 与 vector<unique_ptr<Shape>>
class Shape {
public:
    virtual double area() const = 0;
    virtual std::string name_impl() const = 0;
    virtual ~Shape() = default;

    std::string name() const { return name_impl(); }   // 公共包装（非虚）

protected:
    // 公共数据放在基类——派生类不用重复声明
    std::string id_;
    explicit Shape(const std::string& id) : id_(id) {}
    std::string tag() const { return id_; }             // 派生类可用的受保护工具
};

class Circle : public Shape {
    double r_;
public:
    explicit Circle(double r) : Shape("圆"), r_(r) {}
    double area() const override { return 3.14159265 * r_ * r_; }
    std::string name_impl() const override { return tag() + "(r=" + std::to_string(r_) + ")"; }
};

class Rect : public Shape {
    double w_, h_;
public:
    Rect(double w, double h) : Shape("矩形"), w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    std::string name_impl() const override { return tag() + "(" + std::to_string(w_) + "×" + std::to_string(h_) + ")"; }
};

class Triangle : public Shape {
    double a_, b_, c_;
public:
    Triangle(double a, double b, double c) : Shape("三角形"), a_(a), b_(b), c_(c) {}
    double area() const override {                       // 海伦公式
        double s = (a_ + b_ + c_) / 2;
        return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
    }
    std::string name_impl() const override { return tag() + "(" + std::to_string(a_) + "," + std::to_string(b_) + "," + std::to_string(c_) + ")"; }
};

// 方式一：裸指针容器（要手动 delete——今天的主要反面教材+对照）
void raw_pointer_demo() {
    std::cout << "== 方式一：vector<Shape*>（手动管理） ==" << std::endl;
    std::vector<Shape*> shapes;
    shapes.push_back(new Circle(2));
    shapes.push_back(new Rect(3, 4));
    shapes.push_back(new Triangle(3, 4, 5));

    double total = 0;
    for (const Shape* s : shapes) {
        std::cout << "  " << s->name() << "：面积 " << s->area() << std::endl;
        total += s->area();
    }
    std::cout << "  总面积 = " << total << std::endl;
    for (Shape* s : shapes) { delete s; }    // 忘了这行就泄漏 ×3！
    std::cout << "  （手动 delete ×3——忘了就泄漏）" << std::endl;
}

// 方式二：unique_ptr 容器（自动管理——今天的主角）
void smart_pointer_demo() {
    std::cout << "\n== 方式二：vector<unique_ptr<Shape>>（自动管理） ==" << std::endl;
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));
    shapes.push_back(std::make_unique<Rect>(3, 4));
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));

    double total = 0;
    for (const auto& s : shapes) {           // 注意：const auto&（Day 46 的形态课）
        std::cout << "  " << s->name() << "：面积 " << s->area() << std::endl;
        total += s->area();
    }
    std::cout << "  总面积 = " << total << std::endl;
    // 没有 delete——容器析构自动逐个释放！
    std::cout << "  （零 delete——容器析构自动收摊）" << std::endl;
}

int main() {
    raw_pointer_demo();
    smart_pointer_demo();
    return 0;
}
