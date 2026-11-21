#include <iostream>
#include <map>
#include <set>
#include <string>
#include <tuple>
#include <utility>
#include <vector>

// Day 106：结构化绑定——map 遍历的简化（计划指定）+ pair/tuple/数组/结构体四路绑定
struct Point {
    double x, y;
};

// 返回多个值的函数（tuple）——调用方用结构化绑定直接拆
std::tuple<std::string, int, double> get_student() {
    return {"小明", 15, 88.5};
}

int main() {
    std::cout << "== 计划指定：遍历 map 的简化写法 ==" << std::endl;
    std::map<std::string, int> ages = {{"小明", 15}, {"小红", 14}, {"小刚", 16}};

    std::cout << "旧写法（Day 85 时代）：" << std::endl;
    for (auto it = ages.begin(); it != ages.end(); ++it) {
        std::cout << "  " << it->first << " = " << it->second << std::endl;
    }

    std::cout << "中间写法（范围 for + .first/.second）：" << std::endl;
    for (auto& kv : ages) {
        std::cout << "  " << kv.first << " = " << kv.second << std::endl;
    }

    std::cout << "现代写法（结构化绑定——计划指定）：" << std::endl;
    for (auto& [name, age] : ages) {
        std::cout << "  " << name << " = " << age << std::endl;
    }
    std::cout << "  （k 和 v 有了名字——不用 it->first / kv.second）" << std::endl;

    std::cout << "\n== 旧式手拆对照（std::tie 时代） ==" << std::endl;
    auto p = std::make_pair(42, 3.14);
    int old_a; double old_b;
    std::tie(old_a, old_b) = p;                       // 预声明 + tie 填充
    std::cout << "  tie 版：" << old_a << ", " << old_b << "（要先声明变量——啰嗦）" << std::endl;
    auto [a, b] = p;                                   // 结构化绑定：声明+拆一步到位
    std::cout << "  绑定版：" << a << ", " << b << "（auto [a, b] = p 一步）" << std::endl;

    std::cout << "\n== tuple：多返回值的优雅接收 ==" << std::endl;
    auto [name, age, score] = get_student();           // 三元一次拆
    std::cout << "  " << name << "，" << age << " 岁，" << score << " 分" << std::endl;
    // 旧法：auto t = get_student(); std::get<0>(t) / std::get<1>(t)——序号魔法数

    std::cout << "\n== 结构体：直接拆字段 ==" << std::endl;
    Point pt{3.0, 4.0};
    auto [px, py] = pt;
    std::cout << "  Point{3,4} 拆成 px=" << px << ", py=" << py << std::endl;

    std::cout << "\n== 数组：定长拆解 ==" << std::endl;
    int arr[3] = {10, 20, 30};
    auto [x, y, z] = arr;                              // 数组也能拆（按位置）
    std::cout << "  arr 拆成 " << x << ", " << y << ", " << z << std::endl;

    std::cout << "\n== 嵌套：map 里遍历并统计 ==" << std::endl;
    std::map<std::string, std::vector<int>> grades = {
        {"数学", {90, 85}}, {"语文", {78, 82}},
    };
    int total_subjects = 0;
    for (const auto& [subject, scores] : grades) {     // 绑定 + 常引用
        int sum = 0;
        for (int s : scores) { sum += s; }
        std::cout << "  " << subject << " 均分 = " << (double)sum / scores.size() << std::endl;
        ++total_subjects;
    }
    std::cout << "  共 " << total_subjects << " 科（嵌套遍历配绑定——清晰）" << std::endl;

    std::cout << "\n== insert 的返回值也能拆 ==" << std::endl;
    std::set<std::string> seen;
    auto [iter, inserted] = seen.insert("apple");      // set::insert 返回 pair<迭代器, bool>
    std::cout << "  insert(apple)：inserted = " << (inserted ? "true（新插入）" : "false（已存在）") << std::endl;
    auto [iter2, inserted2] = seen.insert("apple");
    std::cout << "  再 insert(apple)：inserted = " << (inserted2 ? "true" : "false（重复被拒）") << std::endl;
    (void)iter; (void)iter2;
    return 0;
}
