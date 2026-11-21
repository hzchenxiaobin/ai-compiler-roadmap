#include <iostream>
#include <string>
#include <vector>

// Day 79：vector 深入——capacity 增长曲线（计划指定练习）
struct Big {
    double payload[4];       // 32 字节——让增长曲线更醒目
    static int constructions;
    static int copies;
    Big() { ++constructions; }
    Big(const Big&) { ++copies; }
};

int Big::constructions = 0;
int Big::copies = 0;

int main() {
    std::cout << "== 曲线一：逐个 push_back（观察 capacity 阶梯） ==" << std::endl;
    std::vector<int> v;
    std::size_t last_cap = 0;
    for (int i = 0; i < 40; ++i) {
        v.push_back(i);
        if (v.capacity() != last_cap) {
            std::cout << "  size=" << v.size() << " 时扩容：capacity "
                      << last_cap << " → " << v.capacity() << std::endl;
            last_cap = v.capacity();
        }
    }
    std::cout << "  最终：size=" << v.size() << "，capacity=" << v.capacity() << std::endl;

    std::cout << "\n== 曲线二：reserve 先手（一次到位） ==" << std::endl;
    std::vector<int> w;
    w.reserve(64);
    std::cout << "  reserve(64) 后：capacity=" << w.capacity() << std::endl;
    for (int i = 0; i < 40; ++i) { w.push_back(i); }
    std::cout << "  push 40 个后：size=" << w.size() << "，capacity=" << w.capacity()
              << "（全程零扩容日志！）" << std::endl;

    std::cout << "\n== 曲线三：emplace_back vs push_back（构造记账） ==" << std::endl;
    {
        std::vector<Big> a;
        a.reserve(10);                     // 排除扩容搬运的干扰
        a.push_back(Big());                // 先构造临时 + 拷贝进容器
        std::cout << "  push_back 1 个：构造 " << Big::constructions
                  << " 次，拷贝 " << Big::copies << " 次" << std::endl;
    }
    {
        std::vector<Big> b;
        b.reserve(10);
        b.emplace_back();                  // 原地构造——零拷贝
        std::cout << "  emplace_back 1 个：累计构造 " << Big::constructions
                  << " 次，累计拷贝 " << Big::copies << " 次（没增加！）" << std::endl;
    }

    std::cout << "\n== 与手写 v2 的对照 ==" << std::endl;
    std::cout << "  libc++ 的增长因子：0→1→2→4→8…（起步 1，倍增）" << std::endl;
    std::cout << "  手写 v2（Day 47）：4→8→16→32→64（起步 4，倍增）" << std::endl;
    std::cout << "  同款摊还魔法：偶发搬迁被平常廉价 push 摊平——平均 O(1)" << std::endl;
    return 0;
}
