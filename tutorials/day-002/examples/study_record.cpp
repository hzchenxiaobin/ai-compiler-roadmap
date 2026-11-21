#include <iostream>

// 第 2 天主线例子：用 5 种基本类型记录一条学习档案
int main() {
    const int total_days = 144;      // 整数：学习总天数
    int today = 2;                   // 整数：今天是第几天
    double progress = 2.0 / 144.0;   // 小数：当前进度
    char grade = 'B';                // 字符：今日自评等级
    bool finished = false;           // 布尔：今天的目标是否全部完成

    std::cout << "Day " << today << " of " << total_days << std::endl;
    std::cout << "progress = " << progress << std::endl;
    std::cout << "grade = " << grade << std::endl;
    std::cout << "finished = " << finished << std::endl;
    std::cout << "sizeof(today) = " << sizeof(today) << std::endl;
    return 0;
}
