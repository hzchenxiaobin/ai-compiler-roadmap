#include <iostream>

char grade_of(int score);   // 声明（原型）：先报名字，告诉编译器"有这台机器"

int main() {
    int s1 = 95, s2 = 75, s3 = 60, s4 = 42;
    std::cout << s1 << " 分 → 等级 " << grade_of(s1) << std::endl;
    std::cout << s2 << " 分 → 等级 " << grade_of(s2) << std::endl;
    std::cout << s3 << " 分 → 等级 " << grade_of(s3) << std::endl;
    std::cout << s4 << " 分 → 等级 " << grade_of(s4) << std::endl;
    return 0;
}

// 定义：机器的本体放在 main 后面（第 5 周拆多文件时，声明进头文件、定义进源文件）
char grade_of(int score) {
    if (score >= 90) return 'A';
    if (score >= 80) return 'B';
    if (score >= 70) return 'C';
    if (score >= 60) return 'D';
    return 'F';
}
