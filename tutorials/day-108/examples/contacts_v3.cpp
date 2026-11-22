#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// ===== Day 108 项目日：通讯录 v3——现代特性重写（计划指定） =====
// v1（Day 30 数组版）→ v3（今天）：auto / {} / enum class / using / 结构化绑定 / if constexpr 全上场

enum class MatchMode { Exact, Prefix };                  // ② enum class（Day 105）

using ContactList = std::vector<std::pair<std::string, std::string>>;   // ③ using 别名

class Contacts {
private:
    std::map<std::string, std::string> by_name_;        // 名字 → 电话

public:
    bool add(const std::string& name, const std::string& phone) {
        auto [it, inserted] = by_name_.insert({name, phone});    // ⑥ 结构化绑定拆 insert 返回
        return inserted;                                 // false = 重名被拒
    }

    const std::string* find(const std::string& name, MatchMode mode) const {
        if constexpr (false) { /* 预留：类型定制演示位 */ }
        if (mode == MatchMode::Exact) {
            auto it = by_name_.find(name);
            return (it != by_name_.end()) ? &it->second : nullptr;
        }
        // Prefix 模式：lower_bound 找前缀起点（有序 map 的范围查询——Day 94）
        auto it = by_name_.lower_bound(name);
        if (it == by_name_.end() || it->first.compare(0, name.size(), name) != 0) {
            return nullptr;
        }
        return &it->second;
    }

    std::size_t remove_prefix(const std::string& prefix) {
        auto first = by_name_.lower_bound(prefix);
        std::size_t removed = 0;
        while (first != by_name_.end() &&
               first->first.compare(0, prefix.size(), prefix) == 0) {
            first = by_name_.erase(first);               // erase 返回下一个（Day 81 接力）
            ++removed;
        }
        return removed;
    }

    void dump() const {
        for (const auto& [name, phone] : by_name_) {    // ⑥ 绑定遍历（计划指定核心）
            std::cout << "  " << name << "：" << phone << std::endl;
        }
    }
    std::size_t size() const { return by_name_.size(); }
};

// ④ constexpr：编译期校验区号格式（简单版：3 位数字）
constexpr bool valid_area_code(int code) {
    return code >= 100 && code <= 999;
}
static_assert(valid_area_code(100), "100 合法——编译期验证通过");
static_assert(!valid_area_code(10), "10 两位——非法也被正确识别");

int main() {
    Contacts book;

    std::cout << "== 录入（{} 初始化 + insert 绑定判重） ==" << std::endl;
    auto entries = {                                     // ① {} + auto（Day 103/104）
        std::pair{"小明", "13800000001"},
        std::pair{"小红", "13800000002"},
        std::pair{"小刚", "13900000003"},
        std::pair{"小鹏", "13811122233"},
    };
    for (const auto& [name, phone] : entries) {          // ⑥ 绑定遍历
        bool ok = book.add(name, phone);
        std::cout << "  " << name << (ok ? " ✔" : " ✘（重名被拒）") << std::endl;
    }
    std::cout << "  再加小明：";
    std::cout << (book.add("小明", "999") ? "✔" : "✘（重名被拒——insert 绑定立判）") << std::endl;

    std::cout << "\n== 全部联系人（绑定遍历） ==" << std::endl;
    book.dump();

    std::cout << "\n== 查找（enum class 双模式） ==" << std::endl;
    if (const std::string* p = book.find("小明", MatchMode::Exact); p) {   // if 初始化语句
        std::cout << "  精确查 小明 → " << *p << std::endl;
    }
    if (const std::string* p = book.find("小", MatchMode::Prefix); p) {
        std::cout << "  前缀查 \"小\" → " << *p << "（第一个匹配）" << std::endl;
    }
    if (const std::string* p = book.find("大王", MatchMode::Exact); !p) {
        std::cout << "  精确查 大王 → nullptr（没有）" << std::endl;
    }

    std::cout << "\n== 前缀删除（erase 接力） ==" << std::endl;
    std::size_t n = book.remove_prefix("小鹏");
    std::cout << "  删除 小鹏* → " << n << " 条；剩 " << book.size() << " 人" << std::endl;

    std::cout << "\n== 通讯录的现代化清单 ==" << std::endl;
    std::cout << "  auto + {}/init_list 录入｜enum class 双模式｜using 列表别名" << std::endl;
    std::cout << "  绑定遍历 + insert 判重｜constexpr 区号校验（static_assert）" << std::endl;
    std::cout << "  （对照 v1 的 char 数组 + 下标循环——十周的距离）" << std::endl;
    return 0;
}
