#pragma once
#include <vector>
#include "student.h"

// 统计模块的接口
struct Stats {
    double mean;
    double variance;
    int max_score;
    int min_score;
};

Stats compute_stats(const std::vector<Student>& students);
