#include <fstream>
#include <iostream>
#include <string>

// 重写五：存读盘闭环（Day 31）——写两行、读回来、数行数
int main() {
    {
        std::ofstream out("rewrite.txt");
        out << "第一行" << std::endl;
        out << "第二行" << std::endl;
    }                                              // 作用域结束自动关闭

    std::ifstream in("rewrite.txt");
    if (!in) {
        std::cout << "打开失败" << std::endl;
        return 1;
    }
    std::string line;
    int n = 0;
    while (std::getline(in, line)) {
        ++n;
        std::cout << n << ": " << line << std::endl;
    }
    std::cout << "共 " << n << " 行" << std::endl;
    return 0;
}
