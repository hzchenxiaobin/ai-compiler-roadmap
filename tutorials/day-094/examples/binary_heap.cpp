#include <algorithm>
#include <iostream>
#include <vector>

// Day 94：lower_bound/upper_bound 与堆算法——手测二分边界（计划指定）
void dump(const char* label, const std::vector<int>& v) {
    std::cout << "  " << label << "：";
    for (int x : v) { std::cout << x << ' '; }
    std::cout << std::endl;
}

int main() {
    std::vector<int> v = {1, 3, 3, 3, 5, 7, 9};       // 已排序（含重复 3×3）

    std::cout << "== lower_bound：第一个 >= x 的位置 ==" << std::endl;
    dump("数组（下标）", v);
    auto lo = std::lower_bound(v.begin(), v.end(), 3);
    std::cout << "  lower_bound(3) → 下标 " << (lo - v.begin())
              << "（第一个 >= 3 的——3 的首现）" << std::endl;
    auto lo6 = std::lower_bound(v.begin(), v.end(), 6);
    std::cout << "  lower_bound(6) → 下标 " << (lo6 - v.begin())
              << "（6 不存在——第一个 > 6 的位置）" << std::endl;
    auto lo99 = std::lower_bound(v.begin(), v.end(), 99);
    std::cout << "  lower_bound(99) → 下标 " << (lo99 - v.begin())
              << " == end()（都小——插尾部）" << std::endl;

    std::cout << "\n== upper_bound：第一个 > x 的位置 ==" << std::endl;
    auto up = std::upper_bound(v.begin(), v.end(), 3);
    std::cout << "  upper_bound(3) → 下标 " << (up - v.begin())
              << "（3 的最后一个之后——跳过全部 3）" << std::endl;

    std::cout << "\n== 二者的差 = x 的个数 ==" << std::endl;
    std::cout << "  3 的个数 = upper - lower = " << (up - lo) << std::endl;
    auto [l2, u2] = std::equal_range(v.begin(), v.end(), 3);
    std::cout << "  equal_range(3) = [" << (l2 - v.begin()) << ", " << (u2 - v.begin())
              << ")（一次调用拿两个——Day 87 的区间语言）" << std::endl;

    std::cout << "\n== 插入位置的边界手测（计划指定） ==" << std::endl;
    for (int x : {0, 1, 2, 3, 4, 5, 10}) {
        auto pos = std::lower_bound(v.begin(), v.end(), x);
        std::cout << "  lower_bound(" << x << ") → 下标 " << (pos - v.begin())
                  << (pos == v.end() ? "（end——插尾）" : "") << std::endl;
    }

    std::cout << "\n== 堆算法：make/push/pop/sort ==" << std::endl;
    std::vector<int> h = {3, 1, 4, 1, 5, 9, 2, 6};
    dump("原数组", h);
    std::make_heap(h.begin(), h.end());               // 大顶堆
    dump("make_heap 后（首元素=最大）", h);
    std::cout << "  h[0] = " << h[0] << "（堆顶=最大 ✔）" << std::endl;

    h.push_back(10);
    std::push_heap(h.begin(), h.end());               // 上浮新元素
    std::cout << "  push 10 后 h[0] = " << h[0] << "（新最大上浮到顶）" << std::endl;

    std::pop_heap(h.begin(), h.end());                // 堆顶沉底
    int top = h.back();
    h.pop_back();
    std::cout << "  pop_heap → 取出 " << top << "（最大者离场；新堆顶 = " << h[0] << "）" << std::endl;

    std::sort_heap(h.begin(), h.end());               // 堆 → 升序
    dump("sort_heap 后（升序）", h);
    std::cout << "  （priority_queue 的底层就是这套——Day 83 的 5 4 3 1 1 同款）" << std::endl;
    return 0;
}
