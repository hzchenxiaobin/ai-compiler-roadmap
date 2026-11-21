#include <iostream>
#include <string>

// 拷贝构造：对象复制时发生了什么（打日志观察触发时机）
class Logger {
private:
    std::string tag_;

public:
    explicit Logger(const std::string& tag) : tag_(tag) {
        std::cout << "  [构造] " << tag_ << std::endl;
    }

    Logger(const Logger& other) : tag_(other.tag_ + " 的拷贝") {   // 拷贝构造
        std::cout << "  [拷贝构造] 从 " << other.tag_ << " 复制出新对象" << std::endl;
    }

    ~Logger() {
        std::cout << "  [析构] " << tag_ << std::endl;
    }
};

// 值传递：参数是复印件（Day 9 的函数版验证）
void take_by_value(Logger x) {
    std::cout << "  函数内使用 " << "……" << std::endl;
}

// 引用传递：不拷贝
void take_by_ref(const Logger& x) {
    std::cout << "  函数内使用（引用，无拷贝）" << std::endl;
}

Logger make_logger() {
    Logger local("局部对象");
    return local;                    // 返回：现代编译器通常省略拷贝（RVO）
}

int main() {
    std::cout << "== ① 用另一个对象初始化 ==" << std::endl;
    Logger a("原始");
    Logger b = a;                    // 拷贝构造触发点 1：初始化

    std::cout << "\n== ② 值传递进函数 ==" << std::endl;
    take_by_value(a);                // 拷贝构造触发点 2：参数复印

    std::cout << "\n== ③ 引用传递（不拷贝） ==" << std::endl;
    take_by_ref(a);                  // 无拷贝

    std::cout << "\n== ④ 函数返回对象 ==" << std::endl;
    Logger c = make_logger();        // RVO：通常零拷贝（本机实测确认）

    std::cout << "\n== ⑤ 赋值（不是拷贝构造！） ==" << std::endl;
    Logger d("目标");
    d = a;                           // 这是拷贝赋值运算符（Day 62），不是拷贝构造
    std::cout << "  （赋值不打拷贝构造日志——两者不同）" << std::endl;
    return 0;
}
