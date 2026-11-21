#include <iostream>
#include <iomanip>

// 显示格式化巡礼：默认、fixed、setprecision、setw
int main() {
    double pi = 3.141592653589793;
    double progress = 3.0 * 100.0 / 144.0;

    std::cout << "== 默认显示（6 位有效数字） ==" << std::endl;
    std::cout << pi << std::endl;
    std::cout << progress << std::endl;

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "== fixed + setprecision(2) ==" << std::endl;
    std::cout << pi << std::endl;
    std::cout << progress << std::endl;

    std::cout << std::setprecision(10);
    std::cout << "== fixed + setprecision(10) ==" << std::endl;
    std::cout << pi << std::endl;

    std::cout << "== setw(14) 表格对齐 ==" << std::endl;
    std::cout << std::setw(14) << "Day" << std::setw(14) << "Percent" << std::endl;
    std::cout << std::setw(14) << 1 << std::setw(14) << 0.69 << std::endl;
    std::cout << std::setw(14) << 12 << std::setw(14) << 8.33 << std::endl;
    return 0;
}
