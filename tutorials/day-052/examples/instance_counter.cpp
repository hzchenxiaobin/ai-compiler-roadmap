#include <iostream>
#include <string>

// static 成员：全类共享一份（不属于任何单个对象）——实例计数器
class Widget {
private:
    std::string name_;
    static int alive_;                 // 声明：当前存活的对象数（全类一份）

public:
    explicit Widget(const std::string& name) : name_(name) {
        ++alive_;
        std::cout << "  +" << name_ << "　存活数 = " << alive_ << std::endl;
    }

    ~Widget() {
        --alive_;
        std::cout << "  -" << name_ << "　存活数 = " << alive_ << std::endl;
    }

    static int alive() {               // 静态成员函数：不需要对象就能调
        return alive_;
    }
};

int Widget::alive_ = 0;                // 定义：类外初始化（静态成员的家在类外）

int main() {
    std::cout << "开局存活数 = " << Widget::alive() << std::endl;

    Widget a("甲");
    {
        Widget b("乙");
        Widget c("丙");
        std::cout << "块内存活数 = " << Widget::alive() << std::endl;
    }                                  // c、b 逆序销毁（Day 51 的规律）
    std::cout << "出块后存活数 = " << Widget::alive() << std::endl;

    Widget d("丁");
    std::cout << "终局存活数 = " << Widget::alive() << std::endl;
    return 0;
}
