#include <iostream>
#include <vector>

// Day 80：迭代器——begin/end 与四种遍历方式对比（计划指定练习）
int main() {
    std::vector<int> v = {10, 20, 30, 40, 50};

    std::cout << "== 方式一：下标遍历 ==" << std::endl;
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << "  v[" << i << "] = " << v[i] << std::endl;
    }

    std::cout << "\n== 方式二：迭代器遍历 ==" << std::endl;
    for (auto it = v.begin(); it != v.end(); ++it) {
        std::cout << "  *it = " << *it << "（迭代器像指针：* 解引用）" << std::endl;
    }

    std::cout << "\n== 方式三：范围 for（迭代器的语法糖） ==" << std::endl;
    for (int x : v) {
        std::cout << "  x = " << x << std::endl;
    }

    std::cout << "\n== 方式四：const_iterator（只读遍历） ==" << std::endl;
    for (auto it = v.cbegin(); it != v.cend(); ++it) {
        std::cout << "  *it = " << *it << "（cbegin/cend——只读通道）" << std::endl;
    }

    std::cout << "\n== begin/end 的身份 ==" << std::endl;
    std::cout << "  v.begin() 指向首元素：" << *v.begin() << std::endl;
    std::cout << "  v.end() 指向尾后（不 deref！）" << std::endl;
    // *v.end();  // ✘ 取消注释：UB——end 是"过尾"哨兵
    std::cout << "  end - begin = " << v.end() - v.begin() << "（== size）" << std::endl;
    std::cout << "  最后一个元素 = *(end - 1) = " << *(v.end() - 1) << std::endl;

    std::cout << "\n== 迭代器的算术（vector 随机访问） ==" << std::endl;
    auto it = v.begin();
    std::cout << "  it 起点值 = " << *it << std::endl;
    it += 2;                       // 跳两个
    std::cout << "  it += 2 后 = " << *it << std::endl;
    std::cout << "  it[1] = " << it[1] << "（随机访问版下标）" << std::endl;
    std::cout << "  end - it = " << v.end() - it << "（还能走几步）" << std::endl;
    return 0;
}
