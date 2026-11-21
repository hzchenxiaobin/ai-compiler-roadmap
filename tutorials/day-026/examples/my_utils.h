#ifndef MY_UTILS_H
#define MY_UTILS_H

// 自己的工具命名空间：名片（声明）放在 namespace 里
namespace my_utils {

int clamp(int value, int low, int high);          // 夹在区间内
double to_percent(int part, int total);           // 化成百分比

}

#endif
