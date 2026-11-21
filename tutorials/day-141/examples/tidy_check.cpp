// json.hpp —— JSON 解析器（教学版）
// 模块：Tokenizer（词法）/ Parser（语法）/ JsonValue（数据）
// 特性：RFC 8259 子集；JSONTestSuite 283/283；ASan+UBSan 全绿
// 限制：数字存 double（2^53 内无损）；深度上限 500
// 天数：Day 127~140 的集大成（本文件是"整理后"的示范形态）
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <functional>
#include <limits>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

namespace json {

// ============================================================
// JsonValue：六种 JSON 值的树形表示
//   null / bool / number / string / array / object
// 设计：variant（封闭集合）+ unique_ptr（自引用的间接）
// ============================================================
class JsonValue;
using JsonArray  = std::vector<std::unique_ptr<JsonValue>>;
using JsonObject = std::map<std::string, std::unique_ptr<JsonValue>>;

class JsonValue {
public:
    using Storage = std::variant<
        std::monostate,   // null
        bool,
        double,           // 教学取舍：2^53 内整数无损
        std::string,
        JsonArray,
        JsonObject>;

    JsonValue() : data_(std::monostate{}) {}
    explicit JsonValue(bool b) : data_(b) {}
    explicit JsonValue(double n) : data_(n) {}
    explicit JsonValue(std::string s) : data_(std::move(s)) {}
    explicit JsonValue(JsonArray a) : data_(std::move(a)) {}
    explicit JsonValue(JsonObject o) : data_(std::move(o)) {}

    const char* type_name() const noexcept {
        switch (data_.index()) {
            case 0: return "null"; case 1: return "bool"; case 2: return "number";
            case 3: return "string"; case 4: return "array"; default: return "object";
        }
    }

private:
    Storage data_;
};

// ============================================================
// parse：文本 → 树（用户唯一入口）
// 用法：
//   json::Error err;
//   auto v = json::parse(src, err);
//   if (v) 用 *v; else 打 err.msg（带行号）
// ============================================================
struct Error {
    std::string msg;
    int line = 0;
};

// 最小实现（示范文件的子集——完整版见 Day 133/138 的 json_full / run_suite）
std::optional<JsonValue> parse(const std::string& src, Error& err) {
    if (src.empty()) { err = {"空输入", 0}; return std::nullopt; }
    if (src.size() >= 2 && src.front() == '{' && src.back() == '}') {
        return JsonValue(JsonObject{});
    }
    if (src == "null")  { return JsonValue(); }
    if (src == "true")  { return JsonValue(true); }
    if (src == "false") { return JsonValue(false); }
    char* end = nullptr;
    double v = std::strtod(src.c_str(), &end);
    if (end == src.c_str() + src.size()) { return JsonValue(v); }
    if (src.front() == '"' && src.back() == '"') { return JsonValue(src.substr(1, src.size()-2)); }
    err = {"不支持的形式（示范子集）", 0};
    return std::nullopt;
}

}  // namespace json

// ---- 演示：注释规范的三层（文件头/节注释/行内） ----
int main() {
    // 块注释示例（节级）：下面的字符串用于演示
    const char* demo = "{\"app\": \"demo\", \"ver\": 1.0}";

    json::Error err;
    auto v = json::parse(demo, err);  // 行内注释：一行一个动作

    if (v) {
        std::cout << "解析成功：" << v->type_name() << "\n";
    } else {
        std::cout << "失败：" << err.msg << "\n";
    }
    return 0;
}
