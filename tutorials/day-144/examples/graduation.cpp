// Day 144：毕业总结——24 周全部复盘、查漏补缺、下一阶段（计划指定的收官三项）
// 本文件：毕业档案的生成器（数字全来自真实盘点）
#include <iostream>
#include <string>
#include <vector>

struct Stage {
    std::string name;
    int week_from, week_to;
    std::string theme;
    std::string highlight;   // 阶段代表作
};

struct Metric {
    std::string label;
    std::string value;
    std::string source;      // 数字的出处（可复现）
};

int main() {
    std::cout << "════════ 144 天毕业档案 ════════\n\n";

    std::cout << "== 一、24 周的六阶段复盘 ==\n";
    std::vector<Stage> stages = {
        {"阶段一", 1, 6,  "基础：语法/函数/文件",           "Day 42 文件读写——程序的持久化"},
        {"阶段二", 7, 8,  "指针与内存：ASan 纪律",           "Day 55 浅拷贝的坑（故意翻车教学）"},
        {"阶段三", 9, 13, "OOP：三法则到多态",               "Day 76 图形库三部曲（虚函数的实战）"},
        {"阶段四", 14, 17, "STL：容器/算法/模板",            "Day 89 LRU 缓存（毕业考项目）"},
        {"阶段五", 18, 21, "现代 C++：类型说话",             "Day 126 大重构（233 文件 c++20 全绿）"},
        {"阶段六", 22, 24, "JSON 解析器（集大成）",          "Day 138 外部考试 283/283 满分"},
    };
    for (const auto& s : stages) {
        std::cout << "  " << s.name << "（第 " << s.week_from << "~" << s.week_to << " 周）"
                  << "：" << s.theme << "\n";
        std::cout << "     ↳ 代表作：" << s.highlight << "\n";
    }

    std::cout << "\n== 二、144 天的数字档案（全部实测） ==\n";
    std::vector<Metric> metrics = {
        {"教程篇数", "144 篇", "每天一篇——Day 1~144 无缺席"},
        {"源文件数", "244 个", "find tutorials -name '*.cpp' 等的盘点"},
        {"SVG 手绘图", "432 张", "find tutorials -name '*.svg'（每篇 ≥3）"},
        {"代码总行数", "13387 行", "wc -l 的合计（-Wall -Wextra 零警告）"},
        {"项目日", "21 个", "词频/LRU/图形库/基准/JSON…全胜"},
        {"毕业考", "6 次", "78/89/102/114/126/144——全过"},
        {"JSON 外部考试", "283/283", "JSONTestSuite 满分（Day 138）"},
        {"性能优化", "1.77x", "view+reserve 的叠加（Day 139 实测）"},
        {"两 San 状态", "全绿", "ASan+UBSan 零报告（Day 140）"},
        {"构建系统", "CMake 三目标", "demo/tests/sanitize（Day 142）"},
    };
    for (const auto& m : metrics) {
        std::cout << "  " << m.label << "：" << m.value << "\n";
        std::cout << "     ↳ 出处：" << m.source << "\n";
    }

    std::cout << "\n== 三、知识清单的查漏（诚实版） ==\n";
    std::cout << "  掌握扎实（能徒手写出）：\n";
    std::cout << "    RAII/智能指针/移动语义/递归下降/variant 树/测试矩阵\n";
    std::cout << "  用过但需加深（下一轮的目标）：\n";
    std::cout << "    并发的实战（thread 只认知级）/ 模板元编程的深水\n";
    std::cout << "    / CMake 的高级（FetchContent/自定义目标）\n";
    std::cout << "  明确的边界（教学取舍——非缺陷）：\n";
    std::cout << "    JSON 的 double 2^53 / 深度 500 / 错误恢复从简\n";

    std::cout << "\n== 四、下一阶段的目标 ==\n";
    std::cout << "  1. 并发实战（thread/mutex/condition_variable 的项目级使用）\n";
    std::cout << "  2. 开源阅读（nlohmann/json 的真源码——对照教学版）\n";
    std::cout << "  3. 第二个项目：简易 HTTP 服务器或脚本解释器\n";
    std::cout << "  4. CI/CD（GitHub Actions——Day 142 的三步流水上云端）\n";
    std::cout << "  （ Effective 的 43~55 条——网络/并发相关，随项目读）\n";

    std::cout << "\n== 五、给这 144 天的一句话 ==\n";
    std::cout << "  「每天 60 分钟，比周末狂练 8 小时更远」——\n";
    std::cout << "    坚持的复利：144 天前连 makefile 都看不懂的人，\n";
    std::cout << "    今天有一个满分解析器和一套工程习惯。\n";
    return 0;
}
