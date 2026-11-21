#include "my_utils.h"

// 机器（定义）也放进同名 namespace：名片和机器同姓
namespace my_utils {

int clamp(int value, int low, int high) {
    if (value < low) {
        return low;
    }
    if (value > high) {
        return high;
    }
    return value;
}

double to_percent(int part, int total) {
    return part * 100.0 / total;                   // Day 3：先乘 100.0 防截断
}

}
