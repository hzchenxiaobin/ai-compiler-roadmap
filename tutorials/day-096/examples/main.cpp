#include <algorithm>
#include <iostream>
#include <numeric>
#include <vector>
#include "student.h"
#include "stats.h"

// 学生成绩管理系统——容器+算法综合运用（计划指定项目）
void dump(const char* label, const std::vector<Student>& ss) {
    std::cout << "  " << label << "：";
    for (const auto& s : ss) { std::cout << s.name << "(" << s.score << ") "; }
    std::cout << std::endl;
}

int main() {
    std::vector<Student> students = {
        {"Alice", 92}, {"Bob", 78}, {"Charlie", 85}, {"David", 92},
        {"Eve", 66}, {"Frank", 85}, {"Grace", 95}, {"Heidi", 74},
    };
    dump("原始", students);

    std::cout << "\n== 排序（两级裁判：分数降序、同分姓名升序） ==" << std::endl;
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  if (a.score != b.score) { return a.score > b.score; }
                  return a.name < b.name;
              });
    dump("排序后", students);

    std::cout << "\n== 查找（find_if） ==" << std::endl;
    auto first_pass = std::find_if(students.begin(), students.end(),
                                   [](const Student& s) { return s.score >= 60; });
    std::cout << "  第一个及格的：" << first_pass->name
              << "（" << first_pass->score << "）" << std::endl;

    std::cout << "\n== 分段统计（count_if + 谓词） ==" << std::endl;
    int excellent = std::count_if(students.begin(), students.end(),
                                  [](const Student& s) { return s.score >= 90; });
    int failed = std::count_if(students.begin(), students.end(),
                               [](const Student& s) { return s.score < 60; });
    std::cout << "  优秀(>=90)：" << excellent << " 人；不及格(<60)：" << failed << " 人" << std::endl;

    std::cout << "\n== 统计模块（stats.h/cpp 的数值算法） ==" << std::endl;
    Stats st = compute_stats(students);
    std::cout << "  均值 = " << st.mean << "；方差 = " << st.variance << std::endl;
    std::cout << "  最高 = " << st.max_score << "；最低 = " << st.min_score << std::endl;

    std::cout << "\n== 筛选及格名单（copy_if + back_inserter） ==" << std::endl;
    std::vector<Student> passed;
    std::copy_if(students.begin(), students.end(), std::back_inserter(passed),
                 [](const Student& s) { return s.score >= 60; });
    std::cout << "  及格 " << passed.size() << " 人：";
    for (const auto& s : passed) { std::cout << s.name << ' '; }
    std::cout << std::endl;

    std::cout << "\n== 二分定位（lower_bound 有序前提） ==" << std::endl;
    // students 已按分数降序——用反向比较器做二分
    auto pos = std::lower_bound(students.begin(), students.end(), 85,
                                [](const Student& s, int val) { return s.score > val; });
    std::cout << "  第一个 <=85 分的位置：下标 " << (pos - students.begin())
              << "（" << pos->name << "——含边界）" << std::endl;

    std::cout << "\n== 加权排名积分（inner_product：前 3 名加权） ==" << std::endl;
    std::vector<int> top3_scores;
    for (int i = 0; i < 3; ++i) { top3_scores.push_back(students[i].score); }
    std::vector<int> weights = {5, 3, 2};
    int points = std::inner_product(top3_scores.begin(), top3_scores.end(), weights.begin(), 0);
    std::cout << "  前 3 名加权积分 = " << points << "（5/3/2 权重）" << std::endl;
    return 0;
}
