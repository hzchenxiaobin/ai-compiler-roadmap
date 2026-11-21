#include <iostream>
#include <iomanip>

// 第 6 天项目日：小小换算器
// 组合了：Day 4 的问答与输入校验 + Day 5 的 switch 分支 + 菜单循环（Day 8 正式讲，先当固定句式）
int main() {
    std::cout << std::fixed << std::setprecision(2);   // 全程保留 2 位小数显示

    while (true) {                                     // 菜单循环：0 才会 break 出去
        std::cout << "\n=== 小小换算器 ===" << std::endl;
        std::cout << "1) 摄氏 → 华氏" << std::endl;
        std::cout << "2) 千米 → 英里" << std::endl;
        std::cout << "3) 人民币 → 美元（示例汇率 7.2）" << std::endl;
        std::cout << "0) 退出" << std::endl;
        std::cout << "请选择：";

        int choice = 0;
        if (!(std::cin >> choice)) {                   // 不是数字：抢救传送带，重来
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "看不懂这个选项，请输入数字 0~3。" << std::endl;
            continue;                                  // 跳回循环开头重新显示菜单
        }

        if (choice == 0) {
            std::cout << "再见！" << std::endl;
            break;                                     // 唯一的正常出口
        }

        double value = 0.0;
        switch (choice) {
            case 1:
                std::cout << "输入摄氏温度：";
                std::cin >> value;
                std::cout << value << " ℃ = " << value * 9.0 / 5.0 + 32.0 << " ℉" << std::endl;
                break;
            case 2:
                std::cout << "输入千米数：";
                std::cin >> value;
                std::cout << value << " km = " << value * 0.621371 << " 英里" << std::endl;
                break;
            case 3:
                std::cout << "输入人民币金额：";
                std::cin >> value;
                std::cout << value << " 元 ≈ " << value / 7.2 << " 美元（示例汇率）" << std::endl;
                break;
            default:
                std::cout << "没有这个选项：" << choice << std::endl;
        }
    }
    return 0;
}
