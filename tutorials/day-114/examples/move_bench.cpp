#include <chrono>
#include <iostream>
#include <string>
#include <utility>
#include <vector>

// ===== Day 114 项目日：移动 vs 拷贝性能基准（计划指定 <chrono> 计时） =====
// 一个"重"类型：持有大块堆内存——拷贝要分配+抄、移动只偷指针
class BigData {
public:
    explicit BigData(std::size_t n) : size_(n), data_(new int[n]) {
        for (std::size_t i = 0; i < n; ++i) { data_[i] = static_cast<int>(i); }
    }
    BigData(const BigData& o) : size_(o.size_), data_(new int[o.size_]) {   // 拷贝：分配+抄
        for (std::size_t i = 0; i < size_; ++i) { data_[i] = o.data_[i]; }
    }
    BigData(BigData&& o) noexcept : size_(o.size_), data_(o.data_) {        // 移动：偷指针
        o.data_ = nullptr; o.size_ = 0;
    }
    BigData& operator=(const BigData&) = delete;
    BigData& operator=(BigData&&) = delete;
    ~BigData() { delete[] data_; }
    std::size_t size() const { return size_; }
private:
    std::size_t size_;
    int* data_;
};

template <typename F>
long long bench(const char* label, F f) {
    auto start = std::chrono::steady_clock::now();
    f();
    auto stop = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "  " << label << "：" << ms << " ms" << std::endl;
    return ms;
}

int main() {
    const int N = 2000;                    // 元素个数
    const std::size_t SZ = 100000;         // 每个的数据量（40 万字节 ≈ 0.4MB）

    std::cout << "== 准备：造 " << N << " 个 BigData（各 " << SZ * 4 / 1024 << " KB）==" << std::endl;
    std::vector<BigData> source;
    source.reserve(N);
    for (int i = 0; i < N; ++i) { source.emplace_back(SZ); }
    std::cout << "  共约 " << N * SZ * 4 / 1024 / 1024 << " MB 数据" << std::endl;

    std::cout << "\n== 基准一：整批拷贝 vs 整批移动 ==" << std::endl;
    long long t_copy = bench("拷贝整批（vector 拷贝构造）", [&] {
        std::vector<BigData> copy_of(source);          // 每个元素：new + 逐元素抄
    });
    long long t_move = bench("移动整批（vector 移动构造）", [&] {
        std::vector<BigData> moved(std::move(source)); // 每个元素：偷指针
    });
    std::cout << "  移动/拷贝 = " << (t_copy > 0 ? (double)t_copy / t_move : 0.0) << " 倍快"
              << (t_move == 0 ? "（快到测不出毫秒！）" : "") << std::endl;

    std::cout << "\n== 基准二：逐个 push（拷贝版 vs 移动版） ==" << std::endl;
    std::vector<BigData> src2;
    src2.reserve(N);
    for (int i = 0; i < N; ++i) { src2.emplace_back(SZ); }
    long long t_push_copy = bench("push_back(左值 ×N)（每次拷贝）", [&] {
        std::vector<BigData> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) { v.push_back(src2[i]); }
    });
    long long t_push_move = bench("push_back(move ×N)（每次移动）", [&] {
        std::vector<BigData> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) { v.push_back(std::move(src2[i])); }
    });
    std::cout << "  移动/拷贝 = " << (t_push_copy > 0 ? (double)t_push_copy / t_push_move : 0.0) << " 倍快" << std::endl;

    std::cout << "\n== 基准三：扩容搬家（拷贝元素 vs 移动元素） ==" << std::endl;
    bench("扩容·拷贝搬家（不 reserve）", [&] {
        std::vector<BigData> v;
        for (int i = 0; i < N; ++i) { v.push_back(BigData(SZ)); }
    });
    bench("扩容·reserve 后（免搬家）", [&] {
        std::vector<BigData> v;
        v.reserve(N);
        for (int i = 0; i < N; ++i) { v.push_back(BigData(SZ)); }
    });
    std::cout << "  （Day 79 的 reserve 建议 + Day 111 的移动搬家——数字兑现）" << std::endl;

    std::cout << "\n== 结论区 ==" << std::endl;
    std::cout << "  移动的优势来自'不干活'：偷指针（两个机器字）替代分配+逐字节抄" << std::endl;
    std::cout << "  数据越大越明显（0.4MB×2000 的量级差距最大）" << std::endl;
    std::cout << "  小对象（int 等）拷贝=移动——优化无从谈起" << std::endl;
    return 0;
}
