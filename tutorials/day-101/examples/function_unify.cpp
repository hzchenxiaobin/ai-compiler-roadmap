#include <functional>
#include <iostream>
#include <map>
#include <string>
#include <vector>

// ===== Day 101：std::function 与可调用对象的统一——回调注册表（计划指定） =====
int plain_add(int a, int b) { return a + b; }             // 普通函数

struct Multiplier {                                        // 仿函数（函数对象）
    int factor;
    explicit Multiplier(int f) : factor(f) {}
    int operator()(int a, int b) const { return (a + b) * factor; }
};

// 回调注册表：名字 → 任意 int(int,int) 可调用对象
class CallbackRegistry {
private:
    std::map<std::string, std::function<int(int, int)>> table_;
public:
    void register_cb(const std::string& name, std::function<int(int, int)> cb) {
        table_[name] = std::move(cb);
    }
    int call(const std::string& name, int a, int b) {
        auto it = table_.find(name);
        if (it == table_.end()) {
            std::cout << "  [未注册：" << name << "]" << std::endl;
            return 0;
        }
        return it->second(a, b);                           // 统一调用——不关心它是什么
    }
};

int main() {
    std::cout << "== 三种可调用对象装进同一个盒子 ==" << std::endl;
    std::function<int(int, int)> f1 = plain_add;           // 普通函数
    std::function<int(int, int)> f2 = Multiplier(3);       // 仿函数（带状态！）
    int bias = 100;
    std::function<int(int, int)> f3 = [bias](int a, int b) {   // lambda（带捕获）
        return a + b + bias;
    };
    std::cout << "  f1(3,4) = " << f1(3, 4) << "（普通函数）" << std::endl;
    std::cout << "  f2(3,4) = " << f2(3, 4) << "（仿函数 ×3）" << std::endl;
    std::cout << "  f3(3,4) = " << f3(3, 4) << "（lambda +100）" << std::endl;
    std::cout << "  （三种东西、同一个类型、同一个调用语法）" << std::endl;

    std::cout << "\n== 回调注册表（计划指定练习） ==" << std::endl;
    CallbackRegistry registry;
    registry.register_cb("add", plain_add);
    registry.register_cb("triple", Multiplier(3));
    registry.register_cb("biased", [bias](int a, int b) { return a + b + bias; });
    registry.register_cb("mul", [](int a, int b) { return a * b; });

    std::cout << "  call(\"add\", 6, 7)     = " << registry.call("add", 6, 7) << std::endl;
    std::cout << "  call(\"triple\", 6, 7)  = " << registry.call("triple", 6, 7) << std::endl;
    std::cout << "  call(\"biased\", 6, 7)  = " << registry.call("biased", 6, 7) << std::endl;
    std::cout << "  call(\"mul\", 6, 7)     = " << registry.call("mul", 6, 7) << std::endl;
    std::cout << "  call(\"div\", 6, 7)     → ";
    registry.call("div", 6, 7);                            // 未注册的处理

    std::cout << "\n== function 的代价（诚实账本） ==" << std::endl;
    std::cout << "  sizeof(普通函数指针) = " << sizeof(int(*)(int, int)) << std::endl;
    std::cout << "  sizeof(std::function) = " << sizeof(std::function<int(int, int)>)
              << "（更胖——类型擦除的包装）" << std::endl;
    std::cout << "  （换来的：三种可调用统一存取、能进容器、能当字段）" << std::endl;
    std::cout << "  （热路径的性能敏感场景：auto + 模板参数更优——进阶）" << std::endl;
    return 0;
}
