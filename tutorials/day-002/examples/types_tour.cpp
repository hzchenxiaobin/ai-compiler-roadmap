#include <iostream>
#include <climits>
#include <cfloat>

// 打印本机各类型的大小与极值
int main() {
    std::cout << "== 各类型的大小（字节） ==" << std::endl;
    std::cout << "char:        " << sizeof(char) << std::endl;
    std::cout << "bool:        " << sizeof(bool) << std::endl;
    std::cout << "short:       " << sizeof(short) << std::endl;
    std::cout << "int:         " << sizeof(int) << std::endl;
    std::cout << "long:        " << sizeof(long) << std::endl;
    std::cout << "long long:   " << sizeof(long long) << std::endl;
    std::cout << "float:       " << sizeof(float) << std::endl;
    std::cout << "double:      " << sizeof(double) << std::endl;
    std::cout << "long double: " << sizeof(long double) << std::endl;

    std::cout << std::endl << "== 整数类型的极值 ==" << std::endl;
    std::cout << "INT_MIN  = " << INT_MIN << std::endl;
    std::cout << "INT_MAX  = " << INT_MAX << std::endl;
    std::cout << "LONG_MIN = " << LONG_MIN << std::endl;
    std::cout << "LONG_MAX = " << LONG_MAX << std::endl;

    std::cout << std::endl << "== 浮点类型的精度 ==" << std::endl;
    std::cout << "FLT_DIG（float 有效十进制位数）  = " << FLT_DIG << std::endl;
    std::cout << "DBL_DIG（double 有效十进制位数） = " << DBL_DIG << std::endl;
    return 0;
}
