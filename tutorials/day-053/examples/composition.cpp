#include <iostream>
#include <string>

// 组合：类做成员——Car 有一台 Engine、四个轮子（比全塞一个类优雅在哪）
class Engine {
private:
    double horsepower_;

public:
    explicit Engine(double hp) : horsepower_(hp) {
        std::cout << "  [Engine 构造] " << hp << " 马力" << std::endl;
    }
    double horsepower() const { return horsepower_; }
};

class Wheel {
private:
    int pressure_;

public:
    explicit Wheel(int p) : pressure_(p) {
        std::cout << "  [Wheel 构造] 胎压 " << p << std::endl;
    }
    int pressure() const { return pressure_; }
};

class Car {
private:
    std::string model_;      // 成员一：字符串
    Engine engine_;          // 成员二：类对象（组合！）
    Wheel wheels_[4];        // 成员三：类对象数组

public:
    Car(const std::string& model, double hp, int pressure)
        : model_(model), engine_(hp), wheels_{Wheel(pressure), Wheel(pressure),
                                              Wheel(pressure), Wheel(pressure)} {
        std::cout << "  [Car 构造] " << model_ << " 组装完成" << std::endl;
    }

    void report() const {
        std::cout << model_ << "：" << engine_.horsepower() << " 马力，"
                  << "胎压 " << wheels_[0].pressure() << std::endl;
    }
};

int main() {
    std::cout << "创建 car：" << std::endl;
    Car car("学步车 1.0", 75, 230);
    car.report();
    return 0;
}
