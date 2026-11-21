#include <cstring>
#include <iostream>

// ===== Day 60 项目日：MyString v1——内部动态 char 数组，正确的构造/析构/拷贝 =====
// （第 10 周拷贝控制的毕业考；v2 的运算符集是第 11 周 Day 66 的课）
class MyString {
private:
    char* data_;
    std::size_t len_;

public:
    // 构造（含默认）
    MyString(const char* s = "") : len_(strlen(s)) {
        data_ = new char[len_ + 1];
        memcpy(data_, s, len_ + 1);
        std::cout << "  [构造] \"" << data_ << "\" @ " << static_cast<void*>(data_) << std::endl;
    }

    // 三法则 ①拷贝构造（深拷贝）
    MyString(const MyString& other) : len_(other.len_) {
        data_ = new char[len_ + 1];                 // 新开内存
        memcpy(data_, other.data_, len_ + 1);       // 抄内容
        std::cout << "  [拷贝构造] \"" << data_ << "\" @ " << static_cast<void*>(data_) << std::endl;
    }

    // 三法则 ②拷贝赋值（三步式）
    MyString& operator=(const MyString& other) {
        std::cout << "  [拷贝赋值] \"" << other.data_ << "\"" << std::endl;
        if (this == &other) { return *this; }       // 自赋值检查
        delete[] data_;                              // 释放旧
        len_ = other.len_;
        data_ = new char[len_ + 1];
        memcpy(data_, other.data_, len_ + 1);
        return *this;
    }

    // 三法则 ③析构
    ~MyString() {
        std::cout << "  [析构] \"" << data_ << "\"" << std::endl;
        delete[] data_;
    }

    const char* c_str() const { return data_; }
    std::size_t size() const { return len_; }

    friend std::ostream& operator<<(std::ostream& os, const MyString& s);   // Day 59 的技能
};

std::ostream& operator<<(std::ostream& os, const MyString& s) {
    os << s.data_;
    return os;
}

int main() {
    std::cout << "== 三件套联动（五场景） ==" << std::endl;
    MyString a("hello");                // 构造
    MyString b = a;                     // ①拷贝构造（深）
    MyString c("x");
    c = a;                              // ②拷贝赋值（三步式）

    std::cout << "\n== 深拷贝独立性 ==" << std::endl;
    MyString d(a);                      // 拷贝构造
    std::cout << "  d = \"" << d << "\"，a = \"" << a << "\"" << std::endl;
    std::cout << "  两块内存独立（地址见上面的日志）——改 d 不动 a" << std::endl;

    std::cout << "\n== 自赋值 ==" << std::endl;
    MyString& a_alias = a;              // 引用别名（绕开 -Wself-assign 的教学演示法）
    a = a_alias;                        // ④被①拦下
    std::cout << "  自赋值后 a = \"" << a << "\"" << std::endl;
    return 0;                           // ⑤逆序析构 ×4
}
