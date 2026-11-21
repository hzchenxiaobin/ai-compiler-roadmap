#include <iostream>
#include <iomanip>
#include <string>

// 第 4 天主线例子：交互式问答——姓名 + 进度 + 自我介绍
int main() {
    std::string name;
    int days = 0;

    std::cout << "你叫什么名字？ ";
    std::cin >> name;
    std::cout << "你已经学到第几天？ ";
    std::cin >> days;

    const int total = 144;
    double percent = days * 100.0 / total;

    std::cout << std::fixed << std::setprecision(2);   // 保留 2 位小数显示
    std::cout << "你好，" << name << "！" << std::endl;
    std::cout << "第 " << days << " 天 / 共 " << total << " 天，进度 "
              << percent << " %" << std::endl;
    std::cout << "剩下的 " << total - days << " 天，我们一起走完。" << std::endl;
    return 0;
}
