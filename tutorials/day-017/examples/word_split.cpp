#include <iostream>
#include <sstream>
#include <string>

// stringstream：把字符串当成传送带，>> 自动按空白切词
int main() {
    std::string sentence = "the quick   brown fox\tjumps over";   // 故意混多空格和制表符

    std::istringstream ss(sentence);      // 用字符串造一条传送带
    std::string word;
    int count = 0;

    std::cout << "句子：【" << sentence << "】" << std::endl;
    std::cout << "切出的词：" << std::endl;
    while (ss >> word) {                  // >> 自动跳过任意空白，切出一个词
        ++count;
        std::cout << "  " << count << ". " << word
                  << "（长度 " << word.size() << "）" << std::endl;
    }
    std::cout << "共 " << count << " 个词" << std::endl;
    return 0;
}
