#include <cstdio>
#include <iostream>
#include <memory>
#include <string>

// Day 118：make_* 深入 + 自定义删除器——RAII 封装 FILE*（计划指定）
// 计划指定的场景：FILE* 的关闭不是 delete 而是 fclose！
void file_closer(FILE* f) {
    if (f) {
        std::cout << "  [fclose] " << (void*)f << std::endl;
        fclose(f);
    }
}

// 删除器的三种形态对照
struct FileCloser {                                  // 函子版（无状态、零开销）
    void operator()(FILE* f) const { file_closer(f); }
};

int main() {
    std::cout << "== 计划指定：RAII 封装 FILE* ==" << std::endl;
    {
        // 函数指针版
        std::unique_ptr<FILE, void(*)(FILE*)> f1(fopen("/tmp/d118_a.txt", "w"), file_closer);
        if (f1) {
            fprintf(f1.get(), "hello RAII\n");
            std::cout << "  f1 写入完成（函数指针删除器）" << std::endl;
        }
    }                                                 // 出块自动 fclose！
    std::cout << "  （f1 出块——上面打了 [fclose]）" << std::endl;

    {
        // 函子版（更常用——类型里带策略）
        std::unique_ptr<FILE, FileCloser> f2(fopen("/tmp/d118_b.txt", "w"));
        if (f2) {
            fprintf(f2.get(), "functor closer\n");
            std::cout << "  f2 写入完成（函子删除器）" << std::endl;
        }
    }
    std::cout << "  （f2 出块也自动 fclose）" << std::endl;

    {
        // lambda 版（就地定义策略）
        auto f3 = std::unique_ptr<FILE, void(*)(FILE*)>(
            fopen("/tmp/d118_c.txt", "w"),
            [](FILE* f) { if (f) { std::cout << "  [fclose-lambda]\n"; fclose(f); } });
        if (f3) { fprintf(f3.get(), "lambda closer\n"); }
    }

    std::cout << "\n== make_unique vs 裸 new 的对照 ==" << std::endl;
    {
        auto a = std::make_unique<std::string>("make 版");           // 推荐
        std::unique_ptr<std::string> b(new std::string("裸 new 版")); // 能编译但逊
        std::cout << "  make_unique：" << *a << "；裸 new：" << *b << std::endl;
        std::cout << "  （make 省一次类型书写 + 异常安全——Effective 21）" << std::endl;
    }

    std::cout << "\n== 数组版本 ==" << std::endl;
    {
        auto arr = std::make_unique<int[]>(10);        // unique_ptr<int[]>——下标访问
        arr[0] = 42; arr[9] = 99;
        std::cout << "  arr[0]=" << arr[0] << " arr[9]=" << arr[9]
                  << "（make_unique<int[]>——数组删除器 delete[]）" << std::endl;
        std::cout << "  （优先 vector<int>——数组版是特殊情况）" << std::endl;
    }

    std::cout << "\n== 验证文件真的关了 ==" << std::endl;
    FILE* check = fopen("/tmp/d118_a.txt", "r");
    if (check) {
        char buf[64] = {};
        fgets(buf, sizeof(buf), check);
        std::cout << "  读回：" << buf;
        fclose(check);
    }
    std::cout << "  （RAII 封装后读写两不误——忘了 fclose 不存在）" << std::endl;
    return 0;
}
