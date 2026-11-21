#include <algorithm>
#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <variant>
#include <vector>

// Day 128：JsonValue 的数据结构设计——std::variant + 递归结构
// 核心难题：值可以嵌套（数组里有对象、对象里有数组）——类型要"自引用"
// 解法：递归经 unique_ptr 的间接（前向声明 + 指针成员）

// 前向声明（自引用的第一步）
class JsonValue;

// 数组的元素、对象的值：都是 JsonValue（经智能指针的间接）
using JsonArray = std::vector<std::unique_ptr<JsonValue>>;
using JsonObject = std::map<std::string, std::unique_ptr<JsonValue>>;

// variant 的六选一（null 用 std::monostate 表示）
class JsonValue {
public:
    using Storage = std::variant<
        std::monostate,     // null
        bool,
        double,
        std::string,
        JsonArray,          // vector<unique_ptr<JsonValue>>
        JsonObject          // map<string, unique_ptr<JsonValue>>
    >;

    JsonValue() : data_(std::monostate{}) {}                                  // null
    JsonValue(bool b) : data_(b) {}
    JsonValue(double n) : data_(n) {}
    JsonValue(std::string s) : data_(std::move(s)) {}
    JsonValue(JsonArray a) : data_(std::move(a)) {}
    JsonValue(JsonObject o) : data_(std::move(o)) {}

    // 类型查询（六个 is_xxx）
    bool is_null()   const { return std::holds_alternative<std::monostate>(data_); }
    bool is_bool()   const { return std::holds_alternative<bool>(data_); }
    bool is_number() const { return std::holds_alternative<double>(data_); }
    bool is_string() const { return std::holds_alternative<std::string>(data_); }
    bool is_array()  const { return std::holds_alternative<JsonArray>(data_); }
    bool is_object() const { return std::holds_alternative<JsonObject>(data_); }

    const char* type_name() const {
        if (is_null()) return "null";
        if (is_bool()) return "bool";
        if (is_number()) return "number";
        if (is_string()) return "string";
        if (is_array()) return "array";
        return "object";
    }

    // 访问器（错误类型 → 返回默认值；完整版 Day 131 抛异常）
    double as_number() const { return is_number() ? std::get<double>(data_) : 0.0; }
    const std::string& as_string() const { static const std::string e; return is_string() ? std::get<std::string>(data_) : e; }
    bool as_bool() const { return is_bool() ? std::get<bool>(data_) : false; }

    // 容器访问
    JsonArray& as_array() { return std::get<JsonArray>(data_); }
    JsonObject& as_object() { return std::get<JsonObject>(data_); }
    const JsonArray& as_array() const { return std::get<JsonArray>(data_); }
    const JsonObject& as_object() const { return std::get<JsonObject>(data_); }

    // 深度（树的测量——嵌套的量化）
    int depth() const {
        if (is_array()) {
            int d = 0;
            for (const auto& e : as_array()) { d = std::max(d, e->depth()); }
            return d + 1;
        }
        if (is_object()) {
            int d = 0;
            for (const auto& [k, v] : as_object()) { d = std::max(d, v->depth()); }
            return d + 1;
        }
        return 0;   // 叶子
    }

private:
    Storage data_;
};

// 手工建树（明天起由 Parser 自动建）
int main() {
    std::cout << "== 设计的核心：递归 + variant ==" << std::endl;
    std::cout << "  难题：值可嵌套（数组装对象、对象装数组…）——类型要自引用" << std::endl;
    std::cout << "  解法：前向声明 + unique_ptr 间接（数组/对象的元素是指针）" << std::endl;
    std::cout << "  variant：六选一的类型安全联合（Day 121 的技能上场）" << std::endl;

    // 手工建：{"name": "Ada", "scores": [95, 88], "meta": {"ok": true}}
    auto num = [](double n) { return std::make_unique<JsonValue>(n); };

    JsonObject inner;
    inner["ok"] = std::make_unique<JsonValue>(true);

    JsonArray scores;
    scores.push_back(num(95.0));
    scores.push_back(num(88.0));

    JsonObject root;
    root["name"] = std::make_unique<JsonValue>(std::string("Ada"));
    root["scores"] = std::make_unique<JsonValue>(std::move(scores));
    root["meta"] = std::make_unique<JsonValue>(std::move(inner));

    JsonValue doc(std::move(root));

    std::cout << "\n== 手工建树实测 ==" << std::endl;
    std::cout << "  doc.type_name() = " << doc.type_name() << std::endl;
    std::cout << "  键数 = " << doc.as_object().size() << std::endl;
    std::cout << "  name = " << doc.as_object()["name"]->as_string() << std::endl;
    std::cout << "  scores[1] = " << doc.as_object()["scores"]->as_array()[1]->as_number() << std::endl;
    std::cout << "  meta.ok = " << doc.as_object()["meta"]->as_object()["ok"]->as_bool() << std::endl;
    std::cout << "  深度 = " << doc.depth() << "（object→array→number 三层）" << std::endl;

    std::cout << "\n== 类型查询的实测 ==" << std::endl;
    JsonValue n1;
    JsonValue n2(3.14);
    JsonValue n3(std::string("text"));
    JsonValue n4(true);
    std::cout << "  默认构造 → " << n1.type_name() << std::endl;
    std::cout << "  3.14 → " << n2.type_name() << std::endl;
    std::cout << "  \"text\" → " << n3.type_name() << std::endl;
    std::cout << "  true → " << n4.type_name() << std::endl;

    std::cout << "\n== 六值模型的面积 ==" << std::endl;
    std::cout << "  sizeof(JsonValue) = " << sizeof(JsonValue) << " 字节" << std::endl;
    std::cout << "  （variant 的最大成员 + 判别标签）" << std::endl;
    std::cout << "  sizeof(vector<unique_ptr<JsonValue>>) = " << sizeof(JsonArray) << " 字节" << std::endl;
    std::cout << "  sizeof(unique_ptr<JsonValue>) = " << sizeof(std::unique_ptr<JsonValue>) << " 字节（间接的代价）" << std::endl;

    std::cout << "\n== 为什么不用继承多态（对照） ==" << std::endl;
    std::cout << "  继承版：基类+六派生+虚函数——开放扩展（Day 76 风格）" << std::endl;
    std::cout << "  variant版：封闭六选一+visit 穷尽——JSON 的值集合封闭 ✔" << std::endl;
    std::cout << "  （Day 121 的选型原则：封闭用 variant、开放用继承）" << std::endl;
    return 0;
}
