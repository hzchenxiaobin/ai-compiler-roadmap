#include <iostream>

// 访问控制的价值：保护"不变量"——宽高永远非负
class SafeRect {
private:
    double width_;
    double height_;

public:
    bool set_size(double w, double h) {     // 返回 bool：设置成功与否
        if (w < 0 || h < 0) {
            return false;                   // 非法值拒收，对象保持原状
        }
        width_ = w;
        height_ = h;
        return true;
    }

    double area() const { return width_ * height_; }
};

int main() {
    SafeRect r;
    r.set_size(3, 4);
    std::cout << "初始面积 = " << r.area() << std::endl;

    bool ok = r.set_size(-1, 5);            // 想设负宽度
    std::cout << "set_size(-1, 5) 返回 " << (ok ? "成功" : "失败")
              << "，面积仍是 " << r.area() << "（不变量保住了）" << std::endl;

    // std::cout << r.width_;               // 取消注释：编译错误！private 外界不可见
    return 0;
}
