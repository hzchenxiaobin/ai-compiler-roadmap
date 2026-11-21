#include <cstring>
#include <iostream>

// 浅拷贝 vs 深拷贝：含指针成员的经典事故与修复
class ShallowString {
private:
    char* data_;
    std::size_t len_;

public:
    ShallowString(const char* s) : len_(strlen(s)) {
        data_ = new char[len_ + 1];
        memcpy(data_, s, len_ + 1);
        std::cout << "  [构造] 拿了 " << static_cast<void*>(data_) << std::endl;
    }

    // 编译器默认生成的拷贝构造 = 浅拷贝（只抄指针值，不抄指向的内容）
    // ShallowString(const ShallowString&) = default;  ← 等效于不写

    ~ShallowString() {
        std::cout << "  [析构] 释放 " << static_cast<void*>(data_) << std::endl;
        delete[] data_;
    }

    void print() const { std::cout << "  内容：" << data_ << std::endl; }
};

int main() {
    std::cout << "浅拷贝实验（预期 double free 崩溃）：" << std::endl;
    {
        ShallowString a("hello");
        ShallowString b = a;        // 浅拷贝：b.data_ 和 a.data_ 指向同一块内存！
        std::cout << "拷贝后两个对象指向同一地址——" << std::endl;
        a.print();
        b.print();
    }                                // 析构 b（释放内存）→ 析构 a（再释放同一块 = double free！）
    return 0;
}
