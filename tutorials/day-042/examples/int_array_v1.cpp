#include <cassert>
#include <iostream>

// ===== Day 42 项目日：手写动态数组 v1（第 7 周毕业考） =====
// 构造 / 析构 / push_back / size / at —— 六天内存课的总装
class IntArray {
private:
    int* data_;
    std::size_t size_;
    std::size_t capacity_;

    static constexpr std::size_t kInitCap = 4;    // 初始容量

public:
    IntArray() : data_(new int[kInitCap]{}), size_(0), capacity_(kInitCap) {
        std::cout << "  [构造] 容量 " << capacity_ << " 起步" << std::endl;
    }

    ~IntArray() {
        std::cout << "  [析构] 释放 " << static_cast<void*>(data_)
                  << "（存过 " << size_ << " 个元素）" << std::endl;
        delete[] data_;
    }

    // 第 10 周才写拷贝控制——今天先禁拷贝（Day 41 的教训：浅拷贝是雷）
    IntArray(const IntArray&) = delete;
    IntArray& operator=(const IntArray&) = delete;

    void push_back(int v) {
        if (size_ == capacity_) {                  // 满了：扩容（倍增策略）
            std::size_t new_cap = capacity_ * 2;
            int* bigger = new int[new_cap]{};
            for (std::size_t i = 0; i < size_; ++i) { bigger[i] = data_[i]; }
            delete[] data_;                        // 旧数据已搬——放旧块
            data_ = bigger;
            capacity_ = new_cap;
            std::cout << "  [扩容] " << capacity_/2 << " → " << capacity_ << std::endl;
        }
        data_[size_++] = v;
    }

    std::size_t size() const { return size_; }
    std::size_t capacity() const { return capacity_; }

    int& at(std::size_t i) {                       // 带边界检查（v1 的安全通道）
        if (i >= size_) {
            std::cout << "  [at 越界] index " << i << " >= size " << size_ << std::endl;
            std::exit(1);
        }
        return data_[i];
    }

    const int& at(std::size_t i) const {
        if (i >= size_) {
            std::cout << "  [at 越界]" << std::endl;
            std::exit(1);
        }
        return data_[i];
    }
};

int main() {
    std::cout << "== 基本流程 ==" << std::endl;
    IntArray arr;
    for (int i = 1; i <= 5; ++i) {
        arr.push_back(i * 10);
    }
    std::cout << "  size = " << arr.size() << "，capacity = " << arr.capacity() << std::endl;
    for (std::size_t i = 0; i < arr.size(); ++i) {
        std::cout << "  at(" << i << ") = " << arr.at(i) << std::endl;
    }

    std::cout << "\n== 越界被拦 ==" << std::endl;
    std::cout << "  尝试 at(99)……" << std::endl;
    arr.at(99);                                    // 打印越界信息后 exit(1)
    return 0;
}
