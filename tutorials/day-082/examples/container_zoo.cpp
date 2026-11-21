#include <array>
#include <iterator>
#include <deque>
#include <forward_list>
#include <iostream>
#include <list>
#include <vector>

// Day 82：list/deque/array/forward_list 与 vector 的增删改查对比（计划指定练习）
int main() {
    std::cout << "== 五容器的“查”（访问方式对比） ==" << std::endl;
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {1, 2, 3, 4, 5};
    std::deque<int> deq = {1, 2, 3, 4, 5};
    std::array<int, 5> arr = {1, 2, 3, 4, 5};
    std::forward_list<int> flst = {1, 2, 3, 4, 5};

    std::cout << "  vector[2] = " << vec[2] << "（随机访问 O(1)）" << std::endl;
    std::cout << "  deque[2]  = " << deq[2] << "（随机访问 O(1)——分段连续）" << std::endl;
    std::cout << "  array[2]  = " << arr[2] << "（随机访问 O(1)——栈上定长）" << std::endl;
    // lst[2]  ✘ list 没有 operator[]（链表跳不动）
    // flst[2] ✘ forward_list 更没有
    auto lit = lst.begin();
    std::advance(lit, 2);                       // 一步步走两格
    std::cout << "  list 走两步 = " << *lit << "（O(n)——advance 逐个 ++）" << std::endl;

    std::cout << "\n== 五容器的“增”（插入位置对比） ==" << std::endl;
    vec.insert(vec.begin() + 2, 99);             // vector 中插：后面全搬
    std::cout << "  vector 中插 99：";
    for (int x : vec) { std::cout << x << ' '; }
    std::cout << "（O(n)：3 4 5 都后移了）" << std::endl;

    lst.insert(std::next(lst.begin(), 2), 99);   // list 中插：只改指针
    std::cout << "  list   中插 99：";
    for (int x : lst) { std::cout << x << ' '; }
    std::cout << "（O(1)：邻节点改指向）" << std::endl;

    deq.push_front(0);                          // deque 双头：头插 O(1)
    deq.push_back(6);
    std::cout << "  deque  头插 0 尾插 6：";
    for (int x : deq) { std::cout << x << ' '; }
    std::cout << "（两端 O(1)——分段映射）" << std::endl;

    flst.push_front(0);                         // forward_list 只有头
    std::cout << "  forward_list 头插 0：";
    for (int x : flst) { std::cout << x << ' '; }
    std::cout << "（单向：省一个指针的空间）" << std::endl;
    // arr 不能增删——定长！

    std::cout << "\n== 五容器的“删” ==" << std::endl;
    vec.erase(vec.begin() + 2);
    std::cout << "  vector 删中：O(n)（后面前移填坑——Day 81 的事故二）" << std::endl;
    lst.erase(std::next(lst.begin(), 2));
    std::cout << "  list   删中：O(1)（摘节点——只有它失效）" << std::endl;
    deq.pop_front();
    std::cout << "  deque  删头：O(1)（两端快）" << std::endl;
    flst.pop_front();
    std::cout << "  forward_list 删头：O(1)" << std::endl;
    std::cout << "  array：不能删（定长——出生即固定）" << std::endl;

    std::cout << "\n== sizeof 的结构线索 ==" << std::endl;
    std::cout << "  sizeof(vector) = " << sizeof(std::vector<int>)
              << "（指针+size+cap 三件套）" << std::endl;
    std::cout << "  sizeof(list)   = " << sizeof(std::list<int>)
              << "（头节点把手）" << std::endl;
    std::cout << "  sizeof(deque)  = " << sizeof(std::deque<int>)
              << "（中控映射表）" << std::endl;
    std::cout << "  sizeof(array)  = " << sizeof(std::array<int, 5>)
              << "（= 5 个 int 本体！无元数据）" << std::endl;
    std::cout << "  sizeof(forward_list) = " << sizeof(std::forward_list<int>)
              << "（最轻的头把手）" << std::endl;
    return 0;
}
