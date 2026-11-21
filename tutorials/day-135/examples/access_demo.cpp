#include <iostream>
#include <map>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <variant>
#include <vector>

// Day 135：访问接口——get<T>()、operator[]、迭代（让树用起来顺手）
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

    // ===== 类型查询 =====
    bool is_null()   const { return data_.index() == 0; }
    bool is_bool()   const { return data_.index() == 1; }
    bool is_number() const { return data_.index() == 2; }
    bool is_string() const { return data_.index() == 3; }
    bool is_array()  const { return data_.index() == 4; }
    bool is_object() const { return data_.index() == 5; }

    // ===== get<T>()：类型安全的取值（错类型抛异常） =====
    template <typename T>
    T get() const {
        if constexpr (std::is_same_v<T, bool>) {
            if (is_bool()) return std::get<bool>(data_);
        } else if constexpr (std::is_same_v<T, double>) {
            if (is_number()) return std::get<double>(data_);
        } else if constexpr (std::is_same_v<T, std::string>) {
            if (is_string()) return std::get<std::string>(data_);
        }
        throw std::runtime_error(std::string("get<") + typeid(T).name() + ">：实际是 " + type_name());
    }

    // 模板的 int 特化（整数便捷版）
    int get_int() const {
        if (!is_number()) { throw std::runtime_error("get_int：实际是 " + std::string(type_name())); }
        return static_cast<int>(std::get<double>(data_));
    }

    // ===== operator[]：对象的键访问 =====
    // 只读版：不存在 → 抛异常（比返回 null 更直白——访问错键该发现）
    const JsonValue& operator[](const std::string& key) const {
        if (!is_object()) {
            throw std::runtime_error("operator[](key)：实际是 " + std::string(type_name()) + " 不是 object");
        }
        const auto& obj = std::get<JsonObject>(data_);
        auto it = obj.find(key);
        if (it == obj.end()) {
            throw std::runtime_error("键不存在：\"" + key + "\"");
        }
        return *it->second;
    }

    // 数组版：operator[](size_t)
    const JsonValue& operator[](std::size_t i) const {
        if (!is_array()) {
            throw std::runtime_error("operator[](index)：实际是 " + std::string(type_name()) + " 不是 array");
        }
        const auto& arr = std::get<JsonArray>(data_);
        if (i >= arr.size()) {
            throw std::runtime_error("数组越界：[" + std::to_string(i) + "]（size=" + std::to_string(arr.size()) + "）");
        }
        return *arr[i];
    }

    // ===== 迭代支持 =====
    // 数组：begin/end 指向元素
    std::size_t size() const {
        if (is_array())  { return std::get<JsonArray>(data_).size(); }
        if (is_object()) { return std::get<JsonObject>(data_).size(); }
        return 0;
    }

    // 对象的键值迭代（简化：暴露内部的 map——教学取舍）
    const JsonObject& as_object() const {
        if (!is_object()) { throw std::runtime_error("as_object：实际是 " + std::string(type_name())); }
        return std::get<JsonObject>(data_);
    }
    const JsonArray& as_array() const {
        if (!is_array()) { throw std::runtime_error("as_array：实际是 " + std::string(type_name())); }
        return std::get<JsonArray>(data_);
    }
};

} // namespace json

