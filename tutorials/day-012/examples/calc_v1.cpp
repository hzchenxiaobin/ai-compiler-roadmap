#include <iostream>
#include <string>
#include <cstdlib>

// ===== 第 12 天项目日：命令行计算器 v1 =====
// 组装清单：Day 8 循环与输入抢救 + Day 9 函数拆分 + Day 10 默认参数
//           + Day 11 作用域（每个函数各管各的变量）+ Day 3 除法陷阱 + Day 5 逻辑运算

double read_number(const std::string& prompt);   // 声明区：四台机器先递名片
char read_op();
bool is_div_zero(char op, double b);
double calc(double a, char op, double b);

int main() {
    std::cout << "=== 命令行计算器 v1（+ - * /）===" << std::endl;
    while (true) {
        double a = read_number("第一个数");
        char op = read_op();
        double b = read_number("第二个数");

        if (is_div_zero(op, b)) {                // 除零是唯一要在计算前拦的错
            std::cout << "除数不能为 0，本次作废。" << std::endl;
        } else {
            std::cout << a << " " << op << " " << b << " = "
                      << calc(a, op, b) << std::endl;
        }

        std::cout << "继续吗？(y/n)：";
        char again = 0;
        if (!(std::cin >> again) || again != 'y') {   // 不答 y 就收工（|| 短路）
            std::cout << "收工！" << std::endl;
            break;
        }
    }
    return 0;
}

// 读一个数：问不到合法的数就一直问（Day 8 的重试环）
double read_number(const std::string& prompt) {
    double value = 0.0;
    while (true) {
        std::cout << prompt << "：";
        if (std::cin >> value) {
            return value;                        // 读到了，立刻出货
        }
        if (std::cin.eof()) {                    // 输入流到头（如管道喂完）
            std::cout << "\n输入结束，收工。" << std::endl;
            std::exit(0);                        // 在函数里结束整个程序
        }
        std::cin.clear();                        // 抢救三件套（Day 4/8）
        std::cin.ignore(10000, '\n');
        std::cout << "这不是一个数，再试一次。" << std::endl;
    }
}

// 读一个运算符：+ - * / 之外的都打回
char read_op() {
    char op = 0;
    while (true) {
        std::cout << "运算符(+ - * /)：";
        if (std::cin >> op) {
            if (op == '+' || op == '-' || op == '*' || op == '/') {
                return op;
            }
            std::cout << "只支持 + - * /，再试一次。" << std::endl;
        } else {
            if (std::cin.eof()) {
                std::cout << "\n输入结束，收工。" << std::endl;
                std::exit(0);
            }
            std::cin.clear();
            std::cin.ignore(10000, '\n');
        }
    }
}

// 除零检查：除法且除数为 0 才拦（&& 与 || 的组合拳）
bool is_div_zero(char op, double b) {
    return op == '/' && b == 0;
}

// 纯计算：调用前已保证 op 合法、无除零
double calc(double a, char op, double b) {
    if (op == '+') return a + b;
    if (op == '-') return a - b;
    if (op == '*') return a * b;
    return a / b;                                // 走到这里一定是合法除法
}
