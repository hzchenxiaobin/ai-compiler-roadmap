#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <utility>

// Day 78 里程碑：四概念脱稿自测的"程序化讲稿"——每个概念配最小可运行证据
// 一、三法则
class Buffer3 {
    char* data_;
public:
    Buffer3(const char* s) { data_ = new char[strlen(s) + 1]; memcpy(data_, s, strlen(s) + 1); }
    Buffer3(const Buffer3& o) {                          // ①拷贝构造
        data_ = new char[strlen(o.data_) + 1]; memcpy(data_, o.data_, strlen(o.data_) + 1);
    }
    Buffer3& operator=(const Buffer3& o) {               // ②拷贝赋值
        if (this == &o) { return *this; }
        delete[] data_;
        data_ = new char[strlen(o.data_) + 1]; memcpy(data_, o.data_, strlen(o.data_) + 1);
        return *this;
    }
    ~Buffer3() { delete[] data_; }                       // ③析构
    const char* c_str() const { return data_; }
};

// 二、虚析构
class BaseV {
public:
    virtual ~BaseV() { std::cout << "  ~BaseV（虚析构：拆到基类层）" << std::endl; }
};
class DerivedV : public BaseV {
public:
    ~DerivedV() { std::cout << "  ~DerivedV（派生先拆）" << std::endl; }
};

// 三、RAII
class FileTag {
    std::string name_;
public:
    explicit FileTag(std::string n) : name_(std::move(n)) {
        std::cout << "  [打开] " << name_ << std::endl;
    }
    ~FileTag() { std::cout << "  [关闭] " << name_ << "（出块自动）" << std::endl; }
};

// 四、深浅拷贝
struct Shallow {
    char* p;
};
class Deep {
    char* p_;
public:
    Deep(const char* s) : p_(new char[strlen(s) + 1]) { memcpy(p_, s, strlen(s) + 1); }
    Deep(const Deep& o) : p_(new char[strlen(o.p_) + 1]) { memcpy(p_, o.p_, strlen(o.p_) + 1); }
    ~Deep() { delete[] p_; }
    const char* c_str() const { return p_; }
};

int main() {
    std::cout << "== 概念一：三法则（手写三件套的类） ==" << std::endl;
    std::cout << "  规则：类管裸资源（new）→ 拷贝构造/拷贝赋值/析构三件一起写" << std::endl;
    Buffer3 b("hello");
    Buffer3 c = b;                                       // 拷贝构造
    std::cout << "  b=" << b.c_str() << " c=" << c.c_str() << "（各自独立）" << std::endl;

    std::cout << "\n== 概念二：虚析构（delete 基类指针） ==" << std::endl;
    std::cout << "  规则：多态基类析构一律 virtual——否则派生类资源漏拆" << std::endl;
    BaseV* p = new DerivedV();
    delete p;                                            // 实测：DerivedV 先拆、BaseV 后拆

    std::cout << "\n== 概念三：RAII（资源获取即初始化） ==" << std::endl;
    std::cout << "  规则：构造拿资源、析构放资源——'忘了释放'从语法上不存在" << std::endl;
    {
        FileTag f("data.txt");
        std::cout << "  （块内使用……）" << std::endl;
    }                                                    // 出块自动关

    std::cout << "\n== 概念四：深浅拷贝（含指针成员的复制） ==" << std::endl;
    Shallow s1; s1.p = new char[3]{'h','i','\0'};
    Shallow s2 = s1;                                     // 浅：两指针一块内存
    std::cout << "  浅拷贝：s1.p == s2.p → " << (s1.p == s2.p ? "同址（共享！）" : "不同") << std::endl;
    Deep d1("hi");
    Deep d2 = d1;                                        // 深：新开内存抄内容
    std::cout << "  深拷贝：d1/d2 各自独立（" << d1.c_str() << "/" << d2.c_str() << "）" << std::endl;
    delete[] s1.p;                                       // 浅版的清理困境：只还一次
    return 0;
}
