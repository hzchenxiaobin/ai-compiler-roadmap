#include <cstring>
#include <iostream>

// ===== Day 54 项目日：MyString v2——完整拷贝控制（第 9 周收官） =====
// 三法则全手写 + [] 访问 + append + 长度 —— 手写动态数组的毕业考
class MyString {
private:
    char* data_;
    std::size_t len_;

public:
    // 构造
    MyString(const char* s = "") : len_(strlen(s)) {
        data_ = new char[len_ + 1];
        memcpy(data_, s, len_ + 1);
        std::cout << "  [构造] \"" << data_ << "\"" << std::endl;
    }

    // 三法则 ①拷贝构造（深拷贝）
    MyString(const MyString& other) : len_(other.len_) {
        data_ = new char[len_ + 1];
        memcpy(data_, other.data_, len_ + 1);
        std::cout << "  [拷贝构造] \"" << data_ << "\" @ " << static_cast<void*>(data_) << std::endl;
    }

    // 三法则 ②拷贝赋值（三步式）
    MyString& operator=(const MyString& other) {
        std::cout << "  [拷贝赋值] \"" << other.data_ << "\"" << std::endl;
        if (this == &other) { return *this; }         // 自赋值检查
        delete[] data_;                                // 释放旧
        len_ = other.len_;
        data_ = new char[len_ + 1];                    // 开新抄内容
        memcpy(data_, other.data_, len_ + 1);
        return *this;
    }

    // 三法则 ③析构
    ~MyString() {
        std::cout << "  [析构] \"" << data_ << "\"" << std::endl;
        delete[] data_;
    }

    // 访问与工具
    char& operator[](std::size_t i) { return data_[i]; }              // 可写版
    const char& operator[](std::size_t i) const { return data_[i]; }  // 只读版（Day 40）

    std::size_t size() const { return len_; }
    const char* c_str() const { return data_; }

    MyString& append(const char* s) {                // 动态扩容（new 大块 → 抄 → 放旧）
        std::size_t add = strlen(s);
        char* bigger = new char[len_ + add + 1];
        memcpy(bigger, data_, len_);
        memcpy(bigger + len_, s, add + 1);
        delete[] data_;
        data_ = bigger;
        len_ += add;
        return *this;                                 // 支持链式（Day 37/50）
    }
};

int main() {
    std::cout << "== ① 三件套联动 ==" << std::endl;
    MyString a("hello");
    MyString b = a;                                   // 拷贝构造
    MyString c("x");
    c = a;                                            // 拷贝赋值

    std::cout << "\n== ② 下标访问（读+写） ==" << std::endl;
    std::cout << "b[0] = " << b[0] << std::endl;
    b[0] = 'H';                                       // 可写下标
    std::cout << "改后 b = \"" << b.c_str() << "\"（a 仍是 \"" << a.c_str() << "\"——深拷贝独立）" << std::endl;

    std::cout << "\n== ③ 动态扩容 append ==" << std::endl;
    MyString d("ab");
    d.append("cd").append("ef");                      // 链式扩容
    std::cout << "d = \"" << d.c_str() << "\"，size = " << d.size() << std::endl;

    std::cout << "\n== ④ 自赋值 ==" << std::endl;
    a = a;                                            // ①拦下
    std::cout << "自赋值后 a = \"" << a.c_str() << "\"" << std::endl;
    return 0;                                         // ⑤ 逆序析构 ×4
}
