#include <iostream>

// 输入校验：cin 读取失败时会发生什么
int main() {
    int age = 0;
    std::cout << "请输入你的年龄： ";
    if (!(std::cin >> age)) {                 // 读取失败：进入失败状态
        std::cout << "输入的不是整数，读取失败！" << std::endl;
        return 1;
    }
    std::cout << "收到，年龄 = " << age << std::endl;
    return 0;
}
