#include <cstring>
#include <iostream>
#include <utility>
#include <vector>

// Day 111：移动构造/移动赋值 + noexcept——给 MyString 补移动语义（计划指定）
class MyString {
public:
    MyString(const char* s = "") {
        size_ = strlen(s);
        data_ = new char[size_ + 1];
        memcpy(data_, s, size_ + 1);
        std::cout << "  [构造] \"" << data_ << "\"（" << size_ << " 字节）" << std::endl;
    }
    MyString(const MyString& o) : size_(o.size_) {                    // 拷贝构造
        data_ = new char[size_ + 1];
        memcpy(data_, o.data_, size_ + 1);
        std::cout << "  [拷贝构造] \"" << data_ << "\"（新分配 + 逐字节抄）" << std::endl;
    }
    MyString(MyString&& o) noexcept : data_(o.data_), size_(o.size_) { // 移动构造（偷！）
        o.data_ = nullptr;                                             // 掏空源（关键！）
        o.size_ = 0;
        std::cout << "  [移动构造] 偷指针（零分配零拷贝）" << std::endl;
    }
    MyString& operator=(const MyString& o) {                          // 拷贝赋值
        if (this == &o) { return *this; }
        delete[] data_;
        size_ = o.size_;
        data_ = new char[size_ + 1];
        memcpy(data_, o.data_, size_ + 1);
        std::cout << "  [拷贝赋值] 释放旧的 + 新分配 + 抄" << std::endl;
        return *this;
    }
    MyString& operator=(MyString&& o) noexcept {                      // 移动赋值
        if (this == &o) { return *this; }
        delete[] data_;                                                // 释放自己的
        data_ = o.data_;                                               // 偷源的
        size_ = o.size_;
        o.data_ = nullptr;                                             // 掏空源
        o.size_ = 0;
        std::cout << "  [移动赋值] 释放旧的 + 偷指针" << std::endl;
        return *this;
    }
    ~MyString() {
        if (data_) {
            std::cout << "  [析构] 释放 \"" << data_ << "\"" << std::endl;
        } else {
            std::cout << "  [析构] （已被掏空——nullptr，delete[] 安全）" << std::endl;
        }
        delete[] data_;                                                // nullptr 也能安全 delete
    }
    const char* c_str() const { return data_ ? data_ : "(空)"; }
private:
    char* data_;
    size_t size_;
};

int main() {
    std::cout << "== 计划指定：给 MyString 补移动语义 ==" << std::endl;
    std::cout << "语句：MyString a(\"hello\");" << std::endl;
    MyString a("hello");
    std::cout << "语句：MyString b = a;（拷贝构造——左值源）" << std::endl;
    { MyString b = a; (void)b; }
    std::cout << "语句：MyString c = std::move(a);（移动构造——右值源）" << std::endl;
    { MyString c = std::move(a); (void)c; }
    std::cout << "  （a 被掏空——析构时 delete nullptr 安全）" << std::endl;

    std::cout << "\n== 移动赋值 ==" << std::endl;
    MyString x("old");
    MyString y("new");
    std::cout << "语句：x = std::move(y);" << std::endl;
    x = std::move(y);
    std::cout << "  x 现在是 \"" << x.c_str() << "\"（偷来了）" << std::endl;

    std::cout << "\n== vector 的扩容搬家：移动版 ==" << std::endl;
    std::vector<MyString> v;
    v.reserve(2);                                     // 先留 2——看第 3 个触发搬家
    std::cout << "push 三个（第 3 个触发扩容搬家）：" << std::endl;
    MyString s1("aaa"), s2("bbb"), s3("ccc");
    v.push_back(std::move(s1));
    v.push_back(std::move(s2));
    v.push_back(std::move(s3));                       // 搬家时旧元素怎么搬？
    std::cout << "  （移动版搬家：旧元素偷指针进新家——vs 拷贝版逐字节重抄）" << std::endl;
    std::cout << "  这就是 Day 79 说'扩容用移动'的落地" << std::endl;

    std::cout << "\n== noexcept 的重要性 ==" << std::endl;
    std::cout << "  移动构造标 noexcept 的原因：" << std::endl;
    std::cout << "  vector 扩容时用移动搬家有个前提——移动不能抛异常" << std::endl;
    std::cout << "  （搬一半抛了：新家半满 + 旧家已掏——数据丢！）" << std::endl;
    std::cout << "  没 noexcept 的移动 → vector 退拷贝搬家（保安全但慢）" << std::endl;
    std::cout << "  （本机 libc++ 的行为：noexcept 移动 → 走移动搬家 ✔）" << std::endl;
    return 0;
}
