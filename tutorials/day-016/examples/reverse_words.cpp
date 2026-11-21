#include <iostream>
#include <string>

// 计划练习二：反转单词顺序
// "the quick brown fox" → "fox brown quick the"
int main() {
    std::string sentence = "the quick brown fox";
    std::string words[20];                      // 词袋子（够用就行，第 14 周有 vector）
    int count = 0;

    // 正向切词：find 找空格，substr 搬词
    std::size_t start = 0;
    while (start <= sentence.size()) {
        std::size_t space = sentence.find(' ', start);
        if (space == std::string::npos) {
            space = sentence.size();            // 最后一个词后面没有空格
        }
        if (space > start) {                    // 跳过连续空格
            words[count] = sentence.substr(start, space - start);
            ++count;
        }
        if (space == sentence.size()) {
            break;                              // 收尾
        }
        start = space + 1;
    }

    std::cout << "原句：" << sentence << std::endl;
    std::cout << "反转：";
    for (int i = count - 1; i >= 0; --i) {      // 从最后一个词倒着打印
        std::cout << words[i];
        if (i > 0) {
            std::cout << " ";
        }
    }
    std::cout << std::endl;
    return 0;
}
