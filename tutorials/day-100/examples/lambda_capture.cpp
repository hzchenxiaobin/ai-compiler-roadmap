#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Day 100：lambda 深入——10 个捕获小场景（计划指定）
int g_counter = 100;                     // 全局变量（不需要捕获——直接可见）

int main() {
    int x = 10;
    int y = 20;
    std::string name = "hero";

    std::cout << "== 场景 1：不捕获 [] ==" << std::endl;
    auto no_cap = [] { return g_counter; };         // 全局不用捕获
    std::cout << "  [] 访问全局 = " << no_cap() << std::endl;

    std::cout << "\n== 场景 2：值捕获 [x] ==" << std::endl;
    auto by_val = [x] { return x; };
    x = 999;                                          // 捕获之后改原变量
    std::cout << "  捕获后 x 改 999，lambda 里仍是 " << by_val()
              << "（拷贝快照）" << std::endl;

    std::cout << "\n== 场景 3：引用捕获 [&x] ==" << std::endl;
    x = 10;                                           // 恢复
    auto by_ref = [&x] { return x; };
    x = 555;
    std::cout << "  捕获后 x 改 555，lambda 里是 " << by_ref()
              << "（跟着变——是别名）" << std::endl;

    std::cout << "\n== 场景 4：混合捕获 [x, &y] ==" << std::endl;
    auto mixed = [x, &y] { y += x; return y; };      // x 拷贝、y 引用
    std::cout << "  调用前 y = " << y << "；mixed() = " << mixed()
              << "；调用后 y = " << y << "（y 被改了！）" << std::endl;

    std::cout << "\n== 场景 5：全捕获 [&] 和 [=] ==" << std::endl;
    y = 20;
    auto all_ref = [&] { return x + y; };            // 全部引用
    auto all_val = [=] { return x + y; };            // 全部值拷贝
    x = 100; y = 200;
    std::cout << "  [&] 版看到 x+y = " << all_ref() << "（最新值）" << std::endl;
    std::cout << "  [=] 版看到 x+y = " << all_val() << "（捕获时的快照）" << std::endl;

    std::cout << "\n== 场景 6：mutable——值捕获也能改（改的是拷贝） ==" << std::endl;
    auto counter = [n = 0]() mutable { return ++n; };   // 初始化捕获（C++14）
    counter(); counter();
    std::cout << "  第三次调用 counter() = " << counter() << "（内部 n 在涨）" << std::endl;

    std::cout << "\n== 场景 7：初始化捕获 [n = x + 1] ==" << std::endl;
    auto init_cap = [sum = x + y] { return sum; };   // 表达式进捕获
    std::cout << "  [sum = x + y] 捕获时算好 = " << init_cap()
              << "（捕获时刻 100+200）" << std::endl;

    std::cout << "\n== 场景 8：捕获 this（成员场景的模拟） ==" << std::endl;
    struct Game {
        int score = 50;
        auto get_booster() {
            return [this] { return score * 2; };      // 捕获 this——看成员
        }
    };
    Game g;
    auto booster = g.get_booster();
    g.score = 77;
    std::cout << "  g.score 改 77 后 booster() = " << booster()
              << "（this 是指针——跟着对象变）" << std::endl;

    std::cout << "\n== 场景 9：悬空陷阱——引用捕获局部变量的 lambda 逃逸 ==" << std::endl;
    auto bad = [] {
        // int local = 42;  return [&local]{ return local; };  // ✘ 悬空引用（UB）——注释保平安
        return std::string("（正确写法：[local] 值捕获——拷贝活下来）");
    }();
    std::cout << "  " << bad << std::endl;

    std::cout << "\n== 场景 10：容器算法里的捕获（本周的日常） ==" << std::endl;
    std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8};
    int threshold = 4;
    int above = 0;
    std::for_each(v.begin(), v.end(), [&above, threshold](int n) {
        if (n > threshold) { ++above; }               // above 引用（累计）、threshold 值（只读）
    });
    std::cout << "  >" << threshold << " 的有 " << above << " 个（引用累计 + 值只读的标准组合）" << std::endl;
    return 0;
}
