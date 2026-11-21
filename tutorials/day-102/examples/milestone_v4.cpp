#include <iostream>
#include <list>
#include <map>
#include <string>
#include <unordered_map>

// ===== Day 102 里程碑：脱稿重写 LRU + map/unordered 底层差异（阶段四毕业考） =====
// 一、LRU 缓存——从零重写（不看 Day 89 的答案）
class LRUCache {
private:
    int cap_;
    std::list<std::pair<int, int>> items_;                                   // 头=最近
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> pos_;   // key → 位置

public:
    explicit LRUCache(int capacity) : cap_(capacity) {}

    int get(int key) {
        auto it = pos_.find(key);
        if (it == pos_.end()) { return -1; }
        items_.splice(items_.begin(), items_, it->second);                    // O(1) 移头
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = pos_.find(key);
        if (it != pos_.end()) {                                              // 已有：更新+移头
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second);
            return;
        }
        if ((int)items_.size() >= cap_) {                                    // 满：删尾（两处同步！）
            pos_.erase(items_.back().first);
            items_.pop_back();
        }
        items_.push_front({key, value});
        pos_[key] = items_.begin();
    }

    void dump() const {
        std::cout << "  链表视图：[";
        for (const auto& [k, v] : items_) { std::cout << k << ' '; }
        std::cout << "]（头=最近）" << std::endl;
    }
};

// 二、底层差异的"程序化讲稿"——每个论断配机器证据
void explain_internals() {
    std::cout << "\n== 讲稿二：map 与 unordered_map 的底层差异 ==" << std::endl;

    std::cout << "结构层：" << std::endl;
    std::cout << "  map：红黑树（自平衡二叉搜索树）——节点带键值+三指针" << std::endl;
    std::cout << "  unordered_map：哈希桶数组——键经哈希函数映射到桶" << std::endl;
    {
        std::map<std::string, int> m = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
        std::unordered_map<std::string, int> um = {{"apple", 1}, {"banana", 2}, {"cherry", 3}};
        std::cout << "  sizeof(map)=" << sizeof(m) << "；sizeof(um)=" << sizeof(um) << std::endl;
    }

    std::cout << "序层：" << std::endl;
    {
        std::map<std::string, int> m = {{"cherry", 3}, {"apple", 1}, {"banana", 2}};
        std::unordered_map<std::string, int> um = {{"cherry", 3}, {"apple", 1}, {"banana", 2}};
        std::cout << "  同样按 cherry/apple/banana 插入——" << std::endl;
        std::cout << "  map 遍历：";
        for (const auto& [k, v] : m) { std::cout << k << ' '; }
        std::cout << "（字典序=中序遍历）" << std::endl;
        std::cout << "  um 遍历：";
        for (const auto& [k, v] : um) { std::cout << k << ' '; }
        std::cout << "（桶位序——本次碰巧像字典序，但无保证！）" << std::endl;
    }

    std::cout << "复杂度层：" << std::endl;
    std::cout << "  map：查找/插入/删除 O(log n)——树高 ~log₂n 层层比" << std::endl;
    std::cout << "  um：平均 O(1)（一次哈希直达桶）——最坏 O(n)（全冲突）" << std::endl;

    std::cout << "选型层：" << std::endl;
    std::cout << "  要有序遍历/范围查询 → map；要快（计数/缓存）→ unordered" << std::endl;
    std::cout << "  自定义 key：map 只要 operator<；um 还要 hash+equal" << std::endl;
}

int main() {
    std::cout << "== 讲稿一：脱稿重写 LRU（容量 3） ==" << std::endl;
    LRUCache cache(3);
    cache.put(1, 100);
    cache.put(2, 200);
    cache.put(3, 300);
    cache.dump();
    std::cout << "  get(1) = " << cache.get(1) << "（命中——1 移头）" << std::endl;
    cache.dump();
    cache.put(4, 400);                        // 满：淘汰最久未用的 2
    std::cout << "  put(4)：淘汰 2" << std::endl;
    cache.dump();
    std::cout << "  get(2) = " << cache.get(2) << "（已淘汰 → -1）" << std::endl;
    std::cout << "  get(4) = " << cache.get(4) << "（新项在）" << std::endl;
    cache.put(1, 111);                        // 更新已有键
    std::cout << "  put(1,111)：更新不占名额，get(1) = " << cache.get(1) << std::endl;
    cache.dump();

    explain_internals();
    return 0;
}
