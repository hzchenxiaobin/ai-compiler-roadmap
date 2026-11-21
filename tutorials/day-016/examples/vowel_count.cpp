#include <iostream>
#include <string>

// 计划练习一：统计元音字母（大小写各算各，也数总数）
bool is_vowel(char c) {
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int main() {
    std::string text = "The quick brown fox jumps over the lazy dog";
    int counts[5] = {0};                        // a e i o u
    const char vowels[5] = {'a', 'e', 'i', 'o', 'u'};
    int total = 0;

    for (std::size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (c >= 'A' && c <= 'Z') {             // 大写转小写再判
            c = c - 'A' + 'a';
        }
        for (int k = 0; k < 5; ++k) {
            if (c == vowels[k]) {
                ++counts[k];
                ++total;
                break;                          // 找到就别再比了
            }
        }
    }

    std::cout << "句子：" << text << std::endl;
    for (int k = 0; k < 5; ++k) {
        std::cout << vowels[k] << " : " << counts[k] << std::endl;
    }
    std::cout << "元音总数 : " << total << std::endl;
    return 0;
}
