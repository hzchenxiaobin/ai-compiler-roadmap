#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <vector>

// Day 83：容器选择决策 + 适配器——括号匹配（stack 经典题，计划指定）
bool brackets_match(const std::string& s) {
    std::stack<char> st;
    for (char c : s) {
        if (c == '(' || c == '[' || c == '{') {
            st.push(c);                        // 开括号入栈
        } else if (c == ')' || c == ']' || c == '}') {
            if (st.empty()) { return false; }  // 没人配对——早退
            char open = st.top(); st.pop();    // 最近的开括号出栈
            if ((c == ')' && open != '(') ||
                (c == ']' && open != '[') ||
                (c == '}' && open != '{')) {
                return false;                  // 类型不匹配
            }
        }
    }
    return st.empty();                         // 栈空 = 全配对；非空 = 有开无闭
}

int main() {
    std::cout << "== 括号匹配（stack 的 LIFO 天职） ==" << std::endl;
    std::vector<std::string> tests = {
        "()",           // true
        "([{}])",       // true——嵌套
        "(]",           // false——类型错
        "(",            // false——有开无闭
        ")",            // false——有闭无开
        "a(b[c]d)e",    // true——无关字符穿过
        "",             // true——空串
    };
    for (const auto& t : tests) {
        std::cout << "  \"" << t << "\" → "
                  << (brackets_match(t) ? "匹配 ✔" : "不匹配 ✘") << std::endl;
    }

    std::cout << "\n== stack：LIFO（后进先出） ==" << std::endl;
    std::stack<int> st;
    for (int x : {1, 2, 3}) { st.push(x); }
    std::cout << "  push 1 2 3 后 top = " << st.top()
              << "（最后进的最先可见）" << std::endl;
    st.pop();
    std::cout << "  pop 后 top = " << st.top() << std::endl;
    std::cout << "  size = " << st.size() << "（无下标、无遍历——只有顶）" << std::endl;

    std::cout << "\n== queue：FIFO（先进先出） ==" << std::endl;
    std::queue<std::string> q;
    q.push("一号");
    q.push("二号");
    q.push("三号");
    std::cout << "  入队 一二三 后 front = " << q.front()
              << "（最先来的先服务）" << std::endl;
    q.pop();
    std::cout << "  出队后 front = " << q.front() << std::endl;

    std::cout << "\n== priority_queue：按优先级出队 ==" << std::endl;
    std::priority_queue<int> pq;               // 默认大顶堆
    for (int x : {3, 1, 4, 1, 5}) { pq.push(x); }
    std::cout << "  push 3 1 4 1 5 后出队顺序：";
    while (!pq.empty()) { std::cout << pq.top() << ' '; pq.pop(); }
    std::cout << "（大的先出）" << std::endl;
    return 0;
}
