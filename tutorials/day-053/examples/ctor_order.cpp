#include <iostream>
#include <string>

// 构造顺序实验：成员按声明序构造、逆序析构（组合的进出栈）
class Part {
private:
    char tag_;

public:
    explicit Part(char tag) : tag_(tag) {
        std::cout << "  +" << tag_ << " 构造" << std::endl;
    }
    ~Part() {
        std::cout << "  -" << tag_ << " 析构" << std::endl;
    }
};

class Whole {
private:
    Part a_;       // 声明顺序 = 构造顺序
    Part b_;
    Part c_;

public:
    Whole() : a_('A'), b_('B'), c_('C') {
        std::cout << "  [Whole 构造] 零件就位，整体组装" << std::endl;
    }
    ~Whole() {
        std::cout << "  [Whole 析构] 整体拆解" << std::endl;
    }
};

int main() {
    std::cout << "声明 Whole w;（进入作用域）" << std::endl;
    {
        Whole w;
        std::cout << "  ……使用 w……" << std::endl;
    }
    std::cout << "w 已销毁（离开作用域）" << std::endl;
    return 0;
}
