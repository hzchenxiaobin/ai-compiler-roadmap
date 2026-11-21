#include <iostream>

// 实验三（今天的主角）：普通局部 vs static 局部
void normal_counter() {
    int count = 0;                    // 每次调用都重新出生、重新归零
    ++count;
    std::cout << "普通局部 count = " << count << std::endl;
}

void static_counter() {
    static int count = 0;             // 只出生一次；之后调用不再执行这行
    ++count;
    std::cout << "static   count = " << count << std::endl;
}

int main() {
    for (int i = 1; i <= 3; ++i) {
        std::cout << "—— 第 " << i << " 次调用 ——" << std::endl;
        normal_counter();
        static_counter();
    }
    return 0;
}
