#include <iostream>
#include <string>

// Day 59：友元 friend——让外部函数访问私有成员（计划指定练习）
class Account {
private:
    std::string owner_;
    double balance_;

public:
    Account(const std::string& owner, double balance) : owner_(owner), balance_(balance) {}

    // 友元函数：虽然不是成员，却能访问私有成员
    friend void print_private(const Account& a);           // 声明在哪个类里，就友哪个类

    // 友元类：整个类的所有成员函数都是友
    friend class Auditor;

    // 对比：普通成员函数（自己的访问权限天然有）
    void print_public() const {
        std::cout << "  [成员函数] " << owner_ << "：" << balance_ << std::endl;
    }
};

// 友元函数的定义（注意：不在类里，没有 Account:: 前缀）
void print_private(const Account& a) {
    std::cout << "  [友元函数] " << a.owner_ << "：" << a.balance_
              << "（直接摸了 private！）" << std::endl;
}

// 友元类：审计员能看所有账户的私有数据
class Auditor {
public:
    void audit(const Account& a) const {
        std::cout << "  [友元类 Auditor] 审计 " << a.owner_
                  << "：余额 " << a.balance_ << std::endl;
    }
    void compare(const Account& x, const Account& y) const {
        std::cout << "  [友元类 Auditor] " << x.owner_ << "(" << x.balance_ << ") vs "
                  << y.owner_ << "(" << y.balance_ << ")——"
                  << (x.balance_ > y.balance_ ? "前者多" : "后者多或相等") << std::endl;
    }
};

// 非友元的普通函数：只能走 public 接口
void print_via_public(const Account& a) {
    std::cout << "  [普通函数] 经 print_public() 转发：" << std::endl;
    a.print_public();       // 唯一合法通道
    // std::cout << a.balance_;   // ✘ 取消注释：编译错误！private 外人不可见
}

int main() {
    Account a("小明", 1000);
    Account b("小红", 2000);

    std::cout << "== 三种访问身份对照 ==" << std::endl;
    a.print_public();               // 成员函数（自己人）
    print_private(a);               // 友元函数（受邀客人）
    print_via_public(a);            // 普通函数（只能走正门）

    std::cout << "\n== 友元类 ==" << std::endl;
    Auditor aud;
    aud.audit(b);
    aud.compare(a, b);
    return 0;
}
