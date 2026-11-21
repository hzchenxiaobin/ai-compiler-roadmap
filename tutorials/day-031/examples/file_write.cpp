#include <iostream>
#include <fstream>

// 文件写出：ofstream 就像对着文件的 cout
int main() {
    std::ofstream out("notes.txt");            // 打开（不存在就创建）
    if (!out) {                                // 打不开要检查（比如目录没权限）
        std::cout << "文件打开失败" << std::endl;
        return 1;
    }
    out << "第 31 天学习笔记" << std::endl;     // << 的用法和 cout 一模一样
    out << "1. ofstream 负责写文件" << std::endl;
    out << "2. 文件在程序结束后依然存在" << std::endl;
    // out 在 main 结束时自动关闭（离开作用域自动收摊）

    std::cout << "已写入 notes.txt（用 cat 可以查看）" << std::endl;
    return 0;
}
