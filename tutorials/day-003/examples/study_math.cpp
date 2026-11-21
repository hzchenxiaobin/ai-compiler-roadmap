#include <iostream>

// 第 3 天主线例子：学习计划里的数学报表
int main() {
    const int total_days = 144;
    int today = 3;
    int remaining = total_days - today;
    int weeks_left = remaining / 7;               // 整数除法：还剩几个完整周
    int leftover_days = remaining % 7;            // 取余：零头几天
    double percent = today * 100.0 / total_days;  // 先乘 100.0，让除法变浮点

    std::cout << "remaining = " << remaining << " 天" << std::endl;
    std::cout << "weeks_left = " << weeks_left << " 周 + " << leftover_days << " 天" << std::endl;
    std::cout << "percent = " << percent << " %" << std::endl;
    return 0;
}
