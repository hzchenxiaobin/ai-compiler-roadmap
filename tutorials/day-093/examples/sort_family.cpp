#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

// Day 93：sort 家族——sort/stable_sort 与多字段比较器（计划指定：分数降序、同分按姓名）
struct Student {
    std::string name;
    int score;
};

void dump(const char* label, const std::vector<Student>& ss) {
    std::cout << "  " << label << "：";
    for (const auto& s : ss) { std::cout << s.name << "(" << s.score << ") "; }
    std::cout << std::endl;
}

int main() {
    std::vector<Student> students = {
        {"Charlie", 85}, {"Alice", 92}, {"Bob", 85}, {"David", 78}, {"Eve", 92},
    };

    std::cout << "== 原始（插入序） ==" << std::endl;
    dump("原序", students);

    std::cout << "\n== std::sort + 多字段比较器（计划指定练习） ==" << std::endl;
    // 主裁：分数降序；副裁：同分按姓名升序（Day 90 的两级裁判正式版）
    std::sort(students.begin(), students.end(),
              [](const Student& a, const Student& b) {
                  if (a.score != b.score) { return a.score > b.score; }   // 主裁
                  return a.name < b.name;                                   // 副裁
              });
    dump("排序后", students);
    std::cout << "  （92 的 Alice<Eve ✔、85 的 Bob<Charlie ✔——榜面确定）" << std::endl;

    std::cout << "\n== stable_sort：保相对序（同分的保持原顺序） ==" << std::endl;
    std::vector<Student> s2 = {
        {"Charlie", 85}, {"Alice", 92}, {"Bob", 85}, {"David", 78}, {"Eve", 92},
    };
    std::stable_sort(s2.begin(), s2.end(),
                     [](const Student& a, const Student& b) { return a.score > b.score; });  // 只比分数
    dump("stable_sort 后", s2);
    std::cout << "  （85 的 Charlie 仍在 Bob 前——同分保持原相对序）" << std::endl;

    std::cout << "\n== 对照：std::sort 只比分数（不保证同分顺序） ==" << std::endl;
    std::vector<Student> s3 = {
        {"Charlie", 85}, {"Alice", 92}, {"Bob", 85}, {"David", 78}, {"Eve", 92},
    };
    std::sort(s3.begin(), s3.end(),
              [](const Student& a, const Student& b) { return a.score > b.score; });
    dump("sort 后", s3);
    std::cout << "  （本机这次 Charlie 也在 Bob 前——但不保证！换数据/编译器可能变）" << std::endl;

    std::cout << "\n== is_sorted：验证工具 ==" << std::endl;
    std::cout << "  students 已按(分数↓,姓名↑)排序？"
              << (std::is_sorted(students.begin(), students.end(),
                  [](const Student& a, const Student& b) {
                      if (a.score != b.score) { return a.score > b.score; }
                      return a.name < b.name;
                  }) ? "是 ✔" : "否") << std::endl;
    return 0;
}
