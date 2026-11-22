#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Day 125：《Effective Modern C++》条款 1~14 重读——用代码验证经典条款
// 本教程五个月的实践就是这 14 条的“实验记录”——今天集中验证几个代表性条款

// ===== 条款 21（代表验证）：不使用裸 new/delete 的好处 =====
// （本教程 Day 120 已经全量做过——今天一个最小对照）
namespace raw_era {
int* make_arr(int n) { return new int[n](); }
void use_and_del(int* p) { delete[] p; }
}
namespace modern_era {
auto make_arr(int n) { return std::make_unique<int[]>(n); }
// 没有 delete——出作用域自动
}

// ===== 条款 25-29 区域的 use_count 观察（关联条款 19）=====
void show_count(const char* label, const std::shared_ptr<int>& p) {
    std::cout << "  " << label << "：use_count = " << p.use_count() << std::endl;
}

int main() {
    std::cout << "== 五个月实践对条款 1~14 的覆盖图 ==" << std::endl;
    std::cout << "  条款 1-6（类型推导/auto）：Day 103 已学 ✔" << std::endl;
    std::cout << "  条款 7-14（转向现代 C++）：Day 104~112 穿插已学 ✔" << std::endl;
    std::cout << "  （重读的“读法”：每条问“教程哪天验证过它”）" << std::endl;

    std::cout << "\n== 条款 21 验证：make_unique vs 裸 new ==" << std::endl;
    {
        auto arr = modern_era::make_arr(100);
        arr[42] = 7;
        std::cout << "  modern：arr[42] = " << arr[42] << "（零 delete ✔）" << std::endl;
    }
    {
        int* p = raw_era::make_arr(100);
        raw_era::use_and_del(p);
        std::cout << "  raw：用完必须手动删（忘了=泄漏——Day 120 已全量复查过）" << std::endl;
    }

    std::cout << "\n== 条款 21 验证：make 系的异常安全 ==" << std::endl;
    std::cout << "  foo(unique_ptr<T>(new T), bar()) 求值序坑（C++17 前可泄漏）" << std::endl;
    std::cout << "  foo(make_unique<T>(), bar())——不可漏 ✔" << std::endl;
    std::cout << "  （Day 118 学过理论——今天的重读是“又见”）" << std::endl;

    std::cout << "\n== 条款 19 验证：shared 的计数管理 ==" << std::endl;
    auto sp = std::make_shared<int>(42);
    show_count("创建后", sp);
    {
        auto copy = sp;
        show_count("拷贝后", sp);
    }
    show_count("块结束", sp);
    std::cout << "  （Day 116 学过——重读时看的是“当时的疑问解了吗”）" << std::endl;

    std::cout << "\n== 重读的方法论 ==" << std::endl;
    std::cout << "  第一遍（穿插读）：跟着天数学——当天用啥读啥" << std::endl;
    std::cout << "  第二遍（今天）：带着五个月的伤疤读——每条都有画面" << std::endl;
    std::cout << "  读法：条款 → 找教程对应天 → 重跑那个实验 → 写一句新体会" << std::endl;
    std::cout << "  （“读过的书”和“用过的书”是两本书）" << std::endl;
    return 0;
}
