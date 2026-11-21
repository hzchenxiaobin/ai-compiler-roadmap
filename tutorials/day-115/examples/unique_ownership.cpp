#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

// Day 115：unique_ptr——独占所有权、只能移动（计划指定：工厂函数返回 unique_ptr）
class Resource {
public:
    explicit Resource(std::string name) : name_(std::move(name)) {
        std::cout << "  [获取] Resource(\"" << name_ << "\")" << std::endl;
    }
    ~Resource() { std::cout << "  [释放] ~Resource(\"" << name_ << "\")" << std::endl; }
    void use() const { std::cout << "  [使用] " << name_ << std::endl; }
    const std::string& name() const { return name_; }
private:
    std::string name_;
};

// 计划指定：工厂函数返回 unique_ptr（Day 77 的 make_shape 同款升级）
std::unique_ptr<Resource> make_resource(const std::string& name) {
    return std::make_unique<Resource>(name);      // 就地构造 + 自动包进独占指针
}

// 所有权转移：交接仪式（只能 move 不能拷）
std::unique_ptr<Resource> transfer_ownership(std::unique_ptr<Resource> r) {
    std::cout << "  [交接] 收到 \"" << r->name() << "\"（我是新主人）" << std::endl;
    return r;                                       // 再交回去（NRVO/移动）
}

int main() {
    std::cout << "== 基本用法：创建与自动释放 ==" << std::endl;
    {
        auto r = std::make_unique<Resource>("数据库连接");
        r->use();
        std::cout << "  （作用域尾自动释放——没有 delete！）" << std::endl;
    }
    std::cout << "  （出块了——上面已释放 ✔）" << std::endl;

    std::cout << "\n== 独占性：拷贝被禁 ==" << std::endl;
    auto a = std::make_unique<Resource>("独占资源");
    // auto b = a;                       // ✘ 编译错！拷贝构造被 =delete（Day 112 的 NoCopy 同款）
    std::cout << "  auto b = a; → 编译错（拷贝被禁——Day 112 =delete 实战）" << std::endl;
    auto b = std::move(a);               // 移动 ✔（所有权交接）
    std::cout << "  auto b = move(a); → ✔（所有权交给 b）" << std::endl;
    std::cout << "  a 现在 " << (a ? "非空" : "空（nullptr）") << std::endl;
    b->use();

    std::cout << "\n== 计划指定：工厂函数 ==" << std::endl;
    auto r2 = make_resource("工厂产物");
    r2->use();
    std::cout << "  （工厂造的——调用方拿到就是所有者）" << std::endl;

    std::cout << "\n== 所有权交接（函数往返） ==" << std::endl;
    auto r3 = make_resource("传家宝");
    r3 = transfer_ownership(std::move(r3));       // 交出去再拿回来
    std::cout << "  （回来了——" << (r3 ? "非空 ✔" : "空 ✘") << "）" << std::endl;

    std::cout << "\n== 容器里的独占指针 ==" << std::endl;
    std::vector<std::unique_ptr<Resource>> pool;
    pool.push_back(make_resource("资源A"));
    pool.push_back(make_resource("资源B"));
    pool.push_back(std::make_unique<Resource>("资源C"));
    std::cout << "  池里有 " << pool.size() << " 个：" << std::endl;
    for (const auto& r : pool) { r->use(); }
    std::cout << "  （vector 持有所有权——析构时逐个自动释放）" << std::endl;

    std::cout << "\n== 作用域结束的清理顺序 ==" << std::endl;
    std::cout << "  （main 结尾：pool 的三个 + b + r2 + r3 逆序释放——看日志）" << std::endl;
    return 0;
}
