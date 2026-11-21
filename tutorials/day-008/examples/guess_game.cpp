#include <iostream>
#include <cstdlib>

// 第 8 天主线例子：猜数字游戏（随机数 + 次数统计）
// 注意：srand(42) 是固定种子——每次运行答案都相同，方便教学复现；
// 想要每局不同，把 42 换成 std::time(nullptr)（<ctime> 头文件）
int main() {
    std::srand(42);                             // 播下随机种子
    int secret = std::rand() % 100 + 1;         // 1 ~ 100 的随机数
    int guess = 0;
    int tries = 0;

    std::cout << "我想好了一个 1~100 的数，猜猜看！" << std::endl;
    while (true) {
        std::cout << "你的猜测：";
        if (!(std::cin >> guess)) {             // 乱敲字母：抢救后重来
            if (std::cin.eof()) {               // 输入流已到尽头（如管道喂完）
                std::cout << "\n输入结束，还没猜中就散场了。" << std::endl;
                return 1;
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "请输入 1~100 的整数。" << std::endl;
            continue;
        }

        ++tries;                                // 有效猜测才计数
        if (guess < secret) {
            std::cout << "太小了，往大猜。" << std::endl;
        } else if (guess > secret) {
            std::cout << "太大了，往小猜。" << std::endl;
        } else {
            std::cout << "猜中了！答案就是 " << secret
                      << "，共用了 " << tries << " 次。" << std::endl;
            break;                              // 命中，退出循环
        }
    }
    return 0;
}
