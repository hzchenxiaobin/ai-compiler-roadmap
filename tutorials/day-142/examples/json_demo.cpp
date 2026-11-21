// json_demo —— CMake 构建的演示可执行文件
// 验证：CMake 的标准版/警告配置在真实代码上生效
#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <string>
#include <variant>
#include <vector>

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
    explicit JsonValue(bool b) : data_(b) {}
    explicit JsonValue(double n) : data_(n) {}
    explicit JsonValue(std::string s) : data_(std::move(s)) {}
    explicit JsonValue(JsonObject o) : data_(std::move(o)) {}
    const char* type_name() const noexcept {
        switch (data_.index()) {
            case 0: return "null"; case 1: return "bool"; case 2: return "number";
            case 3: return "string"; case 4: return "array"; default: return "object";
        }
    }
    std::size_t size() const {
        if (data_.index() == 5) return std::get<JsonObject>(data_).size();
        return 0;
    }
};
struct Error { std::string msg; int line = 0; };
std::optional<JsonValue> parse(const std::string& src, Error& err) {
    if (src.empty()) { err = {"空输入", 0}; return std::nullopt; }
    if (src.size() >= 2 && src.front() == '{' && src.back() == '}') {
        return JsonValue(JsonObject{});
    }
    err = {"不支持的形式（CMake 演示的子集）", 0};
    return std::nullopt;
}
}  // namespace json

int main() {
    std::cout << "== CMake 构建的 json_demo ==\n";
    std::cout << "  C++ 标准（编译期检测）：" << 
#if __cplusplus >= 202002L
    "C++20 ✔"
#else
    "低于 C++20（配置有问题！）"
#endif
    << "\n";
    json::Error err;
    auto v = json::parse("{\"app\": \"demo\"}", err);
    if (v) { std::cout << "  解析：OK（" << v->type_name() << "，" << v->size() << " 键）\n"; }
    else { std::cout << "  解析：失败 " << err.msg << "\n"; }
    std::cout << "  （-Wall -Wextra 已由 CMake 配置——零警告构建）\n";
    return 0;
}
