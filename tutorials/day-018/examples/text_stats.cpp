#include <iostream>
#include <sstream>
#include <string>

// ===== 第 18 天项目日：文本统计器 =====
// 统计口径（先说清楚，数字才有意义）：
//   行数   = getline 读到的行数（空行也算一行）
//   字符数 = 各行长度之和，不含换行符
//   单词数 = 按"任意空白"切分出的词数（标点跟着词走，123! 也算一个词）
//   字母频率 = 只统计英文字母，大小写合并计数
int main() {
    std::string line;
    int lines = 0;
    int chars_total = 0;
    int words_total = 0;
    int freq[26] = {0};                       // Day 13 的数组：26 格字母账本

    std::cout << "请输入文本（Ctrl+D 或管道结束）：" << std::endl;
    while (std::getline(std::cin, line)) {    // 黄金搭档外层：读行
        ++lines;
        chars_total += (int)line.size();      // 换行符不计入（getline 已丢弃）

        std::istringstream ss(line);          // 黄金搭档内层：切词
        std::string word;
        while (ss >> word) {
            ++words_total;
        }

        for (std::size_t i = 0; i < line.size(); ++i) {   // 逐字符过账
            char c = line[i];
            if (c >= 'A' && c <= 'Z') {
                ++freq[c - 'A'];              // 大写折算到 0~25
            } else if (c >= 'a' && c <= 'z') {
                ++freq[c - 'a'];              // 小写天然在 0~25
            }
        }
    }

    std::cout << "\n===== 统计结果 =====" << std::endl;
    std::cout << "行数：" << lines << std::endl;
    std::cout << "字符数（不含换行）：" << chars_total << std::endl;
    std::cout << "单词数：" << words_total << std::endl;

    std::cout << "字母频率（只列出现过的）：" << std::endl;
    for (int k = 0; k < 26; ++k) {
        if (freq[k] > 0) {
            char letter = 'a' + k;
            std::cout << "  " << letter << " : " << freq[k] << std::endl;
        }
    }
    return 0;
}
