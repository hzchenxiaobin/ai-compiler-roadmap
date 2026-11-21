#include <iostream>
#include <list>
#include <string>
#include <vector>

// 迭代器是"所有容器的通用语言"——vector 和 list 用同一套循环
int main() {
    std::vector<int> vec = {1, 2, 3, 4, 5};
    std::list<int> lst = {1, 2, 3, 4, 5};

    std::cout << "== 同一段循环，两种容器 ==" << std::endl;
    int sum_vec = 0;
    for (auto it = vec.begin(); it != vec.end(); ++it) { sum_vec += *it; }
    int sum_lst = 0;
    for (auto it = lst.begin(); it != lst.end(); ++it) { sum_lst += *it; }
    std::cout << "  vector 求和 = " << sum_vec << "，list 求和 = " << sum_lst
              << "（循环代码一字不改！）" << std::endl;

    std::cout << "\n== 迭代器的能力分层 ==" << std::endl;
    std::cout << "  vector 迭代器：随机访问（it += n / it[i] / end - begin 都行）" << std::endl;
    std::cout << "  list 迭代器：双向（++/-- 可以，+= n 不行——链表不能跳）" << std::endl;
    // auto lit = lst.begin(); lit += 2;   // ✘ 取消注释：编译错误！list 不支持随机跳
    std::cout << "  （list 只能一步步 ++——能力弱但插入删除快）" << std::endl;

    std::cout << "\n== 为什么用 != 而不是 < ==" << std::endl;
    std::cout << "  list 的迭代器是链表节点指针——节点地址不连续，" << std::endl;
    std::cout << "  '<' 没有意义；'!=' 只问\"到头了吗\"——所有容器都答得上来" << std::endl;
    std::cout << "  （习惯：迭代器循环一律用 !=——Day 4 周的老习惯升级）" << std::endl;
    return 0;
}
