#include <cstring>
#include <iostream>

// 三法则完整版：析构 + 拷贝构造 + 拷贝赋值 三件套一起写
class Buffer {
private:
    char* data_;
    std::size_t len_;

public:
    // 构造（三件套之外的第四件——出生）
    Buffer(const char* s) : len_(strlen(s)) {
        data_ = new char[len_ + 1];
        memcpy(data_, s, len_ + 1);
        std::cout << "  [构造] " << data_ << std::endl;
    }

    // ①拷贝构造（Day 55）
    Buffer(const Buffer& other) : len_(other.len_) {
        data_ = new char[len_ + 1];
        memcpy(data_, other.data_, len_ + 1);
        std::cout << "  [拷贝构造] " << data_ << std::endl;
    }

    // ②拷贝赋值（Day 56 三步式）
    Buffer& operator=(const Buffer& other) {
        std::cout << "  [拷贝赋值] " << other.data_ << std::endl;
        if (this == &other) { return *this; }
        delete[] data_;
        len_ = other.len_;
        data_ = new char[len_ + 1];
        memcpy(data_, other.data_, len_ + 1);
        return *this;
    }

    // ③析构（Day 51/46）
    ~Buffer() {
        std::cout << "  [析构] 释放 " << static_cast<void*>(data_) << std::endl;
        delete[] data_;
    }

    const char* c_str() const { return data_; }
};

int main() {
    std::cout << "== 三件套全上场 ==" << std::endl;
    Buffer a("hello");               // 构造
    Buffer b = a;                    // ①拷贝构造
    Buffer c("world");
    c = a;                           // ②拷贝赋值

    std::cout << "\n== 传值/返回也触发 ==" << std::endl;
    // 值传参触发①，出参析构触发③——三件套环环相扣

    std::cout << "\n== 出块统一析构 ==" << std::endl;
    return 0;                        // ③×3 逆序
}
