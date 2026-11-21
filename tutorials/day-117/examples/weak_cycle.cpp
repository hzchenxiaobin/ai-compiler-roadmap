#include <iostream>
#include <memory>
#include <string>

// Day 117：weak_ptr 与循环引用——复现泄漏并修复（计划指定）
int leaked_count = 0;      // 泄漏观测：数构造-析构差
int alive_count = 0;

class Node {
public:
    explicit Node(std::string name) : name_(std::move(name)) {
        ++leaked_count; ++alive_count;
        std::cout << "  [构造] " << name_ << std::endl;
    }
    ~Node() {
        --leaked_count; --alive_count;
        std::cout << "  [析构] " << name_ << std::endl;
    }
    std::string name_;
    std::shared_ptr<Node> partner_;      // 强引用（先演示问题）
};

class Node2 {
public:
    explicit Node2(std::string name) : name_(std::move(name)) {
        ++leaked_count; ++alive_count;
        std::cout << "  [构造] " << name_ << std::endl;
    }
    ~Node2() {
        --leaked_count; --alive_count;
        std::cout << "  [析构] " << name_ << std::endl;
    }
    std::string name_;
    std::weak_ptr<Node2> partner_;       // 弱引用（修复版）——不增加计数！

    std::shared_ptr<Node2> partner_shared() const {    // 用时提升
        return partner_.lock();          // lock：活的→返回 shared；死的→nullptr
    }
};

int main() {
    std::cout << "== 计划指定：复现循环引用泄漏 ==" << std::endl;
    {
        auto a = std::make_shared<Node>("甲");
        auto b = std::make_shared<Node>("乙");
        std::cout << "  互指前：a.use_count = " << a.use_count()
                  << "，b.use_count = " << b.use_count() << std::endl;
        a->partner_ = b;                 // a → b（b 的计数 +1 = 2）
        b->partner_ = a;                 // b → a（a 的计数 +1 = 2）——环形成！
        std::cout << "  互指后：a.use_count = " << a.use_count()
                  << "，b.use_count = " << b.use_count() << std::endl;
        std::cout << "  （块结束：a/b 析构各 -1 → 都剩 1 —— 不归零！）" << std::endl;
    }
    std::cout << "  块结束了——看上面：没有 [析构]！泄漏实锤（leaked = " << leaked_count << "）" << std::endl;

    std::cout << "\n== 修复版：weak_ptr 打破环 ==" << std::endl;
    leaked_count = 0;
    {
        auto a = std::make_shared<Node2>("丙");
        auto b = std::make_shared<Node2>("丁");
        std::cout << "  互指前：a.use_count = " << a.use_count() << std::endl;
        a->partner_ = b;                 // weak 赋值——不 +1！
        b->partner_ = a;
        std::cout << "  互指后：a.use_count = " << a.use_count()
                  << "（weak 不增计数——环没形成）" << std::endl;
        if (auto p = a->partner_shared()) {          // lock 提升——临时拥有
            std::cout << "  a 的伙伴是：" << p->name_ << "（lock 成功）" << std::endl;
        }
        std::cout << "  （块结束：a/b 计数归零 → 正常析构）" << std::endl;
    }
    std::cout << "  块结束了——上面有 [析构] ✔（leaked = " << leaked_count << "）" << std::endl;

    std::cout << "\n== lock 的双向语义 ==" << std::endl;
    std::weak_ptr<Node2> w;
    {
        auto temp = std::make_shared<Node2>("短暂");
        w = temp;                        // weak 观察
        std::cout << "  块内 lock：" << (w.lock() ? "活（非空）" : "死") << std::endl;
    }
    std::cout << "  块外 lock：" << (w.lock() ? "活" : "死（nullptr——对象已亡）") << std::endl;
    std::cout << "  （weak 是观察者：不保活、死了就 nullptr）" << std::endl;

    std::cout << "\n== weak 的使用纪律 ==" << std::endl;
    std::cout << "  观察：cache/observer/回调表——不拥有只看" << std::endl;
    std::cout << "  用时：先 lock 拿 shared（临时拥有）再访问" << std::endl;
    std::cout << "  破环：父子/双向引用的'一边'改 weak（通常子指父）" << std::endl;
    return 0;
}
