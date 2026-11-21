#include <iostream>
#include <string>

// FileGuard 的正式武器化：=delete 替代"约定不拷贝"
class FileGuard {
private:
    int fd_;
    std::string name_;

public:
    FileGuard(const std::string& name, int fd) : fd_(fd), name_(name) {
        std::cout << "  [打开] " << name_ << "（fd=" << fd_ << "）" << std::endl;
    }

    // =delete：物理上禁止拷贝——两个守卫关同一个文件 = double close
    FileGuard(const FileGuard&) = delete;
    FileGuard& operator=(const FileGuard&) = delete;

    // 移动允许（Day 111 预告：转移所有权而非复制）
    FileGuard(FileGuard&& other) noexcept : fd_(other.fd_), name_(std::move(other.name_)) {
        other.fd_ = -1;                    // 源失活——只有一份守卫权
        std::cout << "  [移交] 守卫权转移" << std::endl;
    }

    ~FileGuard() {
        if (fd_ >= 0) {
            std::cout << "  [关闭] " << name_ << "（fd=" << fd_ << "）" << std::endl;
            fd_ = -1;
        } else {
            std::cout << "  [析构] 已移交的空守卫（不重复关）" << std::endl;
        }
    }

    void use() { std::cout << "  [使用] " << name_ << std::endl; }
};

// 用引用传（正确姿势——守卫唯一性保持）
void process(FileGuard& g) {
    g.use();
}

int main() {
    std::cout << "== 引用传递（守卫唯一） ==" << std::endl;
    {
        FileGuard f("data.txt", 3);
        process(f);                       // 引用：不出守卫权
    }                                     // 出块：f 自己关

    std::cout << "\n== 移动移交（Day 111 预告） ==" << std::endl;
    {
        FileGuard f("b.txt", 5);
        FileGuard g = std::move(f);       // 移动：守卫权从 f 转给 g
        g.use();
    }                                     // 出块：g 关（f 已是空守卫——析构不重复关）
    return 0;
}
