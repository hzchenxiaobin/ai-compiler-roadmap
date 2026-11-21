#include <iostream>
#include <string>

// =default 与 =delete：精确控制编译器生成的特殊函数
class Widget {
private:
    std::string name_;
    int id_;

public:
    Widget(const std::string& n, int i) : name_(n), id_(i) {}

    // =default：让编译器生成默认版（比手写空体更清晰地表达"要默认"）
    Widget() = default;                                    // 默认构造
    Widget(const Widget&) = default;                       // 拷贝构造（默认=浅拷贝，string 自带深拷贝所以安全）
    Widget& operator=(const Widget&) = default;            // 拷贝赋值
    ~Widget() = default;                                   // 析构

    void print() const { std::cout << "  " << name_ << " #" << id_ << std::endl; }
};

class NoCopy {
private:
    std::string secret_;

public:
    explicit NoCopy(const std::string& s) : secret_(s) {}

    NoCopy(const NoCopy&) = delete;                        // ②禁止拷贝构造
    NoCopy& operator=(const NoCopy&) = delete;             // ③禁止拷贝赋值
    NoCopy(NoCopy&&) = default;                            // 移动仍允许（Day 111 预告）
    NoCopy& operator=(NoCopy&&) = default;

    ~NoCopy() = default;
    void print() const { std::cout << "  secret!" << std::endl; }
};

int main() {
    std::cout << "== Widget（=default 全家桶） ==" << std::endl;
    Widget w1("普通", 1);
    Widget w2 = w1;               // 默认拷贝构造（成员各自拷贝）
    w2.print();

    std::cout << "\n== NoCopy（=delete 禁拷贝） ==" << std::endl;
    NoCopy n1("机密");
    n1.print();
    // NoCopy n2 = n1;            // 取消注释：编译错误！（拷贝构造被 delete）
    // NoCopy n3; n3 = n1;        // 取消注释：编译错误！（拷贝赋值被 delete）
    std::cout << "  （两行注释的取消注释版会编译失败——见 5.4 实测）" << std::endl;
    return 0;
}