// 简化版 parse（Day 133 的核心——够演示用）
namespace json {
struct Error { std::string msg; int line; };
enum class Tok { LB, RB, LK, RK, C, M, S, N, T, F, NU, E };
struct Token { Tok type; std::string text; int line; };

std::optional<JsonValue> parse(const std::string& src, Error& err) {
    // 极简 tokenizer+parser（演示访问接口为主——完整版见 Day 133）
    std::vector<Token> toks; std::size_t i = 0; int line = 1;
    auto push = [&](Tok t, std::string s = "") { toks.push_back({t, std::move(s), line}); };
    while (i < src.size()) {
        char c = src[i];
        if (c=='\n') { ++line; ++i; continue; }
        if (c==' '||c=='\t'||c=='\r') { ++i; continue; }
        switch (c) {
            case '{': push(Tok::LB); ++i; break;
            case '}': push(Tok::RB); ++i; break;
            case '[': push(Tok::LK); ++i; break;
            case ']': push(Tok::RK); ++i; break;
            case ':': push(Tok::C); ++i; break;
            case ',': push(Tok::M); ++i; break;
            case '"': { std::string s; ++i; while (i<src.size()&&src[i]!='"') s+=src[i++]; ++i; push(Tok::S,std::move(s)); break; }
            default: {
                if (c=='-'||(c>='0'&&c<='9')) { std::string s; while (i<src.size()&&((src[i]>='0'&&src[i]<='9')||src[i]=='.'||src[i]=='e'||src[i]=='E'||src[i]=='-'||src[i]=='+')) s+=src[i++]; push(Tok::N,std::move(s)); }
                else if (src.compare(i,4,"true")==0) { push(Tok::T); i+=4; }
                else if (src.compare(i,5,"false")==0) { push(Tok::F); i+=5; }
                else if (src.compare(i,4,"null")==0) { push(Tok::NU); i+=4; }
                else { err = {"意外字符", line}; return std::nullopt; }
            }
        }
    }
    push(Tok::E);
    // 递归下降
    std::size_t p = 0;
    std::function<std::unique_ptr<JsonValue>()> value = [&]() -> std::unique_ptr<JsonValue> {
        switch (toks[p].type) {
            case Tok::LB: {
                ++p; JsonObject obj;
                if (toks[p].type != Tok::RB) {
                    while (true) {
                        std::string k = toks[p].text; ++p; ++p;  // key + colon
                        obj[k] = value();
                        if (toks[p].type == Tok::M) { ++p; continue; }
                        break;
                    }
                }
                ++p; return std::make_unique<JsonValue>(std::move(obj));
            }
            case Tok::LK: {
                ++p; JsonArray arr;
                if (toks[p].type != Tok::RK) {
                    while (true) {
                        arr.push_back(value());
                        if (toks[p].type == Tok::M) { ++p; continue; }
                        break;
                    }
                }
                ++p; return std::make_unique<JsonValue>(std::move(arr));
            }
            case Tok::S: return std::make_unique<JsonValue>(toks[p++].text);
            case Tok::N: return std::make_unique<JsonValue>(std::stod(toks[p++].text));
            case Tok::T: ++p; return std::make_unique<JsonValue>(true);
            case Tok::F: ++p; return std::make_unique<JsonValue>(false);
            default: ++p; return std::make_unique<JsonValue>();
        }
    };
    auto v = value();
    return JsonValue(std::move(*v));
}
} // namespace json

#include <functional>

int main() {
    std::cout << "Day 135：访问接口\n" << std::endl;

    json::Error err;
    auto parsed = json::parse(
        "{\"name\": \"Ada\", \"age\": 36, \"scores\": [95, 88, 92], "
        "\"address\": {\"city\": \"Hangzhou\", \"zip\": \"310000\"}, \"active\": true}",
        err);
    if (!parsed) { std::cout << "解析失败" << std::endl; return 1; }
    const json::JsonValue& doc = *parsed;

    std::cout << "== get<T>()：类型安全取值 ==" << std::endl;
    std::cout << "  get<std::string>(name) = " << doc["name"].get<std::string>() << std::endl;
    std::cout << "  get<double> 经 [age] = " << doc["age"].get<double>() << std::endl;
    std::cout << "  get_int(age) = " << doc["age"].get_int() << std::endl;
    std::cout << "  get<bool>(active) = " << (doc["active"].get<bool>() ? "true" : "false") << std::endl;

    std::cout << "\n== operator[] 的链式下钻 ==" << std::endl;
    std::cout << "  doc[\"address\"][\"city\"] = " << doc["address"]["city"].get<std::string>() << std::endl;
    std::cout << "  doc[\"scores\"][1] = " << doc["scores"][1].get<double>() << std::endl;

    std::cout << "\n== 迭代 ==" << std::endl;
    std::cout << "  顶层键（对象迭代）：" << std::endl;
    for (const auto& [k, v] : doc.as_object()) {
        std::cout << "    " << k << " : " << v->type_name() << std::endl;
    }
    std::cout << "  scores 逐个（数组迭代）：" << std::endl;
    for (const auto& e : doc["scores"].as_array()) {
        std::cout << "    " << e->get<double>() << ' ';
    }
    std::cout << std::endl;

    std::cout << "\n== 错误的执法 ==" << std::endl;
    try { doc["name"].get<double>(); }
    catch (const std::runtime_error& e) { std::cout << "  get 类型错：\"" << e.what() << "\" ✔" << std::endl; }
    try { doc["nope"]; }
    catch (const std::runtime_error& e) { std::cout << "  键不存在：\"" << e.what() << "\" ✔" << std::endl; }
    try { doc["scores"][99]; }
    catch (const std::runtime_error& e) { std::cout << "  越界：\"" << e.what() << "\" ✔" << std::endl; }
    try { doc["name"][0]; }
    catch (const std::runtime_error& e) { std::cout << "  非容器[]：\"" << e.what() << "\" ✔" << std::endl; }

    std::cout << "\n== 设计要点 ==" << std::endl;
    std::cout << "  get<T>：错类型抛（Day 121 的\"value()\"风格）" << std::endl;
    std::cout << "  operator[]：键不存在抛（比返回 null 更直白）" << std::endl;
    std::cout << "  链式下钻：doc[a][b][c] 一气呵成" << std::endl;
    std::cout << "  as_object/as_array：迭代的入口" << std::endl;
    return 0;
}
