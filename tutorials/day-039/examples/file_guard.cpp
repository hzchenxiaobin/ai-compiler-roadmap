#include <fstream>
#include <iostream>
#include <string>

// 第 39 天主角：FileGuard——构造拿资源、析构放资源（RAII）
class FileGuard {
private:
    std::fstream file_;
    std::string name_;
    bool is_open_;

public:
    explicit FileGuard(const std::string& name) : name_(name), is_open_(false) {
        file_.open(name, std::ios::in | std::ios::out | std::ios::app);
        is_open_ = file_.is_open();
        std::cout << "【构造】打开 " << name_
                  << (is_open_ ? " 成功" : " 失败") << std::endl;
    }

    ~FileGuard() {                                    // 析构函数：对象销毁时自动收摊
        if (is_open_) {
            file_.close();
            std::cout << "【析构】关闭 " << name_ << std::endl;
        }
    }

    bool ok() const { return is_open_; }

    void append(const std::string& line) {
        if (is_open_) {
            file_ << line << std::endl;
        }
    }
};

int main() {
    {
        FileGuard log("guard_log.txt");               // 进入块：构造打开
        log.append("第一行：RAII 写入");
        log.append("第二行：块还没结束");
        std::cout << "  块内工作……" << std::endl;
    }                                                 // 出块：析构自动关闭——没有一行 close()

    std::cout << "块外验证：文件是否写好了？" << std::endl;
    std::ifstream in("guard_log.txt");
    std::string line;
    while (std::getline(in, line)) {
        std::cout << "  读回：" << line << std::endl;
    }
    return 0;
}
