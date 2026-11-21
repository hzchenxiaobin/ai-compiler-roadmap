#include <iostream>
#include <string>

// 虚函数与动态多态：同一个调用，三种行为（Day 67 被拦的格子今天解锁）
class Shape {
protected:
    std::string name_;

public:
    explicit Shape(const std::string& name) : name_(name) {}

    virtual double area() const {          // virtual：派生类可以覆盖
        return 0.0;                        // 基类给个保底（无面积）
    }

    void describe() const {                // 非虚：公共逻辑（不覆盖）
        std::cout << name_ << " 的面积 = " << area() << std::endl;
    }

    virtual ~Shape() = default;            // 虚析构（Day 69 正式讲，今天先照写）
};

class Circle : public Shape {
private:
    double radius_;

public:
    explicit Circle(double r) : Shape("圆"), radius_(r) {}

    double area() const override {         // override：覆盖基类版本
        return 3.14159265 * radius_ * radius_;
    }
};

class Rect : public Shape {
private:
    double w_, h_;

public:
    Rect(double w, double h) : Shape("矩形"), w_(w), h_(h) {}

    double area() const override {
        return w_ * h_;
    }
};

int main() {
    Circle c(2);
    Rect r(3, 4);
    Shape s("无名形状");

    std::cout << "== 各自直调（编译期绑定） ==" << std::endl;
    std::cout << "圆面积 = " << c.area() << std::endl;
    std::cout << "矩形面积 = " << r.area() << std::endl;

    std::cout << "\n== 基类指针 + describe（运行期分发） ==" << std::endl;
    Shape* shapes[] = {&c, &r, &s};
    for (int i = 0; i < 3; ++i) {
        shapes[i]->describe();             // 同一个调用，三种行为！
    }
    return 0;
}
