#include <iostream>

// 题一：两数之和（LeetCode #1 简单版）
// 给定数组和目标值，返回和为目标值的两个下标（保证恰有一解）
void two_sum(const int nums[], int n, int target, int& out_i, int& out_j) {
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {        // j 从 i+1 起：不重复使用同一元素
            if (nums[i] + nums[j] == target) {
                out_i = i;
                out_j = j;
                return;
            }
        }
    }
}

int main() {
    int nums[6] = {2, 7, 11, 15, 3, 6};
    int i = -1, j = -1;
    two_sum(nums, 6, 9, i, j);
    std::cout << "target=9　→ nums[" << i << "]+nums[" << j
              << "] = " << nums[i] << "+" << nums[j] << " = 9" << std::endl;
    two_sum(nums, 6, 21, i, j);
    std::cout << "target=21　→ nums[" << i << "]+nums[" << j
              << "] = " << nums[i] << "+" << nums[j] << " = 21" << std::endl;
    return 0;
}
