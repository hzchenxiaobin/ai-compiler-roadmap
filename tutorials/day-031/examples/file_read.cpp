#include <iostream>
#include <fstream>
#include <string>

// 文件读入：ifstream + getline 逐行读
int main() {
    std::ifstream in("notes.txt");
    if (!in) {                                 // 文件不存在时这里为真
        std::cout << "notes.txt 不存在（或打不开）" << std::endl;
        return 1;
    }

    std::string line;
    int lineno = 0;
    while (std::getline(in, line)) {           // 黄金搭档读文件：读到 EOF 自然停
        ++lineno;
        std::cout << lineno << ": " << line << std::endl;
    }
    std::cout << "共读到 " << lineno << " 行" << std::endl;
    return 0;
}
