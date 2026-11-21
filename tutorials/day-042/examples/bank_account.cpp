#include <iostream>
#include <string>
#include <vector>

// ===== Day 42 项目日：银行账户类（第 7 周毕业考） =====
// 组合清单：封装（private 余额）＋构造（开户即有名有款）＋const 查询
//　　　　　＋static 利率（全类一份）＋mutable 日志计数＋vector 操作日志（组合）
class BankAccount {
private:
    std::string owner_;
    double balance_;
    std::vector<std::string> log_;        // 组合：操作日志（Day 41 的拼装术）
    static double interest_rate_;         // static：利率全类共享一份
    mutable int query_count_;             // mutable：查询计数（观察行为不变）

    void record(const std::string& entry) {   // 私有工具：记账
        log_.push_back(owner_ + "：" + entry);
    }

public:
    BankAccount(const std::string& owner, double opening)
        : owner_(owner), balance_(opening >= 0 ? opening : 0), query_count_(0) {
        record("开户，存入 " + std::to_string(opening));
    }

    bool deposit(double amount) {
        if (amount <= 0) {
            return false;                  // 非法金额拒收（不变量保护）
        }
        balance_ += amount;
        record("存入 " + std::to_string(amount) + "，余额 " + std::to_string(balance_));
        return true;
    }

    bool withdraw(double amount) {
        if (amount <= 0 || amount > balance_) {
            return false;                  // 透支拦截
        }
        balance_ -= amount;
        record("取出 " + std::to_string(amount) + "，余额 " + std::to_string(balance_));
        return true;
    }

    double balance() const {               // const 查询：不改账户
        ++query_count_;                    // mutable：查询计数照走
        return balance_;
    }

    void print_log() const {               // 日志也是只读查询
        std::cout << "  操作日志（共 " << log_.size() << " 条）：" << std::endl;
        for (std::size_t i = 0; i < log_.size(); ++i) {
            std::cout << "    " << (i + 1) << ". " << log_[i] << std::endl;
        }
        std::cout << "  （本账户被查询过 " << query_count_ << " 次）" << std::endl;
    }

    static double interest_rate() { return interest_rate_; }
    static void set_interest_rate(double r) { interest_rate_ = r; }
};

double BankAccount::interest_rate_ = 0.03;  // 类外定义：初始年利率 3%

int main() {
    std::cout << "当前全行利率 = " << BankAccount::interest_rate() * 100 << "%" << std::endl;

    BankAccount a("小明", 1000);
    BankAccount b("银行", 100000);

    a.deposit(500);
    a.withdraw(200);
    a.withdraw(99999);                     // 透支，被拦
    a.deposit(-50);                        // 负数，被拦

    std::cout << "\n小明余额 = " << a.balance() << std::endl;
    std::cout << "银行余额 = " << b.balance() << std::endl;

    BankAccount::set_interest_rate(0.035); // 调整利率（全类生效）
    std::cout << "\n调整后全行利率 = " << BankAccount::interest_rate() * 100 << "%" << std::endl;

    std::cout << "\n小明的账户档案：" << std::endl;
    a.print_log();
    return 0;
}
