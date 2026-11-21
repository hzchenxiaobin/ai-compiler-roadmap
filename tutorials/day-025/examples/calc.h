#ifndef CALC_H                // 头文件守护开始：防止本文件被重复粘贴
#define CALC_H

// 名片区：只放声明（Day 9 的原型），机器本体在 calc.cpp
double calc(double a, char op, double b);
bool is_valid_op(char op);

#endif                        // 守护结束
