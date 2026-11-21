#include <iostream>

// 实验一：块作用域——围栏内可见，出栏即"查无此人"
// 实验二：内层遮蔽——同名变量在块内"另起炉灶"
int main() {
    int x = 10;                       // 外层（main 全范围可见）

    {
        int y = 99;                   // 只在这对 {} 里活着
        int inner_x = 5;              // 内层新名字
        std::cout << "块内：x = " << x
                  << "，y = " << y
                  << "，inner_x = " << inner_x << std::endl;
    }
    // 此处再访问 y 或 inner_x 都会编译错误（见教程 5.4 的实测）

    std::cout << "块外：x = " << x << std::endl;

    {
        int x = 777;                  // 故意与外层同名：遮蔽（shadowing）
        std::cout << "内层同名块：x = " << x << std::endl;   // 看见的是 777
    }
    std::cout << "出块后：x = " << x << std::endl;           // 外层毫发无损
    return 0;
}
