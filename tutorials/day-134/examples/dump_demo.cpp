#include <cmath>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <variant>
#include <vector>

// Day 134：序列化——dump() 与格式化输出（树 → 文本）
namespace json {

class JsonValue;
using JsonArray  = std::vector<std::unique_ptr<JsonValue>>;
using JsonObject = std::map<std::string, std::unique_ptr<JsonValue>>;

class JsonValue {
public:
    using Storage = std::variant<std::monostate, bool, double, std::string, JsonArray, JsonObject>;
    Storage data_;
public:
    JsonValue() : data_(std::monostate{}) {}
    JsonValue(bool b) : data_(b) {}
    JsonValue(double n) : data_(n) {}
    JsonValue(std::string s) : data_(std::move(s)) {}
    JsonValue(JsonArray a) : data_(std::move(a)) {}
    JsonValue(JsonObject o) : data_(std::move(o)) {}
    const char* type_name() const {
        switch (data_.index()) {
            case 0: return "null"; case 1: return "bool"; case 2: return "number";
            case 3: return "string"; case 4: return "array"; default: return "object";
        }
    }
    const Storage& raw() const { return data_; }
};

// ===== 字符串的转义编码（Day 130 的逆过程） =====
std::string escape_string(const std::string& s) {
    std::string out = "\"";
    for (char c : s) {
        switch (c) {
            case '"':  out += "\\\""; break;
            case '\\': out += "\\\\"; break;
            case '\n': out += "\\n";  break;
            case '\t': out += "\\t";  break;
            case '\r': out += "\\r";  break;
            case '\b': out += "\\b";  break;
            case '\f': out += "\\f";  break;
            default:   out += c;      break;   // 非 ASCII（UTF-8 字节）直通
        }
    }
    out += '"';
    return out;
}

// ===== 数字的格式化（短而准确） =====
std::string format_number(double v) {
    if (std::isfinite(v) && v == static_cast<long long>(v)
        && std::abs(v) < 1e15) {
        // 整数值且不太大 → 打成整数（42 而不是 42.000000）
        return std::to_string(static_cast<long long>(v));
    }
    std::ostringstream oss;
    oss << v;       // 默认 6 位有效——够教学
    return oss.str();
}

// ===== dump：递归序列化 =====
void dump_to(const JsonValue& v, std::string& out, int indent, int depth) {
    // indent == 0：紧凑；>0：每层缩进 N 空格
    auto nl = [&](int d) {
        if (indent > 0) { out += '\n'; out += std::string(static_cast<std::size_t>(indent) * d, ' '); }
    };

    switch (v.raw().index()) {
        case 0: out += "null"; break;
        case 1: out += (std::get<bool>(v.raw()) ? "true" : "false"); break;
        case 2: out += format_number(std::get<double>(v.raw())); break;
        case 3: out += escape_string(std::get<std::string>(v.raw())); break;
        case 4: {   // array
            const auto& arr = std::get<JsonArray>(v.raw());
            if (arr.empty()) { out += "[]"; break; }
            out += '[';
            for (std::size_t i = 0; i < arr.size(); ++i) {
                nl(depth + 1);
                dump_to(*arr[i], out, indent, depth + 1);
                if (i + 1 < arr.size()) { out += ','; }
            }
            nl(depth);
            out += ']';
            break;
        }
        default: {  // object
            const auto& obj = std::get<JsonObject>(v.raw());
            if (obj.empty()) { out += "{}"; break; }
            out += '{';
            bool first = true;
            for (const auto& [k, val] : obj) {
                if (!first) { out += ','; }
                first = false;
                nl(depth + 1);
                out += escape_string(k);
                out += indent > 0 ? ": " : ":";
                dump_to(*val, out, indent, depth + 1);
            }
            nl(depth);
            out += '}';
            break;
        }
    }
}

std::string dump(const JsonValue& v, int indent = 0) {
    std::string out;
    dump_to(v, out, indent, 0);
    return out;
}

} // namespace json

int main() {
    std::cout << "Day 134：序列化 dump()\n" << std::endl;

    // 手工建一棵树（明天的访问接口会更方便）
    auto num = [](double n) { return std::make_unique<json::JsonValue>(n); };

    json::JsonObject inner;
    inner["level"] = num(2.0);
    inner["desc"] = std::make_unique<json::JsonValue>(std::string("嵌套\"对象\""));

    json::JsonArray arr;
    arr.push_back(num(1.0));
    arr.push_back(num(2.5));
    arr.push_back(std::make_unique<json::JsonValue>(true));
    arr.push_back(std::make_unique<json::JsonValue>(std::string("含\n换行")));

    json::JsonObject root;
    root["name"] = std::make_unique<json::JsonValue>(std::string("测试"));
    root["port"] = num(8080.0);
    root["pi"] = num(3.14159);
    root["empty_arr"] = std::make_unique<json::JsonValue>(json::JsonArray{});
    root["empty_obj"] = std::make_unique<json::JsonValue>(json::JsonObject{});
    root["tags"] = std::make_unique<json::JsonValue>(std::move(arr));
    root["nested"] = std::make_unique<json::JsonValue>(std::move(inner));
    json::JsonValue doc(std::move(root));

    std::cout << "== 紧凑模式（indent=0） ==" << std::endl;
    std::cout << json::dump(doc) << std::endl;

    std::cout << "\n== 格式化模式（indent=2） ==" << std::endl;
    std::cout << json::dump(doc, 2) << std::endl;

    std::cout << "\n== 数字的整数形态 ==" << std::endl;
    std::cout << "  8080.0 → " << json::format_number(8080.0) << std::endl;
    std::cout << "  3.14159 → " << json::format_number(3.14159) << std::endl;
    std::cout << "  -0.5 → " << json::format_number(-0.5) << std::endl;
    std::cout << "  42.0 → " << json::format_number(42.0) << std::endl;

    std::cout << "\n== 转义的编码 ==" << std::endl;
    std::cout << "  \"a\\nb\" → " << json::escape_string("a\nb") << std::endl;
    std::cout << "  含引号 → " << json::escape_string("say \"hi\"") << std::endl;
    std::cout << "  反斜杠 → " << json::escape_string("C:\\path") << std::endl;

    std::cout << "\n== 空/单元素 ==" << std::endl;
    json::JsonValue empty_arr(json::JsonArray{});
    json::JsonValue empty_obj(json::JsonObject{});
    std::cout << "  空数组 → " << json::dump(empty_arr) << std::endl;
    std::cout << "  空对象 → " << json::dump(empty_obj) << std::endl;

    std::cout << "\n== 设计要点 ==" << std::endl;
    std::cout << "  递归下降的镜像：dump_to 也是递归（树结构决定）" << std::endl;
    std::cout << "  紧凑/格式化：indent 参数控制（0=紧凑 >0=缩进）" << std::endl;
    std::cout << "  转义编码：Day 130 解码的逆过程（\n→\\n 等）" << std::endl;
    std::cout << "  数字的整数化：42.0 打成 42（美观）" << std::endl;
    return 0;
}
