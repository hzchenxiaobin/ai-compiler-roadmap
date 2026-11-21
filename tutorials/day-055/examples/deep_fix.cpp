#include <cstring>
#include <iostream>

// 修复版：手写深拷贝的拷贝构造
class DeepString {
private:
    char* data_;
    std::size_t len_;

public:
    DeepString(const char* s) : len_(strlen(s)) {
        data_ = new char[len_ + 1];
        memcpy(data_, s, len_ + 1);
        std::cout << "  [构造] " << static_cast<void*>(data_) << "：" << data_ << std::endl;
    }

    // 深拷贝的拷贝构造：另开内存、复制内容
    DeepString(const DeepString& other) : len_(other.len_) {
        data_ = new char[len_ + 1];               // ① 另开一块同样大小的内存
        memcpy(data_, other.data_, len_ + 1);      // ② 把内容抄过来
        std::cout << "  [深拷贝] 新地址 " << static_cast<void*>(data_)
                  << "（源 " << static_cast<void*>(other.data_) << "）" << std::endl;
    }

    ~DeepString() {
        delete[] data_;
        std::cout << "  [析构] 释放 " << static_cast<void*>(data_) << std::endl;
    }

    void print() const { std::cout << "  内容：" << data_ << " @ " << static_cast<void*>(data_) << std::endl; }
};

int main() {
    std::cout << "深拷贝实验（各自独立，安全析构）：" << std::endl;
    {
        DeepString a("hello");
        DeepString b = a;            // 深拷贝：新内存、抄内容
        a.print();                   // 地址不同
        b.print();
    }                                // 各自释放各自的——没有 double free
    std::cout << "安全退出（exit=0 预期）" << std::endl;
    return 0;
}
