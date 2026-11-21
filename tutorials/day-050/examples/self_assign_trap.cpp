#include <cstring>
#include <iostream>

// 自赋值陷阱演示：不写检查会发生什么（删掉①后 a = a 会怎样）
class NoCheck {
private:
    char* data_;
    std::size_t len_;

public:
    NoCheck(const char* s) : len_(strlen(s)) {
        data_ = new char[len_ + 1];
        memcpy(data_, s, len_ + 1);
    }

    // 故意不写自赋值检查的三步式
    NoCheck& operator=(const NoCheck& other) {
        delete[] data_;                            // ② 先释放自己的
        len_ = other.len_;
        data_ = new char[len_ + 1];
        memcpy(data_, other.data_, len_ + 1);      // other 就是 *this！data_ 已被释放！
        return *this;
    }

    ~NoCheck() { delete[] data_; }

    void print() const { std::cout << "  \"" << data_ << "\"" << std::endl; }
};

int main() {
    NoCheck a("hello");
    std::cout << "赋值前：";
    a.print();

    a = a;                                         // 自赋值：先 delete 再从已释放内存抄！
    std::cout << "自赋值后：";
    a.print();                                     // 读已释放内存 = UB（可能崩、可能乱码）
    return 0;
}
