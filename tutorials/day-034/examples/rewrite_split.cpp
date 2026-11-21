#include <iostream>
#include <sstream>
#include <string>

// 重写三：getline + stringstream 切词统计（Day 17）
int main() {
    std::string line = "the quick   brown fox";
    std::istringstream ss(line);

    std::string word;
    int count = 0;
    int total_len = 0;
    while (ss >> word) {
        ++count;
        total_len += (int)word.size();
    }
    std::cout << "词数 = " << count << "，总字符 = " << total_len << std::endl;
    return 0;
}
