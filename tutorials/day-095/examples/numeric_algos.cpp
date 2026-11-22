#include <iostream>
#include <numeric>
#include <string>
#include <vector>

// Day 95：数值算法——accumulate/iota/inner_product（计划指定：统计计算小程序）
int main() {
    std::cout << "== accumulate：求和（自定义初值防坑） ==" << std::endl;
    std::vector<int> v = {1, 2, 3, 4, 5};
    int sum = std::accumulate(v.begin(), v.end(), 0);            // 初值 0（int）
    std::cout << "  sum = " << sum << std::endl;
    double avg = (double)sum / v.size();
    std::cout << "  avg = " << avg << std::endl;

    std::vector<double> d = {1.5, 2.5, 3.5};
    double dsum = std::accumulate(d.begin(), d.end(), 0.0);      // 初值 0.0（double！）
    std::cout << "  double 求和 = " << dsum << "（初值 0.0——写 0 会截断成 int！）" << std::endl;
    double wrong = std::accumulate(d.begin(), d.end(), 0);       // 故意写 0
    std::cout << "  初值写 0 的版本 = " << wrong << "（1.5+2.5 被截成 1+2 —— 坑！）" << std::endl;

    std::cout << "\n== accumulate 的第四参：自定义运算 ==" << std::endl;
    int product = std::accumulate(v.begin(), v.end(), 1,                 // 乘积（初值 1）
                                  [](int acc, int x) { return acc * x; });
    std::cout << "  阶乘风格乘积 = " << product << std::endl;
    std::string joined = std::accumulate(v.begin() + 1, v.end(), std::to_string(v[0]),
                                         [](const std::string& acc, int x) {
                                             return acc + "-" + std::to_string(x);
                                         });
    std::cout << "  字符串拼接 = " << joined << "（accumulate 不止数字！）" << std::endl;

    std::cout << "\n== iota：批量编号 ==" << std::endl;
    std::vector<int> idx(5);
    std::iota(idx.begin(), idx.end(), 1);                        // 1,2,3,4,5
    std::cout << "  iota(1)：";
    for (int x : idx) { std::cout << x << ' '; }
    std::cout << std::endl;

    std::cout << "\n== inner_product：点积 ==" << std::endl;
    std::vector<int> a = {1, 2, 3}, w = {10, 20, 30};
    int dot = std::inner_product(a.begin(), a.end(), w.begin(), 0);
    std::cout << "  a·w = " << dot << "（1×10+2×20+3×30——加权求和的原理）" << std::endl;
    std::vector<int> x = {1, 2, 3}, y = {4, 5, 6};
    std::cout << "  x·y = " << std::inner_product(x.begin(), x.end(), y.begin(), 0)
              << "（向量点积）" << std::endl;

    std::cout << "\n== 统计小程序（计划指定）：方差的两算式 ==" << std::endl;
    std::vector<double> scores = {85, 90, 78, 92, 88};
    double n = scores.size();
    double mean = std::accumulate(scores.begin(), scores.end(), 0.0) / n;
    // 方差 = E[(x-mean)^2] = (Σx²)/n - mean²（两种算法对照）
    double var1 = std::accumulate(scores.begin(), scores.end(), 0.0,
                                  [mean](double acc, double s) {
                                      double d = s - mean;
                                      return acc + d * d;
                                  }) / n;
    double var2 = std::inner_product(scores.begin(), scores.end(), scores.begin(), 0.0) / n
                  - mean * mean;
    std::cout << "  均值 = " << mean << std::endl;
    std::cout << "  方差（定义式）= " << var1 << std::endl;
    std::cout << "  方差（E[x²]-mean²）= " << var2 << "（两式一致 ✔）" << std::endl;
    std::cout << "  标准差 ≈ " << 4.88 << "（sqrt 手查——近似值）" << std::endl;
    return 0;
}
