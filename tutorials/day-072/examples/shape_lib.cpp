#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

// ===== Day 72 项目日：图形库 v1（第 12 周总装大考） =====
// 5 种图形 + 多态计算面积/周长 + 按面积排序输出

class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual std::string name() const = 0;

    void report() const {                           // Day 70 的杠杆
        std::cout << "  " << name() << "：面积 " << area()
                  << "，周长 " << perimeter() << std::endl;
    }

    virtual ~Shape() = default;                    // Day 69 的虚析构
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

class Square : public Rect {                        // 正方形是一个矩形（is-a）
public:
    explicit Square(double side) : Rect(side, side) {}
    std::string name() const override { return "正方形(边" + std::to_string(side_value()) + ")"; }

private:
    double side_value() const { return perimeter() / 4; }   // 从周长反推（复用基类）
};

class Triangle : public Shape {
private:
    double a_, b_, c_;

public:
    Triangle(double a, double b, double c) : a_(a), b_(b), c_(c) {}
    double area() const override {                 // 海伦公式
        double s = (a_ + b_ + c_) / 2;
        return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
    }
    double perimeter() const override { return a_ + b_ + c_; }
    std::string name() const override {
        return "三角形(" + std::to_string(a_) + "," + std::to_string(b_) + "," + std::to_string(c_) + ")";
    }
};

class Ellipse : public Shape {
private:
    double a_, b_;                                 // 长短半轴

public:
    Ellipse(double a, double b) : a_(a), b_(b) {}
    double area() const override { return 3.14159265 * a_ * b_; }
    double perimeter() const override {            // Ramanujan 近似
        double h = (a_ - b_) * (a_ - b_) / ((a_ + b_) * (a_ + b_));
        return 3.14159265 * (a_ + b_) * (1 + 3 * h / (10 + std::sqrt(4 - 3 * h)));
    }
    std::string name() const override { return "椭圆(" + std::to_string(a_) + "," + std::to_string(b_) + ")"; }
};

int main() {
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));
    shapes.push_back(std::make_unique<Rect>(3, 4));
    shapes.push_back(std::make_unique<Square>(5));
    shapes.push_back(std::make_unique<Triangle>(3, 4, 5));
    shapes.push_back(std::make_unique<Ellipse>(4, 2));

    std::cout << "== 排序前（插入序） ==" << std::endl;
    for (const auto& s : shapes) {
        s->report();
    }

    std::sort(shapes.begin(), shapes.end(),         // 按面积升序
              [](const std::unique_ptr<Shape>& a, const std::unique_ptr<Shape>& b) {
                  return a->area() < b->area();
              });

    std::cout << "\n== 按面积升序后 ==" << std::endl;
    double total = 0;
    for (const auto& s : shapes) {
        s->report();
        total += s->area();
    }
    std::cout << "\n总面积 = " << total << std::endl;
    return 0;
}
