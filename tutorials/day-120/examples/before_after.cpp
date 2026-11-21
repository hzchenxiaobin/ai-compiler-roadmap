#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// ===== Day 120 项目日：旧项目的裸 new/delete → 智能指针改造（计划指定）+ ASan 复查 =====
// 演示载体：Day 76/77 图形库风格的层次（多态 + 工厂）

// ---------- 改造前（裸指针时代——历史代码风格） ----------
namespace old_code {
class Shape {
public:
    virtual ~Shape() { std::cout << "  [旧·析构]\n"; }
    virtual double area() const = 0;
};
class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
private:
    double r_;
};
Shape* make_circle(double r) { return new Circle(r); }        // 裸 new

int run_old() {
    Shape* shapes[3];
    shapes[0] = make_circle(1);                               // 谁删？——裸时代的协议负担
    shapes[1] = make_circle(2);
    shapes[2] = make_circle(3);
    double total = 0;
    for (auto* s : shapes) { total += s->area(); }
    for (auto* s : shapes) { delete s; }                      // 手动删——忘了就泄漏
    return static_cast<int>(total);
}
}  // namespace old_code

// ---------- 改造后（智能指针时代——今天的目标形态） ----------
namespace new_code {
class Shape {
public:
    virtual ~Shape() = default;                                // 虚析构照留
    virtual double area() const = 0;
};
class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    double area() const override { return 3.14159 * r_ * r_; }
private:
    double r_;
};
std::unique_ptr<Shape> make_circle(double r) {                 // 工厂返回独占
    return std::make_unique<Circle>(r);
}

int run_new() {
    std::vector<std::unique_ptr<Shape>> shapes;                // 容器持有所有权
    shapes.push_back(make_circle(1));
    shapes.push_back(make_circle(2));
    shapes.push_back(make_circle(3));
    double total = 0;
    for (const auto& s : shapes) { total += s->area(); }       // 遍历照常
    return static_cast<int>(total);                            // 出作用域自动全部析构
}
}  // namespace new_code

int main() {
    std::cout << "== 改造前（裸指针时代） ==" << std::endl;
    int t_old = old_code::run_old();
    std::cout << "  旧版总面积 = " << t_old << "（手动 delete 三次——看上面的 [旧·析构]）" << std::endl;

    std::cout << "\n== 改造后（智能指针时代） ==" << std::endl;
    int t_new = new_code::run_new();
    std::cout << "  新版总面积 = " << t_new << "（零 delete——出作用域全自动）" << std::endl;

    std::cout << "\n== 改造清单（差异逐行对照） ==" << std::endl;
    std::cout << "  Shape* shapes[3];        → vector<unique_ptr<Shape>>" << std::endl;
    std::cout << "  return new Circle(r);    → return make_unique<Circle>(r);" << std::endl;
    std::cout << "  for … delete s;          → （删除——出作用域自动）" << std::endl;
    std::cout << "  虚析构 virtual ~Shape()  → = default（照留——多态底线）" << std::endl;

    std::cout << "\n== 复杂度对照 ==" << std::endl;
    std::cout << "  旧版：5 处内存相关代码（new×1 函数/数组/遍历删/虚析构留心）" << std::endl;
    std::cout << "  新版：3 处（make_unique/容器/自动析构）——删掉的是'协议负担'" << std::endl;
    std::cout << "  （行为一致：两版结果都是 " << (t_old == t_new ? "相同 ✔" : "不同 ✘") << "）" << std::endl;
    return 0;
}
