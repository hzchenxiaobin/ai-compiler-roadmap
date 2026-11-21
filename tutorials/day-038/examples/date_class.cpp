#include <iostream>

// 第 38 天主角：Date 类的多种构造方式（计划指定练习）
class Date {
private:
    int year_;
    int month_;
    int day_;

    bool is_valid(int y, int m, int d) const {     // 私有工具：校验日期
        if (y < 1 || m < 1 || m > 12 || d < 1 || d > 31) {
            return false;
        }
        return true;
    }

public:
    // 构造函数一：默认构造——无参，给出安全默认值
    Date() : year_(2026), month_(1), day_(1) {}    // 初始化列表（冒号后面）

    // 构造函数二：三参构造——校验后初始化
    Date(int y, int m, int d) : year_(y), month_(m), day_(d) {
        if (!is_valid(y, m, d)) {                  // 非法日期退回默认（更严格可抛异常，进阶话题）
            year_ = 2026; month_ = 1; day_ = 1;
        }
    }

    // 构造函数三：委托构造——把活转包给三参版本，自己只改年份
    Date(int y) : Date(y, 1, 1) {}

    void print() const {
        std::cout << year_ << "-" << month_ << "-" << day_ << std::endl;
    }
};

int main() {
    Date d1;                   // 调默认构造
    Date d2(2026, 11, 22);     // 调三参构造
    Date d3(2026);             // 调委托构造（先转包三参）
    Date d4(2026, 13, 99);     // 非法日期 → 退回默认

    std::cout << "d1（默认）　：";  d1.print();
    std::cout << "d2（三参）　：";  d2.print();
    std::cout << "d3（委托）　：";  d3.print();
    std::cout << "d4（非法）　：";  d4.print();
    return 0;
}
