#include <algorithm>
#include <cmath>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ===== Day 76 项目日：图形库 v2——重新设计继承层次，抽出公共基类与接口 =====
// v1（Day 72）是平铺的五类；v2 分三层：接口（Shape）→ 公共基类（LabeledShape）→ 具体

// 第一层：纯接口（只定规矩——Day 70 的接口类）
class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual ~Shape() = default;
};

// 第二层：公共基类（公共设施上提——名字标签 + 杠杆打印）
class LabeledShape : public Shape {
protected:
    std::string id_;
    std::string dims_;

    LabeledShape(const std::string& id, const std::string& dims)
        : id_(id), dims_(dims) {}

public:
    // 杠杆：非虚的公共打印（吃派生类实现的 area/perimeter 的饭）
    void report() const {
        std::cout << "  " << id_ << " " << dims_
                  << "：面积 " << area() << "，周长 " << perimeter() << std::endl;
    }
};

// 第三层：多边形中间层（直边图形的公共：周长=边之和的思路演示）
class Polygon : public LabeledShape {
protected:
    Polygon(const std::string& id, const std::string& dims)
        : LabeledShape(id, dims) {}
};

// —— 具体类：每层只填自己的差异 ——
class Circle : public LabeledShape {
    double r_;
public:
    explicit Circle(double r) : LabeledShape("圆", "(r=" + std::to_string(r) + ")"), r_(r) {}
    double area() const override { return 3.14159265 * r_ * r_; }
    double perimeter() const override { return 2 * 3.14159265 * r_; }
};

class Rect : public Polygon {
    double w_, h_;
protected:
    Rect(const std::string& id, double w, double h)             // 带标签版（Square 用）
        : Polygon(id, "(" + std::to_string(w) + "×" + std::to_string(h) + ")"), w_(w), h_(h) {}
public:
    Rect(double w, double h) : Rect("矩形", w, h) {}            // 委托构造（Day 50 老朋友）

    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2 * (w_ + h_); }
};

class Square : public Rect {                     // is-a：正方形是矩形（Day 72 沿用）
public:
    explicit Square(double s) : Rect("正方形", s, s) {}         // 经带标签版自报家门
};

class Triangle : public Polygon {
    double a_, b_, c_;
public:
    Triangle(double a, double b, double c)
        : Polygon("三角形", "(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ")"),
          a_(a), b_(b), c_(c) {}
    double area() const override {               // 海伦
        double s = (a_ + b_ + c_) / 2;
        return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
    }
    double perimeter() const override { return a_ + b_ + c_; }
};

class Ellipse : public LabeledShape {
    double a_, b_;
public:
    Ellipse(double a, double b)
        : LabeledShape("椭圆", "(" + std::to_string(a) + "," + std::to_string(b) + ")"), a_(a), b_(b) {}
    double area() const override { return 3.14159265 * a_ * b_; }
    double perimeter() const override {           // Ramanujan 近似
        double h = (a_ - b_) * (a_ - b_) / ((a_ + b_) * (a_ + b_));
        return 3.14159265 * (a_ + b_) * (1 + 3 * h / (10 + std::sqrt(4 - 3 * h)));
    }
};

int main() {
    std::cout << "== v2 三层层次：接口 → 公共基类 → 具体 ==" << std::endl;
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));
    shapes.push_back(std::make_unique<Rect>(3, 4));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.push_back(std::make_unique<Ellipse>(4, 2));

    // report() 是 LabeledShape 的杠杆——经基类引用调到它（Day 71 容器姿势）
    std::vector<std::unique_ptr<LabeledShape>> labeled;
    // 接口视角：纯 Shape 只认 area/perimeter（不认识 report——接口的最小面）
    for (const auto& s : shapes) {
        std::cout << "  [接口视角] area = " << s->area() << std::endl;
    }
    std::cout << "（纯 Shape* 只认 area/perimeter——接口的最小面）" << std::endl;

    std::cout << "\n== 完整报表（LabeledShape 杠杆） ==" << std::endl;
    labeled.push_back(std::make_unique<Circle>(2));
    labeled.push_back(std::make_unique<Rect>(3, 4));
    labeled.push_back(std::make_unique<Square>(5));
    labeled.push_back(std::make_unique<Triangle>(3, 4, 5));
    labeled.push_back(std::make_unique<Ellipse>(4, 2));
    for (const auto& s : labeled) {
        s->report();
    }

    std::cout << "\n== 按面积排序（Day 72 同款） ==" << std::endl;
    std::sort(labeled.begin(), labeled.end(),
              [](const auto& a, const auto& b) { return a->area() < b->area(); });
    for (const auto& s : labeled) { s->report(); }
    double total = 0;
    for (const auto& s : labeled) { total += s->area(); }
    std::cout << "  总面积 = " << total << std::endl;
    return 0;
}
