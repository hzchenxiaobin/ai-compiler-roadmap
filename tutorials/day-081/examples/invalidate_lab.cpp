#include <iostream>
#include <vector>

// Day 81：迭代器失效——扩容与 erase 的悬空事故（计划指定：复现并修正）
int main() {
    std::cout << "== 事故一：扩容使全部迭代器失效 ==" << std::endl;
    std::vector<int> v = {1, 2, 3};
    int* raw = v.data();                      // 记录旧内存地址
    auto it = v.begin(); (void)it;   // 记录旧位置（悬空证据用）
    std::cout << "  扩容前：data = " << raw << std::endl;
    v.push_back(4);                           // 触发扩容（初始 capacity=3：翻倍到 6 并搬家）
    v.push_back(5);                           // capacity 已是 6：落在余量内、不再扩
    std::cout << "  扩容后：data = " << v.data()
              << (v.data() != raw ? "（搬家了！旧地址全部作废）" : "（没搬——还没触发）") << std::endl;
    // *it = 99;  // ✘ 取消注释（ASan 版）：heap-use-after-free——旧内存已还
    std::cout << "  （旧迭代器 it 仍存着旧地址——用它就是悬空访问）" << std::endl;

    std::cout << "\n== 事故二：erase 使被删位置及之后的失效 ==" << std::endl;
    std::vector<int> w = {10, 20, 30, 40, 50};
    auto it2 = w.begin() + 2;                 // 指向 30
    std::cout << "  删除前：*it2 = " << *it2 << std::endl;
    w.erase(w.begin() + 1);                   // 删 20——30 前移
    std::cout << "  删除 20 后：*it2 = " << *it2
              << "（值变了——元素前移填坑）" << std::endl;
    std::cout << "  w 现在是：";
    for (int x : w) { std::cout << x << ' '; }
    std::cout << std::endl;

    std::cout << "\n== 事故三：边遍历边 erase 的经典死循环 ==" << std::endl;
    std::vector<int> u = {1, 2, 3, 4};
    // 坏例（注释着）：跳过元素 + 末尾越界
    // for (auto it3 = u.begin(); it3 != u.end(); ++it3) {
    //     if (*it3 % 2 == 0) { u.erase(it3); }   // 删偶数——但 ++it3 跳过了下一个！
    // }
    std::cout << "  坏例行为：删 2 后 ++ 跳到 4——4 被删后 ++ 越过 end——UB！" << std::endl;
    std::cout << "  （偶数只删到一半 + 末尾越界读——两个坑一次踩）" << std::endl;

    std::cout << "\n== 修正：erase 返回下一个有效迭代器 ==" << std::endl;
    std::vector<int> s = {1, 2, 3, 4, 5, 6};
    for (auto it4 = s.begin(); it4 != s.end(); /* 不 ++——由 erase 推进 */) {
        if (*it4 % 2 == 0) {
            it4 = s.erase(it4);                // erase 返回"被删位置的下一个"——已推进！
        } else {
            ++it4;                             // 没删才 ++
        }
    }
    std::cout << "  删偶数后：";
    for (int x : s) { std::cout << x << ' '; }
    std::cout << "（1 3 5 ✔——正确且安全）" << std::endl;
    return 0;
}
