// Day 139：性能测试与优化——string_view、预留空间、减少拷贝（计划指定）
// 基准：对同一份生成的 JSON（约 1MB）做 parse——对比优化前后
#include <chrono>
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>
#include <functional>
#include <cstdlib>
#include <iomanip>

namespace json_bench {

// ===== 三个版本的 tokenizer（只比数字/字符串扫描段——其余同 Day 138） =====

// 版本 A：朴素版——substr 造子串（拷贝）+ 不预留
struct TokensA {
    std::vector<std::pair<int, std::string>> toks;
};

// 版本 B：string_view（零拷贝的词段）
#include <string_view>
struct TokensB {
    std::vector<std::pair<int, std::string_view>> toks;
};

// 生成测试数据：N 个键值对的对象（{"k0":"v0","k1":"v1",...}）
std::string make_input(int n) {
    std::string s = "{";
    for (int i = 0; i < n; ++i) {
        if (i) s += ',';
        s += "\"key";
        s += std::to_string(i);
        s += "\":\"value";
        s += std::to_string(i * 7);
        s += "\"";
    }
    s += '}';
    return s;
}

// 版本 A：字符串子串拷贝的 tokenizer（只扫到记号列表——测词法成本）
double bench_substr(const std::string& src, int repeat) {
    auto t0 = std::chrono::steady_clock::now();
    volatile std::size_t sink = 0;
    for (int r = 0; r < repeat; ++r) {
        std::vector<std::pair<int, std::string>> toks;
        std::size_t i = 0;
        while (i < src.size()) {
            char c = src[i];
            if (c == '"') {
                std::size_t start = ++i;
                while (i < src.size() && src[i] != '"') ++i;
                toks.emplace_back(6, src.substr(start, i - start));   // ★ substr 拷贝
                ++i;
            } else {
                toks.emplace_back(0, std::string(1, c));
                ++i;
            }
        }
        sink += toks.size();
    }
    auto t1 = std::chrono::steady_clock::now();
    (void)sink;
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

// 版本 B：string_view 版（零拷贝）
double bench_view(const std::string& src, int repeat) {
    auto t0 = std::chrono::steady_clock::now();
    volatile std::size_t sink = 0;
    for (int r = 0; r < repeat; ++r) {
        std::vector<std::pair<int, std::string_view>> toks;
        std::size_t i = 0;
        while (i < src.size()) {
            char c = src[i];
            if (c == '"') {
                std::size_t start = ++i;
                while (i < src.size() && src[i] != '"') ++i;
                toks.emplace_back(6, std::string_view(src).substr(start, i - start));  // ★ 零拷贝
                ++i;
            } else {
                toks.emplace_back(0, std::string_view(&c, 1));
                ++i;
            }
        }
        sink += toks.size();
    }
    auto t1 = std::chrono::steady_clock::now();
    (void)sink;
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

// 版本 B+：string_view + reserve 预留
double bench_view_reserve(const std::string& src, int repeat) {
    auto t0 = std::chrono::steady_clock::now();
    volatile std::size_t sink = 0;
    for (int r = 0; r < repeat; ++r) {
        std::vector<std::pair<int, std::string_view>> toks;
        toks.reserve(src.size() / 6 + 16);      // ★ 预估容量（一次到位免重分配）
        std::size_t i = 0;
        while (i < src.size()) {
            char c = src[i];
            if (c == '"') {
                std::size_t start = ++i;
                while (i < src.size() && src[i] != '"') ++i;
                toks.emplace_back(6, std::string_view(src).substr(start, i - start));
                ++i;
            } else {
                toks.emplace_back(0, std::string_view(&c, 1));
                ++i;
            }
        }
        sink += toks.size();
    }
    auto t1 = std::chrono::steady_clock::now();
    (void)sink;
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

// 版本 C：朴素 token struct（含 string 拷贝）+ reserve —— 分离两个优化的贡献
double bench_substr_reserve(const std::string& src, int repeat) {
    auto t0 = std::chrono::steady_clock::now();
    volatile std::size_t sink = 0;
    for (int r = 0; r < repeat; ++r) {
        std::vector<std::pair<int, std::string>> toks;
        toks.reserve(src.size() / 6 + 16);
        std::size_t i = 0;
        while (i < src.size()) {
            char c = src[i];
            if (c == '"') {
                std::size_t start = ++i;
                while (i < src.size() && src[i] != '"') ++i;
                toks.emplace_back(6, src.substr(start, i - start));
                ++i;
            } else {
                toks.emplace_back(0, std::string(1, c));
                ++i;
            }
        }
        sink += toks.size();
    }
    auto t1 = std::chrono::steady_clock::now();
    (void)sink;
    return std::chrono::duration<double, std::milli>(t1 - t0).count();
}

} // namespace json_bench

int main() {
    std::cout << "Day 139：性能测试与优化\n" << std::endl;

    std::string input = json_bench::make_input(20000);   // 约 0.5MB 的对象
    int repeat = 20;
    std::cout << "输入：" << input.size() << " 字节（20000 键值对）× " << repeat << " 轮\n" << std::endl;

    // 预热
    json_bench::bench_substr(input, 2);

    double t_a = json_bench::bench_substr(input, repeat);
    double t_ar = json_bench::bench_substr_reserve(input, repeat);
    double t_b = json_bench::bench_view(input, repeat);
    double t_br = json_bench::bench_view_reserve(input, repeat);

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "== 词法段的四个版本（毫秒） ==" << std::endl;
    std::cout << "  A  朴素（substr 拷贝）：" << std::setw(8) << t_a << " ms（基线）" << std::endl;
    std::cout << "  A+ A + reserve 预留　：" << std::setw(8) << t_ar << " ms（提速 " << t_a / t_ar << "x）" << std::endl;
    std::cout << "  B  string_view 零拷贝：" << std::setw(8) << t_b << " ms（提速 " << t_a / t_b << "x）" << std::endl;
    std::cout << "  B+ view + reserve　　：" << std::setw(8) << t_br << " ms（提速 " << t_a / t_br << "x）★" << std::endl;

    std::cout << "\n== 优化的分解 ==" << std::endl;
    std::cout << "  reserve 的贡献（A→A+）：" << t_a - t_ar << " ms（省的重组分配）" << std::endl;
    std::cout << "  view 的贡献（A→B）　：" << t_a - t_b << " ms（省的字符串拷贝）" << std::endl;
    std::cout << "  叠加（A→B+）　　　　：" << t_a - t_br << " ms" << std::endl;

    std::cout << "\n== 测量的纪律 ==" << std::endl;
    std::cout << "  预热（缓存/分支预测就位）后再计时" << std::endl;
    std::cout << "  多轮取总时长（平均更稳）" << std::endl;
    std::cout << "  volatile sink 防优化掉（编译器不能删）" << std::endl;
    std::cout << "  （Day 114 的 chrono 方法论——今天加防优化）" << std::endl;

    std::cout << "\n== 三板斧的适用面（诚实注） ==" << std::endl;
    std::cout << "  string_view：词段的生命期在原串内才安全（Day 122 三律）" << std::endl;
    std::cout << "  reserve：知道量级时（src.size()/6 的预估）" << std::endl;
    std::cout << "  减少拷贝：热路径的每次 substr/push_back 都值得审" << std::endl;
    return 0;
}
