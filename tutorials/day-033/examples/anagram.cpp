#include <iostream>
#include <string>

// 题三：有效的字母异位词（LeetCode #242）
// 两词是否只是字母顺序不同？26 格频率表对比（Day 18 的老办法）
bool is_anagram(const std::string& a, const std::string& b) {
    if (a.size() != b.size()) {
        return false;                          // 长度不同直接出局
    }
    int freq[26] = {0};
    for (std::size_t i = 0; i < a.size(); ++i) {
        ++freq[a[i] - 'a'];                    // a 的每个字母 +1
        --freq[b[i] - 'a'];                    // b 的每个字母 -1（一次循环干两件事）
    }
    for (int k = 0; k < 26; ++k) {
        if (freq[k] != 0) {
            return false;                      // 有净多或净少 → 不是异位词
        }
    }
    return true;
}

int main() {
    std::cout << "anagram / nagarat → " << (is_anagram("anagram", "nagarat") ? "是" : "否") << std::endl;
    std::cout << "rat / car　　　　→ " << (is_anagram("rat", "car") ? "是" : "否") << std::endl;
    std::cout << "listen / silent → " << (is_anagram("listen", "silent") ? "是" : "否") << std::endl;
    std::cout << "aab / abb　　　 → " << (is_anagram("aab", "abb") ? "是" : "否") << std::endl;
    return 0;
}
