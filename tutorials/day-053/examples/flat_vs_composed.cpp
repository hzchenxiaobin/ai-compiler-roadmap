#include <iostream>
#include <string>

// 反例：不用组合，把所有数据塞进一个类（"上帝类"的雏形）
class FlatCar {
private:
    std::string model_;
    double horsepower_;      // Engine 的数据被拍平搬进来
    int wheel_pressure_[4];  // Wheel 的数据被拍平搬进来

public:
    FlatCar(const std::string& model, double hp, int pressure) : model_(model), horsepower_(hp) {
        for (int i = 0; i < 4; ++i) {
            wheel_pressure_[i] = pressure;
        }
    }

    // 想给发动机加个"转速"功能？得改 FlatCar——发动机的活全在 FlatCar 里
    void turbo(double factor) { horsepower_ *= factor; }

    void report() const {
        std::cout << model_ << "：" << horsepower_ << " 马力，胎压 " << wheel_pressure_[0] << std::endl;
    }
};

// 组合版：Engine 自己管自己，Car 只管拼装（见 composition.cpp）
// 两条演进路线的对比：
// 1. FlatCar 加功能 = 改一个大类（改发动机要动车架）
// 2. Car 加功能 = 改 Engine 类，Car 原样复用

int main() {
    FlatCar flat("拍平版", 75, 230);
    flat.turbo(1.5);
    flat.report();
    std::cout << "（发动机逻辑长在车里——换台发动机要重写整车）" << std::endl;
    return 0;
}
