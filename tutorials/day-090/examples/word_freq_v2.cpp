#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// ===== Day 90 项目日：词频统计 v2——Top-N + 双排序（计划指定） =====
// v1（Day 84）→ v2 的升级：unordered 提速 + Top-N + 频序/字典序两种输出
std::string clean_word(const std::string& raw) {
    std::string out;
    for (char c : raw) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    return out;
}

struct WordCount {
    std::string word;
    int count;
};

int main() {
    // 测试文本（不存在则生成——Day 77/84 的模式）
    const std::string fname = "sample.txt";
    {
        std::ifstream probe(fname);
        if (!probe.good()) {
            std::ofstream out(fname);
            out << "the quick brown fox jumps over the lazy dog\n";
            out << "the dog barks and the fox runs quickly\n";
            out << "a quick brown dog meets a lazy fox and a happy cat\n";
            out << "the cat and the dog chase the quick fox\n";
        }
    }

    // 1) 计数（v2 升级点①：unordered_map——Day 86 学完哈希后的兑现）
    std::unordered_map<std::string, int> freq;
    std::ifstream in(fname);
    std::string word;
    std::size_t total = 0;
    while (in >> word) {
        std::string w = clean_word(word);
        if (w.empty()) { continue; }
        ++freq[w];
        ++total;
    }
    std::cout << "读了 " << total << " 词，去重 " << freq.size() << " 个（unordered 计数）" << std::endl;

    // 2) 搬进 vector（v2 升级点②：Top-N 的载体）
    std::vector<WordCount> items;
    items.reserve(freq.size());
    for (const auto& [w, n] : freq) { items.push_back({w, n}); }

    // 3) 排序一：按频率降序、同频字典序升序（v2 升级点③）
    std::sort(items.begin(), items.end(), [](const WordCount& a, const WordCount& b) {
        if (a.count != b.count) { return a.count > b.count; }
        return a.word < b.word;
    });
    std::cout << "\n== 按频率排序（同频按字典序）—— 全部 ==" << std::endl;
    for (const auto& it : items) {
        std::cout << "  " << it.word << " × " << it.count << std::endl;
    }

    // 4) Top-N（计划指定）
    const int N = 5;
    std::cout << "\n== Top-" << N << " ==" << std::endl;
    for (int i = 0; i < N && i < (int)items.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << items[i].word
                  << " × " << items[i].count << std::endl;
    }

    // 5) 排序二：按字典序（v1 的输出——v2 保留双视图）
    std::sort(items.begin(), items.end(), [](const WordCount& a, const WordCount& b) {
        return a.word < b.word;
    });
    std::cout << "\n== 按字典序排序（前 10 展示） ==" << std::endl;
    for (int i = 0; i < 10 && i < (int)items.size(); ++i) {
        std::cout << "  " << items[i].word << " × " << items[i].count << std::endl;
    }
    std::cout << "  …（v1 的字典序视图 v2 保留——双排序是计划要求）" << std::endl;
    return 0;
}
