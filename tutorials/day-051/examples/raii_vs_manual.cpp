#include <fstream>
#include <iostream>
#include <string>

// 对照实验：手动 close 在多 return 路径下的漏关风险 vs RAII 的全自动收摊
int manual_style(int mode, const std::string& file_name) {
    std::ofstream out(file_name);
    if (!out) {
        return 1;                     // 路径一：打开失败（此时没开，无需关）
    }
    out << "manual style line" << std::endl;
    if (mode == 1) {
        return 2;                     // 路径二：提前返回——忘了 close！（冲盘靠析构兜底）
    }
    if (mode == 2) {
        // 假设这里还有十行代码、三层 if……忘了 close 的概率只增不减
        return 3;                     // 路径三：又一个提前返回
    }
    out.close();                      // 幸存路径：记得关
    return 0;
}

class Guard {
private:
    std::ofstream out_;
    std::string name_;

public:
    explicit Guard(const std::string& name) : name_(name) {
        out_.open(name);
        std::cout << "  【构造】" << name_ << std::endl;
    }
    ~Guard() {
        if (out_.is_open()) {
            out_.close();
            std::cout << "  【析构】" << name_ << " 已收摊" << std::endl;
        }
    }
    void write(const std::string& s) { out_ << s << std::endl; }
};

int raii_style(int mode, const std::string& file_name) {
    Guard g(file_name);               // 打开
    g.write("raii style line");
    if (mode == 1) {
        return 2;                     // 提前返回——析构照样执行！
    }
    if (mode == 2) {
        return 3;                     // 再提前返回——依然安全
    }
    return 0;                         // 正常路径——同样安全
}

int main() {
    std::cout << "手动版 mode=1（提前返回，忘了 close）：" << std::endl;
    manual_style(1, "manual.txt");
    std::cout << "  （手动版无输出——但 manual.txt 已被析构冲盘）" << std::endl;

    std::cout << "RAII 版 mode=1（提前返回）：" << std::endl;
    raii_style(1, "raii.txt");
    std::cout << "RAII 版 mode=0（正常路径）：" << std::endl;
    raii_style(0, "raii.txt");
    return 0;
}
