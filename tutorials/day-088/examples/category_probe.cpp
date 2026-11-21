#include <deque>
#include <forward_list>
#include <iostream>
#include <iterator>
#include <type_traits>
#include <list>
#include <map>
#include <vector>

// Day 88：迭代器分类——五类能力的实测对照（计划指定：做一张对照表）
// 用 iterator_traits 打印每个迭代器的"类别标签"
template <typename It>
void show_category(const char* name) {
    using Cat = typename std::iterator_traits<It>::iterator_category;
    if constexpr (std::is_same_v<Cat, std::random_access_iterator_tag>) {
        std::cout << "  " << name << "：随机访问（+= n / it[i] / end-begin 全行）" << std::endl;
    } else if constexpr (std::is_same_v<Cat, std::bidirectional_iterator_tag>) {
        std::cout << "  " << name << "：双向（++ / -- 行；+= n 不行）" << std::endl;
    } else if constexpr (std::is_same_v<Cat, std::forward_iterator_tag>) {
        std::cout << "  " << name << "：前向（只有 ++ 行）" << std::endl;
    } else if constexpr (std::is_same_v<Cat, std::input_iterator_tag>) {
        std::cout << "  " << name << "：输入（单遍扫描）" << std::endl;
    }
}

int main() {
    std::cout << "== 各容器迭代器的类别（编译期检测） ==" << std::endl;
    show_category<std::vector<int>::iterator>("vector");
    show_category<std::deque<int>::iterator>("deque");
    show_category<std::list<int>::iterator>("list");
    show_category<std::map<int, int>::iterator>("map");
    show_category<std::forward_list<int>::iterator>("forward_list");
    show_category<std::istream_iterator<int>>("istream（输入流）");

    std::cout << "\n== 能力阶梯的实测（每级一例） ==" << std::endl;
    std::vector<int> v = {10, 20, 30, 40};
    auto vit = v.begin();
    vit += 2;                                   // 随机访问：跳两格
    std::cout << "  vector：it += 2 → " << *vit
              << "；it[1] = " << vit[1]
              << "；end-begin = " << (v.end() - v.begin()) << std::endl;

    std::list<int> l = {10, 20, 30};
    auto lit = l.begin();
    ++lit; --lit;                               // 双向：来回走
    std::cout << "  list：++ 后 -- 回来 → " << *lit << "（双向 ✔）" << std::endl;
    // lit += 1;  // ✘ 编译错——list 没有随机访问

    std::forward_list<int> fl = {10, 20, 30};
    auto fit = fl.begin();
    ++fit;                                      // 前向：只能 ++
    std::cout << "  forward_list：++ → " << *fit << "（前向 ✔；-- 不可）" << std::endl;
    // --fit;   // ✘ 编译错——单向链不能回头

    std::cout << "\n== 为什么分类：算法的入场券 ==" << std::endl;
    // std::sort 需要随机访问——list 的迭代器进不了场：
    std::cout << "  std::sort 要求随机访问 → vector/deque ✔、list ✘" << std::endl;
    std::cout << "  （list 有自己的成员 sort——l.sort()）" << std::endl;
    std::cout << "  std::advance 万金油：随机访问 O(1)、其他 O(n) 逐走" << std::endl;
    std::advance(lit, 2);                       // list 上走两步（内部逐 ++）
    std::cout << "  list 上 advance(it, 2) → " << *lit << "（逐走到位）" << std::endl;

    std::cout << "\n== 分类总表 ==" << std::endl;
    std::cout << "  输入      ：istream_iterator（单遍读）" << std::endl;
    std::cout << "  前向      ：forward_list（++；多遍可）" << std::endl;
    std::cout << "  双向      ：list/map/set（++/--）" << std::endl;
    std::cout << "  随机访问  ：vector/deque/array/string（+=n/[]/差值）" << std::endl;
    std::cout << "  （输出迭代器：back_inserter 等——第 16 周算法课见）" << std::endl;
    return 0;
}
