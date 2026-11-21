#include <iostream>

// 多个守卫嵌套：构造顺序进、析构逆序出（栈的后进先出）
class Sentinel {
private:
    char tag_;

public:
    explicit Sentinel(char tag) : tag_(tag) {
        std::cout << "  +" << tag_ << " 构造（拿资源）" << std::endl;
    }
    ~Sentinel() {
        std::cout << "  -" << tag_ << " 析构（放资源）" << std::endl;
    }
};

int work(bool fail_early) {
    Sentinel a('A');
    Sentinel b('B');
    {
        Sentinel c('C');
        if (fail_early) {
            std::cout << "  提前返回……" << std::endl;
            return -1;                // c、b、a 逆序析构
        }
    }                                 // c 先走（块结束）
    std::cout << "  正常工作" << std::endl;
    return 0;                         // b、a 逆序析构
}

int main() {
    std::cout << "场景一：正常走完" << std::endl;
    work(false);
    std::cout << std::endl << "场景二：中途提前返回" << std::endl;
    work(true);
    return 0;
}
