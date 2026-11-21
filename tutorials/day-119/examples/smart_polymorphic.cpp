#include <iostream>
#include <memory>
#include <string>
#include <vector>

// Day 119：智能指针与容器/多态 + enable_shared_from_this
class Shape {
public:
    virtual ~Shape() = default;                        // 虚析构（多态容器的底线——Day 69）
    virtual std::string name() const = 0;
    virtual double area() const = 0;
};

class Circle : public Shape {
public:
    explicit Circle(double r) : r_(r) {}
    std::string name() const override { return "圆(r=" + std::to_string(r_) + ")"; }
    double area() const override { return 3.14159 * r_ * r_; }
private:
    double r_;
};

class Rect : public Shape {
public:
    Rect(double w, double h) : w_(w), h_(h) {}
    std::string name() const override { return "矩形(" + std::to_string(w_) + "×" + std::to_string(h_) + ")"; }
    double area() const override { return w_ * h_; }
private:
    double w_, h_;
};

// enable_shared_from_this：对象内部能安全拿到自己的 shared_ptr
class Service : public std::enable_shared_from_this<Service> {
public:
    void register_self();                              // 把自己注册到某处（需要 shared）
    std::string id() const { return "服务"; }
private:
};

std::vector<std::shared_ptr<Service>> service_registry;   // 服务注册表（演示用）

void Service::register_self() {
    // shared_from_this()：安全拿到管理自己的 shared_ptr 的拷贝
    // （不能 shared_ptr<Service>(this)——会造出第二个独立计数 → 双删！）
    service_registry.push_back(shared_from_this());    // 需要类是 shared 管理的
}

int main() {
    std::cout << "== 多态容器：智能指针的标准配合 ==" << std::endl;
    std::vector<std::unique_ptr<Shape>> shapes;
    shapes.push_back(std::make_unique<Circle>(2));
    shapes.push_back(std::make_unique<Rect>(3, 4));
    shapes.push_back(std::make_unique<Circle>(1));
    double total = 0;
    for (const auto& s : shapes) {
        std::cout << "  " << s->name() << "：面积 " << s->area() << std::endl;
        total += s->area();
    }
    std::cout << "  总面积 = " << total << "（Day 76 的老朋友——vector<unique_ptr<Shape>>）" << std::endl;

    std::cout << "\n== 容器拷贝语义：unique 版 vs shared 版 ==" << std::endl;
    // auto copy = shapes;   // ✘ 编译错（unique 不可拷贝——Day 115）
    std::cout << "  vector<unique_ptr> 拷贝 → 编译错（独占传播）" << std::endl;
    std::vector<std::shared_ptr<Shape>> shared_shapes;
    shared_shapes.push_back(std::make_shared<Circle>(2));
    shared_shapes.push_back(std::make_shared<Rect>(3, 4));
    auto shared_copy = shared_shapes;                  // ✔ 拷贝（计数 +1）
    std::cout << "  vector<shared_ptr> 拷贝 → ✔（计数各 +1："
              << shared_copy[0].use_count() << "）" << std::endl;
    std::cout << "  （要拷贝容器的场景选 shared 版）" << std::endl;

    std::cout << "\n== enable_shared_from_this（了解） ==" << std::endl;
    {
        auto svc = std::make_shared<Service>();
        std::cout << "  注册前 count = " << svc.use_count() << std::endl;
        svc->register_self();                          // 内部拿自己的 shared 注册
        std::cout << "  注册后 count = " << svc.use_count()
                  << "（registry 里那份也是 +1）" << std::endl;
    }
    std::cout << "  （svc 出块——但 service_registry 还持着：服务活着" << std::endl;
    std::cout << "   （查一下：" << (service_registry.back() ? "非空" : "空") << "）" << std::endl;
    std::cout << "  （典型用途：回调/异步任务里保活自己）" << std::endl;
    service_registry.clear();                          // 手动清——服务此刻才销毁
    std::cout << "  registry 清了——服务此刻销毁 ✔" << std::endl;
    return 0;
}
