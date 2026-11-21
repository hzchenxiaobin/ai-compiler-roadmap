#include <iostream>

// Day 47：动态数组 v2——倍增扩容、reserve、capacity 与 size 的区别
class IntArray {
private:
    int* data_;
    std::size_t size_;
    std::size_t capacity_;
    int grow_count_;                        // 扩容计数（账本）

    static constexpr std::size_t kInitCap = 4;

    void grow() {                           // 倍增扩容（v2 的主角）
        std::size_t new_cap = capacity_ * 2;
        int* bigger = new int[new_cap]{};
        for (std::size_t i = 0; i < size_; ++i) { bigger[i] = data_[i]; }
        delete[] data_;
        data_ = bigger;
        capacity_ = new_cap;
        ++grow_count_;
        std::cout << "  [扩容 " << grow_count_ << "] " << new_cap / 2 << " → " << new_cap << std::endl;
    }

public:
    IntArray() : data_(new int[kInitCap]{}), size_(0), capacity_(kInitCap), grow_count_(0) {}

    ~IntArray() { delete[] data_; }
    IntArray(const IntArray&) = delete;             // 拷贝控制仍是第 10 周的课
    IntArray& operator=(const IntArray&) = delete;

    void push_back(int v) {
        if (size_ == capacity_) { grow(); }
        data_[size_++] = v;
    }

    void reserve(std::size_t new_cap) {             // v2 新增：预留容量
        if (new_cap <= capacity_) { return; }       // 只扩大不缩小
        int* bigger = new int[new_cap]{};
        for (std::size_t i = 0; i < size_; ++i) { bigger[i] = data_[i]; }
        delete[] data_;
        data_ = bigger;
        capacity_ = new_cap;
        ++grow_count_;
        std::cout << "  [reserve " << grow_count_ << "] 直接扩到 " << new_cap << std::endl;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    int& at(std::size_t i) {
        if (i >= size_) { std::cout << "  [越界] " << i << " >= " << size_ << std::endl; std::exit(1); }
        return data_[i];
    }
};

int main() {
    std::cout << "== 倍增策略：逐个 push 100 个 ==" << std::endl;
    IntArray a;
    for (int i = 0; i < 100; ++i) {
        a.push_back(i);
    }
    std::cout << "  size = " << a.size() << "，capacity = " << a.capacity()
              << "（倍增 5 次连登：4→8→16→32→64→128）" << std::endl;

    std::cout << "\n== reserve 预留：一次到位 ==" << std::endl;
    IntArray b;
    b.reserve(128);                          // 提前留好
    for (int i = 0; i < 100; ++i) {
        b.push_back(i);                      // 全程无扩容日志！
    }
    std::cout << "  size = " << b.size() << "，capacity = " << b.capacity()
              << "（0 次倍增扩容——reserve 1 次搞定）" << std::endl;

    std::cout << "\n== size 与 capacity 的身份对照 ==" << std::endl;
    IntArray c;
    c.push_back(1);
    std::cout << "  push 1 个后：size = " << c.size() << "，capacity = " << c.capacity()
              << "（住了 1 人、租了 4 间）" << std::endl;
    std::cout << "  at(1) 尝试（size 边界外、capacity 内）：" << std::endl;
    std::cout << "  ";
    c.at(1);                                 // 越界被拦——at 只认 size
    return 0;
}
