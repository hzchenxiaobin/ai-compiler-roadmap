#include <iostream>
#include <map>
#include <set>
#include <string>

// Day 87：multimap/multiset 与 equal_range——学生按班级分组（计划指定练习）
int main() {
    std::cout << "== multimap：一键多值 ==" << std::endl;
    std::multimap<std::string, std::string> classes;     // 键=班级 值=学生
    classes.insert({"一班", "小明"});
    classes.insert({"二班", "小红"});
    classes.insert({"一班", "小刚"});                     // 同键再插——不覆盖！
    classes.insert({"一班", "小亮"});
    classes.insert({"二班", "小李"});
    std::cout << "  插入 5 人（一班×3、二班×2），size = " << classes.size() << std::endl;
    std::cout << "  （map 同场景会覆盖——multimap 全保留）" << std::endl;

    std::cout << "\n== 按班级分组视图（键序遍历） ==" << std::endl;
    for (const auto& [cls, student] : classes) {
        std::cout << "  " << cls << "：" << student << std::endl;
    }
    std::cout << "  （一键多值连续排列——同键相邻，键序分组）" << std::endl;

    std::cout << "\n== count：一班有几人 ==" << std::endl;
    std::cout << "  count(一班) = " << classes.count("一班") << std::endl;
    std::cout << "  count(二班) = " << classes.count("二班") << std::endl;
    std::cout << "  count(三班) = " << classes.count("三班") << "（不存在）" << std::endl;

    std::cout << "\n== equal_range：取出一班的全部 ==" << std::endl;
    auto [first, last] = classes.equal_range("一班");     // 返回该键的区间 [first, last)
    std::cout << "  一班成员：";
    for (auto it = first; it != last; ++it) {
        std::cout << it->second << ' ';
    }
    std::cout << "（区间长度 = " << classes.count("一班") << "）" << std::endl;

    std::cout << "\n== multiset：可重复的集合 ==" << std::endl;
    std::multiset<int> scores = {90, 85, 90, 78, 85, 90};
    std::cout << "  插入 {90, 85, 90, 78, 85, 90}，size = " << scores.size() << std::endl;
    std::cout << "  排序后：";
    for (int s : scores) { std::cout << s << ' '; }
    std::cout << std::endl;
    std::cout << "  count(90) = " << scores.count(90) << "（重复全保留）" << std::endl;

    std::cout << "\n== equal_range 在 multiset：90 分的有谁（位置区间） ==" << std::endl;
    auto [f2, l2] = scores.equal_range(90);
    std::cout << "  90 分区间长度 = " << std::distance(f2, l2) << std::endl;
    std::cout << "  （排序序列里同值相邻——equal_range 一刀切出区间）" << std::endl;

    std::cout << "\n== 删除策略对照 ==" << std::endl;
    scores.erase(90);                                     // 按值删——全部 90 消失
    std::cout << "  erase(90) 后 count(90) = " << scores.count(90) << "（全删）" << std::endl;
    scores.insert({90, 85, 90});
    scores.erase(scores.find(90));                        // 按迭代器删——只删一个
    std::cout << "  erase(find(90)) 后 count(90) = " << scores.count(90) << "（只删一个！）" << std::endl;
    return 0;
}
