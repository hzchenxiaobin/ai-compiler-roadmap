#include <iostream>

// 等级字母 → 评语（故意演示漏写 break 的事故）
int main() {
    char grade = 'B';
    switch (grade) {
        case 'A':
            std::cout << "优秀！保持节奏" << std::endl;
            break;
        case 'B':
            std::cout << "良好！稳步前进" << std::endl;   // 这里故意漏了 break
        case 'C':
            std::cout << "中等！加把劲" << std::endl;
            break;
        case 'D':
        case 'F':
            std::cout << "需要复盘薄弱环节" << std::endl;
            break;
        default:
            std::cout << "未知等级" << std::endl;
    }
    return 0;
}
