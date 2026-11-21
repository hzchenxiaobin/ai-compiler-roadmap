#include <iostream>
#include <memory>
#include <string>
#include <thread>
#include <vector>

// Day 116：shared_ptr——引用计数（计划指定：观察 use_count 变化）
class Doc {
public:
    explicit Doc(std::string t) : title_(std::move(t)) {
        std::cout << "  [创建] Doc(\"" << title_ << "\")" << std::endl;
    }
    ~Doc() { std::cout << "  [销毁] ~Doc(\"" << title_ << "\")" << std::endl; }
    const std::string& title() const { return title_; }
private:
    std::string title_;
};

void show(const char* label, const std::shared_ptr<Doc>& p) {
    std::cout << "  " << label << "：use_count = " << p.use_count()
              << (p.use_count() == 1 ? "（唯一）" : "") << std::endl;
}

int main() {
    std::cout << "== 计划指定：use_count 的生命周期 ==" << std::endl;
    std::cout << "语句：auto a = make_shared<Doc>(\"共享文档\");" << std::endl;
    auto a = std::make_shared<Doc>("共享文档");
    show("创建后 a", a);                                  // 1

    std::cout << "语句：{ auto b = a; }（块内共享）" << std::endl;
    {
        auto b = a;                                      // 拷贝 → 计数 +1
        show("块内 b = a 后", a);                          // 2（a 和 b 共享）
        show("从 b 看", b);                                // 2（同一计数）
    }
    show("块结束 b 析构后", a);                            // 1（减回）

    std::cout << "语句：auto c = a; auto d = a;" << std::endl;
    auto c = a; auto d = a;
    show("三个共享时", a);                                 // 3
    c.reset();                                           // c 放手
    show("c.reset() 后", a);                               // 2
    d = nullptr;                                         // d 也放手（赋 nullptr 同效）
    show("d = nullptr 后", a);                             // 1

    std::cout << "\n== 最后一个放手才销毁 ==" << std::endl;
    std::cout << "语句：a.reset();（最后一个）" << std::endl;
    a.reset();
    std::cout << "  （上面应打出 [销毁]——计数归零触发）" << std::endl;

    std::cout << "\n== 拷贝 vs 移动对计数的不同影响 ==" << std::endl;
    auto x = std::make_shared<Doc>("X");
    auto y = x;                                          // 拷贝：计数 +1
    show("拷贝后 x", x);                                   // 2
    auto z = std::move(x);                               // 移动：计数不变（换把手）
    show("move 后 z", z);                                  // 2（x 的份额转给 z）
    std::cout << "  x 现在 " << (x ? "非空" : "空") << std::endl;

    std::cout << "\n== 容器共享：多处持有一份 ==" << std::endl;
    auto shared_doc = std::make_shared<Doc>("容器共享");
    std::vector<std::shared_ptr<Doc>> refs;
    refs.push_back(shared_doc);                          // 计数 +1
    refs.push_back(shared_doc);                          // 再 +1
    show("原始 + 容器两个", shared_doc);                    // 3
    std::cout << "  （多处引用同一份——最后清空才销毁）" << std::endl;
    refs.clear();
    show("容器清空后", shared_doc);                         // 1

    std::cout << "\n== 计数的原子性（线程安全说明） ==" << std::endl;
    std::cout << "  计数本身是原子的（多线程增减不加锁也不乱）" << std::endl;
    std::cout << "  但指向的对象不是！（并发读写 Doc 要自己加锁）" << std::endl;
    std::cout << "  （明天 weak_ptr + 后天并发周展开）" << std::endl;
    std::cout << "\n== main 尾的清理 ==" << std::endl;
    return 0;
}
