#include "stats.h"
#include <numeric>
#include <algorithm>

// 统计模块的实现（本周数值算法的实战）
Stats compute_stats(const std::vector<Student>& students) {
    Stats s{};
    double n = students.size();
    s.mean = std::accumulate(students.begin(), students.end(), 0.0,
                             [](double acc, const Student& st) { return acc + st.score; }) / n;
    s.variance = std::accumulate(students.begin(), students.end(), 0.0,
                                 [s](double acc, const Student& st) {
                                     double d = st.score - s.mean;
                                     return acc + d * d;
                                 }) / n;
    auto [lo, hi] = std::minmax_element(students.begin(), students.end(),
                                        [](const Student& a, const Student& b) { return a.score < b.score; });
    s.max_score = hi->score;
    s.min_score = lo->score;
    return s;
}
