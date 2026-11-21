#include <iostream>

// 第 5 天主线例子：成绩等级判定（含非法输入处理）
int main() {
    int score = 0;
    std::cout << "请输入成绩（0~100）：";

    if (!(std::cin >> score)) {                 // 第一关：是不是整数
        std::cout << "输入的不是整数！" << std::endl;
        return 1;
    }
    if (score < 0 || score > 100) {             // 第二关：范围合法吗（|| 有一个成立就拦）
        std::cout << "成绩必须在 0~100 之间！" << std::endl;
        return 1;
    }

    if (score >= 90) {
        std::cout << "等级 A：优秀" << std::endl;
    } else if (score >= 80) {
        std::cout << "等级 B：良好" << std::endl;
    } else if (score >= 70) {
        std::cout << "等级 C：中等" << std::endl;
    } else if (score >= 60) {
        std::cout << "等级 D：及格" << std::endl;
    } else {
        std::cout << "等级 F：不及格" << std::endl;
    }
    return 0;
}
