#include <iostream>

// 越界实验：合法下标是 0~7，故意写 scores[8]
int main() {
    const int WEEKS = 8;
    int scores[WEEKS] = {12, 25, 9, 31, 18, 27, 22, 35};

    std::cout << "scores[7] = " << scores[7] << "（最后一个合法成员）" << std::endl;
    std::cout << "现在往 scores[8] 写入 999……" << std::endl;
    scores[8] = 999;                          // 越界！写进了邻居的地盘
    std::cout << "写完了，程序还在跑——但没有报错不等于没出事" << std::endl;
    return 0;
}
