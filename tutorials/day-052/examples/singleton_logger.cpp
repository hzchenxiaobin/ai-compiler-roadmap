#include <iostream>
#include <string>

// =delete 的设计应用：不可拷贝的单例雏形（计划指定练习）
class Logger {
private:
    std::string name_;
    int log_count_;

    // ①构造私有：外界不能自己造 Logger
    Logger() : name_("全局日志"), log_count_(0) {
        std::cout << "  [Logger 出生] 全局唯一实例创建" << std::endl;
    }

public:
    // ②禁拷贝：单例的命根子——拷贝出第二个就不是单例了
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    // ③静态访问点：全程序经这里拿唯一实例
    static Logger& instance() {
        static Logger the_one;              // 函数内 static：首次调用时创建，进程结束才销毁
        return the_one;
    }

    void log(const std::string& msg) {
        ++log_count_;
        std::cout << "  [#" << log_count_ << "] " << msg << std::endl;
    }

    int count() const { return log_count_; }
    ~Logger() {
        std::cout << "  [Logger 销毁] 共记录 " << log_count_ << " 条" << std::endl;
    }
};

// 全局函数试用单例（不同的调用方共享同一实例）
void module_a() {
    Logger::instance().log("模块 A 启动");
}

void module_b() {
    Logger::instance().log("模块 B 干活");
}

int main() {
    std::cout << "== 通过静态点获取（两次都是同一个） ==" << std::endl;
    module_a();
    module_b();
    Logger::instance().log("main 结束前");

    std::cout << "\n== 引用别名（仍然唯一） ==" << std::endl;
    Logger& ref = Logger::instance();       // 引用不拷贝——单例的正确持有方式
    ref.log("经引用调用");
    std::cout << "总条数 = " << Logger::instance().count() << std::endl;

    // Logger l;                       // ✘ 编译错误：构造私有！
    // Logger l2 = Logger::instance(); // ✘ 编译错误：拷贝构造被 =delete！
    return 0;                            // 进程结束时静态实例销毁（打析构日志）
}
