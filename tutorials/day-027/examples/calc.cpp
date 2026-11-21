#include "calc.h"             // 自己的头文件也要包含：让编译器核对声明与定义是否一致

// 机器区：函数本体（定义）放在源文件里
double calc(double a, char op, double b) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    return a / b;
}

bool is_valid_op(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/';
}
