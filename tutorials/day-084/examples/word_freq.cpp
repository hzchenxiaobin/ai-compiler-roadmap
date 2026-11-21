#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

// ===== Day 84 项目日：词频统计 v1——读文本 → 计数 → 输出（第 14 周收官） =====
// 计划指定 unordered_map——本机 libc++ 下 map/set 的对照也一起做（关联容器下周正课）

// 清洗：转小写、去掉非字母数字（把 don't 拆成 don t——v1 的简化，诚实记录）
std::string clean_word(const std::string& raw) {
    std::string out;
    for (char c : raw) {
        if (std::isalnum(static_cast<unsigned char>(c))) {
            out += static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
        }
    }
    return out;
}

int main() {
    // 1) 生成测试文本（文件不存在的常见兜底——Day 77 的模式）
    const std::string fname = "sample.txt";
    {
        std::ifstream probe(fname);
        if (!probe.good()) {
            std::ofstream out(fname);
            out << "the quick brown fox jumps over the lazy dog\n";
            out << "the dog barks and the fox runs\n";
            out << "a quick brown dog meets a lazy fox\n";
        }
    }

    // 2) 读文件 + 清洗 + 计数（今天的主角）
    std::map<std::string, int> freq;                 // 有序版（按字典序输出）
    std::ifstream in(fname);
    std::string word;
    std::size_t total_words = 0;
    while (in >> word) {
        std::string w = clean_word(word);
        if (w.empty()) { continue; }
        ++freq[w];                                   // 核心：一行搞定计数！
        ++total_words;
    }
    std::cout << "读了 " << total_words << " 个词，去重后 "
              << freq.size() << " 个" << std::endl;

    // 3) 输出一：按字典序（map 天然有序）
    std::cout << "\n== 按字典序（map 的天然序） ==" << std::endl;
    for (const auto& [w, n] : freq) {
        std::cout << "  " << w << " × " << n << std::endl;
    }

    // 4) 输出二：按频次降序 Top 5（把 map 搬进 vector 排序——两种容器的协作）
    std::vector<std::pair<std::string, int>> items(freq.begin(), freq.end());
    std::sort(items.begin(), items.end(),
              [](const auto& a, const auto& b) { return a.second > b.second; });
    std::cout << "\n== 按频次 Top 5 ==" << std::endl;
    for (std::size_t i = 0; i < items.size() && i < 5; ++i) {
        std::cout << "  " << (i + 1) << ". " << items[i].first
                  << " × " << items[i].second << std::endl;
    }
    return 0;
}
