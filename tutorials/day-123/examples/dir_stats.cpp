#include <filesystem>
#include <fstream>
#include <iostream>
#include <optional>
#include <string>
#include <vector>

namespace fs = std::filesystem;

// Day 123：filesystem——递归遍历目录统计文件大小（计划指定）
struct Stats {
    std::size_t files = 0;
    std::size_t dirs = 0;
    std::uintmax_t total_bytes = 0;
    std::uintmax_t max_file = 0;
    std::string max_name;
};

// 计划指定：递归遍历统计
Stats scan(const fs::path& root) {
    Stats st;
    for (auto it = fs::recursive_directory_iterator(root);
         it != fs::recursive_directory_iterator(); ++it) {
        if (it->is_directory()) { ++st.dirs; continue; }
        if (it->is_regular_file()) {
            ++st.files;
            auto sz = it->file_size();                   // 每个文件的大小
            st.total_bytes += sz;
            if (sz > st.max_file) { st.max_file = sz; st.max_name = it->path().string(); }
        }
    }
    return st;
}

int main() {
    // 造一个测试目录树（三层）
    fs::path root = fs::temp_directory_path() / "d123_demo";
    fs::remove_all(root);                                // 干净起点
    fs::create_directories(root / "a" / "sub");
    fs::create_directories(root / "b");
    auto mk = [&](const fs::path& p, std::size_t n) {
        std::ofstream f(p, std::ios::binary);
        f << std::string(n, 'x');
    };
    mk(root / "top.txt", 100);
    mk(root / "a" / "one.bin", 500);
    mk(root / "a" / "sub" / "two.bin", 200);
    mk(root / "b" / "three.txt", 300);
    mk(root / "b" / ".hidden", 1);

    std::cout << "== 计划指定：递归遍历统计 ==" << std::endl;
    Stats st = scan(root);
    std::cout << "  目录数：" << st.dirs << "（a、a/sub、b + 根）" << std::endl;
    std::cout << "  文件数：" << st.files << std::endl;
    std::cout << "  总字节：" << st.total_bytes << "（100+500+200+300+1 = 1101）" << std::endl;
    std::cout << "  最大文件：" << st.max_name << "（" << st.max_file << " 字节）" << std::endl;

    std::cout << "\n== 遍历的三种姿势 ==" << std::endl;
    std::cout << "  ①directory_iterator（单层）：" << std::endl;
    for (const auto& e : fs::directory_iterator(root)) {
        std::cout << "    " << e.path().filename().string()
                  << (e.is_directory() ? "/" : "") << std::endl;
    }
    std::cout << "  ②recursive_directory_iterator（递归——今天的扫描器）" << std::endl;
    std::cout << "  ③范围 for + entry 的多态接口（is_directory/file_size…）" << std::endl;

    std::cout << "\n== entry 的能力一览 ==" << std::endl;
    for (const auto& e : fs::directory_iterator(root / "a")) {
        std::cout << "  " << e.path().filename().string()
                  << "：目录？" << (e.is_directory() ? "Y" : "N")
                  << " 大小=" << (e.is_regular_file() ? std::to_string(e.file_size()) : "-")
                  << std::endl;
    }

    std::cout << "\n== 常用操作速览 ==" << std::endl;
    std::cout << "  exists / file_size / create_directories ✔（今天用了）" << std::endl;
    std::cout << "  remove_all（清目录）✔（造场前清了）" << std::endl;
    fs::copy(root / "top.txt", root / "top_copy.txt");
    std::cout << "  copy：复制后文件数+1 → " << fs::exists(root / "top_copy.txt") << std::endl;
    std::cout << "  当前目录 current_path() = " << fs::current_path().string().substr(0, 30) << "…" << std::endl;

    // 清理
    fs::remove_all(root);
    std::cout << "\n（测试目录已清理——" << (fs::exists(root) ? "还在" : "已删") << "）" << std::endl;
    return 0;
}
