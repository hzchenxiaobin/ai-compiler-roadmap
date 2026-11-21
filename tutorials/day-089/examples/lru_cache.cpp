#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <vector>

// ===== Day 89 项目日：LRU 缓存——list + unordered_map（面试高频经典题） =====
// LRU = Least Recently Used：容量满时淘汰"最久没用"的那条
// 双结构协作：list 管序（头=最近用、尾=最久没用）、unordered_map 管查找（O(1) 定位）
class LRUCache {
private:
    int capacity_;
    std::list<std::pair<int, int>> items_;                       // (key, value)——链表头=最新
    std::unordered_map<int, std::list<std::pair<int, int>>::iterator> pos_;   // key → 链表位置

public:
    explicit LRUCache(int capacity) : capacity_(capacity) {}

    int get(int key) {
        auto it = pos_.find(key);
        if (it == pos_.end()) { return -1; }                     // 没有
        // 命中：移到链表头（最近使用）
        items_.splice(items_.begin(), items_, it->second);       // O(1) 摘节点接到头
        return it->second->second;
    }

    void put(int key, int value) {
        auto it = pos_.find(key);
        if (it != pos_.end()) {                                   // 已存在：更新值 + 移到头
            it->second->second = value;
            items_.splice(items_.begin(), items_, it->second);
            return;
        }
        if ((int)items_.size() >= capacity_) {                    // 满：淘汰链表尾（最久未用）
            int lru_key = items_.back().first;
            pos_.erase(lru_key);                                  // 同步删 map
            items_.pop_back();                                    // 删链表尾
        }
        items_.push_front({key, value});                          // 新项进头
        pos_[key] = items_.begin();                               // 记位置
    }

    void dump(const char* label) const {
        std::cout << "  " << label << "：[";
        for (const auto& [k, v] : items_) { std::cout << k << ' '; }
        std::cout << "]（头=最近）" << std::endl;
    }
};

int main() {
    LRUCache cache(2);                                            // 容量 2

    std::cout << "== 基本流程（容量 2） ==" << std::endl;
    cache.put(1, 100);
    cache.dump("put(1)");
    cache.put(2, 200);
    cache.dump("put(2)");
    std::cout << "  get(1) = " << cache.get(1) << "（命中——1 移到头）" << std::endl;
    cache.dump("get(1) 后");
    cache.put(3, 300);                                            // 满：淘汰最久未用的 2
    std::cout << "  put(3)：容量满 → 淘汰尾部的 2" << std::endl;
    cache.dump("put(3) 后");
    std::cout << "  get(2) = " << cache.get(2) << "（已被淘汰 → -1）" << std::endl;

    std::cout << "\n== 更新已有键（不占新名额） ==" << std::endl;
    cache.put(1, 111);                                            // 1 已存在：更新 + 移头
    cache.dump("put(1,111) 后");
    std::cout << "  get(1) = " << cache.get(1) << "（新值 ✔）" << std::endl;
    cache.put(4, 400);                                            // 淘汰 3
    cache.dump("put(4) 后（3 被淘汰）");
    std::cout << "  get(3) = " << cache.get(3) << "、get(4) = " << cache.get(4) << std::endl;
    return 0;
}
