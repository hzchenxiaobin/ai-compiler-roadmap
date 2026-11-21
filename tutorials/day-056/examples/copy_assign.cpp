#include <cstring>
#include <iostream>

// 拷贝赋值：d = a 的正确写法（含自赋值处理）
class MyString {
private:
    char* data_;
    std::size_t len_;

    void swap(MyString& other) noexcept {         // 交换助手（copy-and-swap 的零件）
        char* td = data_; data_ = other.data_; other.data_ = td;
        std::size_t tl = len_; len_ = other.len_; other.len_ = tl;
    }

public:
    MyString(const char* s) : len_(strlen(s)) {
        data_ = new char[len_ + 1];
        memcpy(data_, s, len_ + 1);
        std::cout << "  [构造] " << data_ << " @ " << static_cast<void*>(data_) << std::endl;
    }

    MyString(const MyString& other) : len_(other.len_) {      // 拷贝构造（Day 55）
        data_ = new char[len_ + 1];
        memcpy(data_, other.data_, len_ + 1);
        std::cout << "  [拷贝构造] 新 @ " << static_cast<void*>(data_) << std::endl;
    }

    // 拷贝赋值：方案 A（教科书三步式）
    MyString& operator=(const MyString& other) {
        std::cout << "  [赋值] 逐字面三步式" << std::endl;
        if (this == &other) {                      // ① 自赋值检查
            std::cout << "    自赋值，直接返回" << std::endl;
            return *this;
        }
        delete[] data_;                            // ② 释放旧内存
        len_ = other.len_;
        data_ = new char[len_ + 1];                // ③ 开新 + 抄内容
        memcpy(data_, other.data_, len_ + 1);
        return *this;                              // 返回 *this 支持链式（Day 49）
    }

    ~MyString() {
        std::cout << "  [析构] 释放 " << static_cast<void*>(data_) << std::endl;
        delete[] data_;
    }

    const char* c_str() const { return data_; }
    void print() const { std::cout << "  \"" << data_ << "\" @ " << static_cast<void*>(data_) << std::endl; }
};

int main() {
    MyString a("hello");
    MyString b("world");
    MyString c("target");

    std::cout << "\n== 普通赋值 c = a ==" << std::endl;
    c = a;
    a.print();
    c.print();                                     // 地址不同（深赋值）

    std::cout << "\n== 自赋值 a = a ==" << std::endl;
    a = a;                                         // 被①拦下，不炸
    a.print();                                     // 内容完好

    std::cout << "\n== 链式赋值 c = b = a ==" << std::endl;
    c = b = a;                                     // 返回 *this 支持链
    b.print();
    c.print();
    return 0;
}
