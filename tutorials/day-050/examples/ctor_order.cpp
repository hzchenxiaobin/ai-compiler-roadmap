#include <iostream>
#include <string>

// 构造时机观察：带日志的类，数一数谁出生了几次
class Logger {
private:
    std::string tag_;

public:
    explicit Logger(const std::string& tag) : tag_(tag) {
        std::cout << "  【出生】" << tag_ << std::endl;
    }

    ~Logger() {                                     // 析构函数：对象销毁时自动调用（Day 51 主角，先混个脸熟）
        std::cout << "  【销毁】" << tag_ << std::endl;
    }
};

int main() {
    std::cout << "进入 main" << std::endl;
    {
        Logger a("栈对象a");
        Logger b("栈对象b");
        std::cout << "  块内工作……" << std::endl;
    }                                               // 出块：b、a 逆序销毁
    std::cout << "回到 main" << std::endl;

    Logger arr[2] = {Logger("数组[0]"), Logger("数组[1]")};
    std::cout << "main 结束前" << std::endl;
    return 0;                                       // arr 的两个元素在此销毁
}
