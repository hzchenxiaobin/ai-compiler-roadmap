#include <iostream>
#include <string>

// std::string 全家福：长度、下标、拼接、substr、find
int main() {
    std::string s1 = "Hello";
    std::string s2 = "C++";
    std::string joined = s1 + ", " + s2 + "!";     // + 号直接拼接

    std::cout << "joined = " << joined << std::endl;
    std::cout << "size() = " << joined.size() << "（自带长度，不用数句号）" << std::endl;
    std::cout << "第一个字符 joined[0] = " << joined[0] << std::endl;

    joined += "!!";                                 // += 追加，自动扩容
    std::cout << "追加后 = " << joined << "，size() = " << joined.size() << std::endl;

    std::string sub = joined.substr(7, 3);          // 从下标 7 起，取 3 个字符
    std::cout << "substr(7, 3) = " << sub << std::endl;

    std::size_t pos = joined.find("C++");           // 找子串，返回下标
    std::cout << "find(\"C++\") = " << pos << std::endl;

    std::size_t miss = joined.find("Java");
    std::cout << "find(\"Java\") = " << miss << "（npos：没找到的暗号）" << std::endl;
    return 0;
}
