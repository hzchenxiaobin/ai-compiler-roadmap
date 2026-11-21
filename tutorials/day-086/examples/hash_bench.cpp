#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

// Day 86：unordered_map 哈希原理 + map vs unordered_map 性能对比（计划指定实验）
template <typename MapT>
long long bench(const char* name, int n) {
    auto start = std::chrono::steady_clock::now();
    MapT m;
    for (int i = 0; i < n; ++i) {
        m["key_" + std::to_string(i)] = i;            // 插入
    }
    long long sum = 0;
    for (int i = 0; i < n; ++i) {
        sum += m["key_" + std::to_string(i)];         // 查找
    }
    auto stop = std::chrono::steady_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start).count();
    std::cout << "  " << name << "：" << n << " 个键的插入+全量查找 = "
              << ms << " ms（校验和 " << (sum == (long long)n * (n - 1) / 2 ? "✔" : "✘") << "）" << std::endl;
    return ms;
}

int main() {
    std::cout << "== 哈希原理演示（桶 + 哈希函数） ==" << std::endl;
    std::unordered_map<std::string, int> um;
    um["apple"] = 1;
    um["banana"] = 2;
    um["cherry"] = 3;
    std::cout << "  bucket_count = " << um.bucket_count()
              << "（桶的数量——哈希函数把键映射到桶）" << std::endl;
    std::cout << "  apple 在桶 " << um.bucket("apple")
              << "，banana 在桶 " << um.bucket("banana")
              << "，cherry 在桶 " << um.bucket("cherry") << std::endl;
    std::cout << "  load_factor = " << um.load_factor()
              << "（size/bucket_count——超过阈值自动扩桶）" << std::endl;

    std::cout << "\n== 遍历顺序：无序！ ==" << std::endl;
    std::cout << "  插入序：apple banana cherry" << std::endl;
    std::cout << "  遍历序：";
    for (const auto& [k, v] : um) { std::cout << k << ' '; }
    std::cout << std::endl;
    std::cout << "  （取决于桶位置——与插入序和字典序都无关）" << std::endl;

    std::cout << "\n== 计划指定实验：map vs unordered_map 性能对比 ==" << std::endl;
    int n = 100000;
    long long t1 = bench<std::map<std::string, int>>("map（红黑树）", n);
    long long t2 = bench<std::unordered_map<std::string, int>>("unordered_map（哈希）", n);
    std::cout << "  unordered/map 速度比 = " << (t1 > 0 ? (double)t1 / t2 : 0.0) << " 倍" << std::endl;

    std::cout << "\n== 但有序输出只有 map 能免费给 ==" << std::endl;
    std::map<std::string, int> om = {{"banana", 2}, {"apple", 1}, {"cherry", 3}};
    std::cout << "  map 遍历：";
    for (const auto& [k, v] : om) { std::cout << k << ' '; }
    std::cout << "（字典序 ✔）" << std::endl;
    return 0;
}
