#include <iostream>
#include <map>
#include <set>
#include <string>

// Day 85：map 与 set——红黑树基础、有序遍历（计划指定：单词计数按字典序输出）
int main() {
    std::cout << "== map：键值对 + 有序 ==" << std::endl;
    std::map<std::string, int> ages;
    ages["小明"] = 15;
    ages["小红"] = 14;
    ages["小刚"] = 16;
    ages["小明"] = 16;                    // 同键再赋值 = 覆盖（键唯一）

    std::cout << "  插入顺序：小明 小红 小刚 小明（重复）" << std::endl;
    std::cout << "  遍历输出（字典序）：";
    for (const auto& [name, age] : ages) {
        std::cout << name << "(" << age << ") ";
    }
    std::cout << std::endl;
    std::cout << "  （插入乱序、遍历有序——红黑树在维持序）" << std::endl;
    std::cout << "  size = " << ages.size() << "（小明的 15 被覆盖——键唯一）" << std::endl;

    std::cout << "\n== 查找三兄弟 ==" << std::endl;
    if (ages.count("小明") > 0) {
        std::cout << "  count(小明) = " << ages.count("小明") << "（存在）" << std::endl;
    }
    auto it = ages.find("小红");
    if (it != ages.end()) {
        std::cout << "  find(小红) → " << it->first << " = " << it->second << std::endl;
    }
    std::cout << "  count(小王) = " << ages.count("小王") << "（不存在）" << std::endl;
    // 注意陷阱：operator[] 会插入！
    std::cout << "  访问前 size = " << ages.size() << std::endl;
    std::cout << "  ages[\"小王\"]（只想看看）= " << ages["小王"] << std::endl;
    std::cout << "  访问后 size = " << ages.size() << "（被插入了！——operator[] 的陷阱）" << std::endl;

    std::cout << "\n== set：只有键的集合 ==" << std::endl;
    std::set<std::string> seen;
    std::string words[] = {"apple", "banana", "apple", "cherry", "banana"};
    for (const auto& w : words) {
        bool inserted = seen.insert(w).second;      // insert 返回 pair<迭代器, 是否成功>
        std::cout << "  insert(" << w << ") → " << (inserted ? "新插入" : "已存在（忽略）") << std::endl;
    }
    std::cout << "  set 内容（字典序）：";
    for (const auto& w : seen) { std::cout << w << ' '; }
    std::cout << "（自动去重+排序）" << std::endl;

    std::cout << "\n== 计划指定练习：单词计数按字典序输出 ==" << std::endl;
    std::map<std::string, int> freq;
    std::string text[] = {"the", "quick", "brown", "fox", "the", "lazy", "dog", "the"};
    for (const auto& w : text) { ++freq[w]; }
    for (const auto& [w, n] : freq) {
        std::cout << "  " << w << " × " << n << std::endl;
    }
    std::cout << "  （Day 84 的词频核心今天有了原理注脚）" << std::endl;
    return 0;
}
