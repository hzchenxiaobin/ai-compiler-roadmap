#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

// ===== Day 77 项目日：图形库 v3——从文件读取图形描述、多态创建、文本渲染 =====
// 复用 v2 的三层骨架（接口 → 公共基类 → 具体）
class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual ~Shape() = default;
};

class LabeledShape : public Shape {
protected:
    std::string id_;
    LabeledShape(const std::string& id) : id_(id) {}
public:
    void report() const {
        std::cout << "  " << id_ << "：面积 " << area() << "，周长 " << perimeter() << std::endl;
    }
    const std::string& id() const { return id_; }
};

class Circle : public LabeledShape {
    double r_;
public:
    explicit Circle(double r) : LabeledShape("圆(r=" + std::to_string(r) + ")"), r_(r) {}
    double area() const override { return 3.14159265 * r_ * r_; }
    double perimeter() const override { return 2 * 3.14159265 * r_; }
};

class Rect : public LabeledShape {
    double w_, h_;
public:
    Rect(double w, double h) : LabeledShape("矩形(" + std::to_string(w) + "×" + std::to_string(h) + ")"), w_(w), h_(h) {}
    double area() const override { return w_ * h_; }
    double perimeter() const override { return 2 * (w_ + h_); }
};

class Triangle : public LabeledShape {
    double a_, b_, c_;
public:
    Triangle(double a, double b, double c)
        : LabeledShape("三角形(" + std::to_string(a) + "," + std::to_string(b) + "," + std::to_string(c) + ")"),
          a_(a), b_(b), c_(c) {}
    double area() const override {
        double s = (a_ + b_ + c_) / 2;
        return std::sqrt(s * (s - a_) * (s - b_) * (s - c_));
    }
    double perimeter() const override { return a_ + b_ + c_; }
};

// ===== 工厂函数：一行描述 → 多态创建（今天的核心新增） =====
// 格式约定："circle 2"、"rect 3 4"、"triangle 3 4 5"
// 返回 LabeledShape 层（渲染要用 report——依赖需要的面，比纯 Shape 多一层）
std::unique_ptr<LabeledShape> make_shape(const std::string& line) {
    std::istringstream iss(line);
    std::string kind;
    iss >> kind;

    if (kind == "circle") {
        double r;
        if (iss >> r) { return std::make_unique<Circle>(r); }
    } else if (kind == "rect") {
        double w, h;
        if (iss >> w >> h) { return std::make_unique<Rect>(w, h); }
    } else if (kind == "triangle") {
        double a, b, c;
        if (iss >> a >> b >> c) { return std::make_unique<Triangle>(a, b, c); }
    }
    return nullptr;                        // 无法识别的行 → 调用方跳过
}

int main() {
    // 1) 测试文件：不存在才生成样例（存在则用用户的——数据驱动的关键）
    const std::string fname = "shapes.txt";
    {
        std::ifstream probe(fname);
        if (probe.good()) {
            std::cout << "  （shapes.txt 已存在——使用现有数据，不覆盖）" << std::endl;
        } else {
        std::ofstream out(fname);
        out << "circle 2\n";
        out << "rect 3 4\n";
        out << "# 注释行（应被跳过）\n";
        out << "triangle 3 4 5\n";
        out << "hexagon 6\n";              // 未知类型（应被跳过）
        out << "circle\n";                 // 缺参数（应被跳过）
        out << "rect 5 2\n";
        }
    }

    // 2) 从文件读取 + 工厂创建 + 收集
    std::vector<std::unique_ptr<LabeledShape>> shapes;
    std::ifstream in(fname);
    std::string line;
    int line_no = 0, skipped = 0;
    while (std::getline(in, line)) {
        ++line_no;
        if (line.empty() || line[0] == '#') { ++skipped; continue; }   // 空行/注释
        auto s = make_shape(line);
        if (s) {
            shapes.push_back(std::move(s));
        } else {
            ++skipped;
            std::cout << "  [跳过] 第 " << line_no << " 行：\"" << line << "\"（未知类型或缺参数）" << std::endl;
        }
    }

    std::cout << "== 文件读取结果 ==" << std::endl;
    std::cout << "  共 " << line_no << " 行，跳过 " << skipped << " 行，创建 "
              << shapes.size() << " 个图形" << std::endl;

    // 3) 文本渲染（报表 + 排序 + 总面积）
    std::cout << "\n== 渲染（LabeledShape 报表） ==" << std::endl;
    for (const auto& s : shapes) { s->report(); }

    std::sort(shapes.begin(), shapes.end(),
              [](const auto& a, const auto& b) { return a->area() < b->area(); });
    std::cout << "\n== 按面积排序后 ==" << std::endl;
    double total = 0;
    for (const auto& s : shapes) {
        s->report();
        total += s->area();
    }
    std::cout << "  总面积 = " << total << std::endl;
    return 0;
}
