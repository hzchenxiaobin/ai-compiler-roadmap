// json_sanitize —— CMake 配置的消毒柜目标（ASan+UBSan）
#include <iostream>
int main() {
    int arr[3] = {1, 2, 3};
    int sum = 0;
    for (int x : arr) { sum += x; }
    std::cout << "== sanitize 目标运行（ASan+UBSan 已由 CMake 注入）==\n";
    std::cout << "  sum = " << sum << "（无报告=干净）\n";
    return 0;
}
