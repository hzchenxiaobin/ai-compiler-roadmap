#include <iostream>
#include <string>

// 题四：最长公共前缀（LeetCode #14）
// 找出字符串数组中所有词共有的最长开头
std::string longest_common_prefix(const std::string words[], int n) {
    if (n == 0) {
        return "";
    }
    std::string prefix = words[0];             // 拿第一个词当候选
    for (int i = 1; i < n; ++i) {
        std::size_t k = 0;
        while (k < prefix.size() && k < words[i].size()
               && prefix[k] == words[i][k]) {
            ++k;                               // 和当前词比到第一个不同处
        }
        prefix = prefix.substr(0, k);          // 候选截短到共同部分
        if (prefix.empty()) {
            break;                             // 已经没有公共前缀，提前收工
        }
    }
    return prefix;
}

int main() {
    std::string w1[3] = {"flower", "flow", "flight"};
    std::string w2[3] = {"dog", "racecar", "car"};
    std::string w3[2] = {"same", "same"};

    std::cout << "flower/flow/flight →「" << longest_common_prefix(w1, 3) << "」" << std::endl;
    std::cout << "dog/racecar/car　　→「" << longest_common_prefix(w2, 3) << "」" << std::endl;
    std::cout << "same/same　　　　　→「" << longest_common_prefix(w3, 2) << "」" << std::endl;
    return 0;
}
