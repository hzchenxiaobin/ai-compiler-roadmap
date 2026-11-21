#include <algorithm>
#include <iostream>
#include <iterator>
#include <vector>

// Day 92：修改算法——copy/transform/remove/unique + erase-remove 惯用法（计划指定）
void dump(const char* label, const std::vector<int>& v) {
    std::cout << "  " << label << "：";
    for (int x : v) { std::cout << x << ' '; }
    std::cout << std::endl;
}

int main() {
    std::vector<int> v = {1, 2, 3, 4, 5, 4, 3};

    std::cout << "== copy：拷贝到目标 ==" << std::endl;
    std::vector<int> dest(v.size());
    std::copy(v.begin(), v.end(), dest.begin());
    dump("copy 后 dest", dest);
    std::cout << "  （目标要先有空间——begin() 是输出迭代器 Day 88 的伏笔）" << std::endl;

    std::cout << "\n== back_inserter：自动扩容的拷贝 ==" << std::endl;
    std::vector<int> dest2;                             // 空！
    std::copy(v.begin(), v.end(), std::back_inserter(dest2));
    dump("back_inserter 版", dest2);
    std::cout << "  （每写一个自动 push_back——目标不用预留）" << std::endl;

    std::cout << "\n== transform：边拷边变（一元） ==" << std::endl;
    std::vector<int> doubled;
    std::transform(v.begin(), v.end(), std::back_inserter(doubled),
                   [](int x) { return x * 2; });
    dump("×2 后", doubled);

    std::cout << "\n== transform：二元版（两列运算） ==" << std::endl;
    std::vector<int> a = {1, 2, 3}, b = {10, 20, 30}, sum;
    std::transform(a.begin(), a.end(), b.begin(), std::back_inserter(sum),
                   [](int x, int y) { return x + y; });
    dump("a+b", sum);

    std::cout << "\n== remove：'伪删除'的真相 ==" << std::endl;
    dump("remove 前", v);
    auto new_end = std::remove(v.begin(), v.end(), 4);  // 想删所有的 4
    dump("remove 后（注意尾部！）", v);
    std::cout << "  size 仍是 " << v.size()
              << "（remove 不删元素——只是把'留下的'前移，返回新逻辑尾）" << std::endl;
    std::cout << "  新逻辑尾之后的 [" << (new_end - v.begin()) << ", " << v.size() << ") 是'垃圾区'" << std::endl;

    std::cout << "\n== erase-remove 惯用法（真正删除） ==" << std::endl;
    v.erase(new_end, v.end());                          // 一刀切掉垃圾区
    dump("erase 后", v);
    std::cout << "  size = " << v.size() << "（这才删干净——两步合成惯用法）" << std::endl;
    // 一步版：v.erase(std::remove(v.begin(), v.end(), 4), v.end());

    std::cout << "\n== remove_if + erase：按条件删 ==" << std::endl;
    v.erase(std::remove_if(v.begin(), v.end(), [](int x) { return x % 2 == 0; }), v.end());
    dump("删偶数后", v);

    std::cout << "\n== unique：相邻去重（先排序才全效） ==" << std::endl;
    std::vector<int> u = {1, 1, 2, 2, 2, 3, 1, 1};      // 未排序——1 分离在两处
    dump("原数组", u);
    u.erase(std::unique(u.begin(), u.end()), u.end());  // 只去相邻的
    dump("unique+erase 后", u);
    std::cout << "  （分离的 1 没去——unique 只看相邻！）" << std::endl;
    std::sort(u.begin(), u.end());
    u.erase(std::unique(u.begin(), u.end()), u.end());
    dump("先 sort 再 unique 后", u);
    std::cout << "  （全量去重的正确姿势：sort + unique + erase 三连）" << std::endl;
    return 0;
}
