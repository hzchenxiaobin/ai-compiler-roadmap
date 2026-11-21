#include <iostream>

// 故意的二义性：两个候选都要求"整型转浮点"，编译器无法裁决
void f(float x) {
    std::cout << "f(float)" << std::endl;
}

void f(double x) {
    std::cout << "f(double)" << std::endl;
}

int main() {
    f(3);      // int → float 和 int → double 都是"转换"，不分胜负
    return 0;
}
