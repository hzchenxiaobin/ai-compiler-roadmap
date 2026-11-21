#include <iostream>
#include <sstream>
#include <string>

// getline 的黄金搭档：while (getline(cin, line)) 逐行处理直到输入结束
int main() {
    std::string line;
    int lines = 0;
    int words = 0;

    std::cout << "逐行输入（管道喂入或 Ctrl+D 结束）：" << std::endl;
    while (std::getline(std::cin, line)) {         // 读到一行就进循环；EOF 才出
        ++lines;
        std::istringstream ss(line);
        std::string word;
        int words_in_line = 0;
        while (ss >> word) {
            ++words_in_line;
        }
        words += words_in_line;
        std::cout << "第 " << lines << " 行：长度 " << line.size()
                  << "，" << words_in_line << " 个词" << std::endl;
    }
    std::cout << "合计：" << lines << " 行，" << words << " 个词" << std::endl;
    return 0;
}
