#include <iostream>

// 修复版：同样的逻辑、零泄漏——对照实验
class IntBuffer {
private:
    int* data_;
    std::size_t size_;

public:
    explicit IntBuffer(std::size_t n) : data_(new int[n]{}), size_(n) {}
    ~IntBuffer() { delete[] data_; }               // RAII：出块自动还

    IntBuffer(const IntBuffer&) = delete;          // 今天先禁拷贝（Day 57 三法则补全）
    IntBuffer& operator=(const IntBuffer&) = delete;

    int& operator[](std::size_t i) { return data_[i]; }
    std::size_t size() const { return size_; }
};

void fixed_version() {
    IntBuffer buf(100);             // 构造时拿
    buf[0] = 1;                     // 用
}                                    // 出块析构自动还——零泄漏

int main() {
    for (int i = 0; i < 1000; ++i) {
        fixed_version();            // 同样的 1000 次，每次都干净归还
    }
    std::cout << "1000 次分配全部经析构归还——零泄漏（ASan 全程无报告）" << std::endl;
    std::cout << "（RAII 是根治——'忘了 delete'从语法上不存在）" << std::endl;
    return 0;
}
