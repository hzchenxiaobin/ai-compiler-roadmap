# 第 17 天：getline 与 stringstream——读整行，自动切词

> **今日目标**：完成本教程后，你能做到：用 `getline` 读入带空格的整行；复现并修复 `>>` 与 `getline` 混用时的新行残留坑；用 `istringstream` + `while (ss >> word)` 自动切词（连多空格、制表符都不怕）；默写"getline 外层读行、stringstream 内层切词"的黄金搭档结构。
> **预计时间**：学习 25 分钟＋实践 45 分钟
> **前置准备**：完成 Day 16（std::string 四件兵器）；Day 4 埋的"cin >> 见空格停"伏笔今天正式回收。
> **今日产出**：坑与修复的对照实验、切词统计程序、逐行记账程序——明天项目日的全部零件。

## 1. 今天解决什么问题？

Day 4 就发现 `cin >> name` 输入 `Xiao Ming` 只拿到 `Xiao`，当时说"getline 专治此症，Day 17 专题讲"。今天兑现。同时解决第二个问题：**拿到整行之后，怎么把行里的词切出来**？昨天用 `find`/`substr` 手工切了十行代码还特判了连续空格——今天用 `stringstream` 一行核心循环全搞定。

这两件事拼起来就是一条流水线：**getline 读行 → stringstream 切词**。明天的文本统计器（行数/字符数/单词数/字母频率）正是沿这条流水线组装的。

## 2. 先看全貌：今天的内容位于哪里？

![getline 与 >> 的对比、stringstream 传送带与黄金搭档结构](images/01-overview.svg)

**图 1**：左上红卡是老坑（`>>` 见空格停，只拿一个词）；左下绿卡是新武器（`getline` 整行抱走，换行符被吃但不进内容）；右侧蓝卡是第二件新武器（`istringstream` 用字符串造传送带，`while (ss >> word)` 自动按空白切词）。底部黄条：本周四块零件集齐，明天项目日只差组装。

三步概括今天：

1. `getline(cin, line)`：读到换行符为止，整行进 `line`（换行符本身被丢弃）；
2. `>>` 和 `getline` 混用，中间必须 `cin.ignore(10000, '\n')` 冲掉残留换行；
3. `istringstream ss(line)` 后 `while (ss >> word)` 逐词切分——任意空白（空格、制表符）全自动跳过。

## 3. 核心概念：先用直觉，再给准确定义

### 3.1 getline：整行抱走

- **一句通俗解释**：`getline` 从传送带上读到**行尾**为止，把整行内容交给字符串。
- **一个类比**：`>>` 像用夹子夹一颗豆子（碰到空隙就停）；`getline` 像用铲子把整盘豆子端走（碰到盘子边才停）。
- **对应到技术上是**：`std::getline(std::cin, line)` 从当前位置读到换行符，换行符被消费但**不计入** line。本机实测：输入 `Xiao Ming`，line 为 9 字符。类比的边界：铲子端的是"这一行"，下一行还在传送带上等下一次 getline。
- **常见误解**："getline 会把换行符存进字符串"——不会，读掉即丢弃；用 `size()` 验证长度不含换行。

### 3.2 混用坑：残留的换行符

- **一句通俗解释**：`>>` 读完数字后**停在换行符前**，换行符留在传送带上；紧接着的 getline 一启动就撞到行尾，抱回一个**空串**。
- **一个类比**：自助餐排队——前面那位取完餐没走开（残留换行），你一进场就"到队尾了"。
- **对应到技术上是**：本机实测坑版：喂入 `20` 和 `Xiao Ming`，`name` 长度 0；修复版加一行 `cin.ignore(10000, '\n')` 后正确拿到 `Xiao Ming`。类比的边界：ignore 这行只在">> 之后接 getline"时需要——**全程只用 getline 就不用**（每次都把换行吃干净了）。
- **标准修复**：`cin >> x;` 之后、`getline` 之前，插一句 `std::cin.ignore(10000, '\n');`——Day 4 的抢救三件套在这里从"救火"变成"日常"。

### 3.3 stringstream：字符串造的传送带

- **一句通俗解释**：`istringstream` 把一个字符串变成"只读传送带"，`>>` 从上面按空白切词。
- **一个类比**：把一句写好的话重新口述给速记员——速记员自动按停顿断句，不用你指出空格在哪。
- **对应到技术上是**：`std::istringstream ss(sentence); while (ss >> word)` 每圈切出一个词；多个连续空格、制表符统统当一次分隔。本机实测：`"the quick   brown fox\tjumps over"` 切出 6 个词，长度 3/5/5/3/5/4。类比的边界：stringstream 只管**这条**字符串——切完就空了，想再切一遍得重新造一个（把同一个句子再喂给新的 istringstream）。
- **最小例子**：昨天 `reverse_words` 十行的 find/substr 切词，核心换成 `while (ss >> word)` 后"连续空格"特判整段消失。
- **常见误解**：以为切词要自己找空格——`>>` 生来就会跳空白（Day 4 就讲过它"跳过空白取一段"），stringstream 只是给了它一条新传送带。

## 4. 图解核心机制

![换行残留的传送带图解与切词流水线](images/02-mechanism.svg)

**图 2**：上半部分把坑演成连环画：喂入 `20⏎ Xiao Ming⏎`——`>>` 吃掉 20 停在 ⏎ 前；getline 从 ⏎ 起步立刻到行尾，抱回空串（红框实测输出）；右下绿框是修复版实测（只差一行 ignore）。左下绿卡是切词流水线：跳空白、取一词、循环——连续空格自动免疫；右下黄卡是黄金搭档的代码骨架。

按图逐步讲：

1. **先看 ⏎ 的位置**：坑的全部原因就是 `>>` 停在换行符**前**——它把 ⏎ 留给了下一位；
2. **再看两条命运**：同样输入，坑版长度 0、修复版长度 9——实验只差一行，结论却天差地别；
3. **切词看跳空白**：`>>` 的老本行（Day 4 传送带取件规则）在 stringstream 上原样生效——这才是"自动切词"的真相；
4. **黄金搭档**：外层 `while (getline(cin, line))` 读行，内层 `while (ss >> word)` 切词——EOF 是外层的自然出口（Day 8 的"暗门"在这里成了正门）。

## 5. 动手实践：完成一个最小可运行例子

### 5.1 准备文件

```bash
cd ai-compiler-roadmap/tutorials/day-017/examples
```

四个程序：坑版 `getline_pitfall.cpp`、修复版 `getline_fixed.cpp`、切词 `word_split.cpp`、逐行记账 `line_loop.cpp`。环境不变（macOS 27，Apple clang 21）。

### 5.2 编写代码

切词主角 `word_split.cpp`（完整代码）：

```cpp
#include <iostream>
#include <sstream>
#include <string>

// stringstream：把字符串当成传送带，>> 自动按空白切词
int main() {
    std::string sentence = "the quick   brown fox\tjumps over";   // 故意混多空格和制表符

    std::istringstream ss(sentence);      // 用字符串造一条传送带
    std::string word;
    int count = 0;

    std::cout << "句子：【" << sentence << "】" << std::endl;
    std::cout << "切出的词：" << std::endl;
    while (ss >> word) {                  // >> 自动跳过任意空白，切出一个词
        ++count;
        std::cout << "  " << count << ". " << word
                  << "（长度 " << word.size() << "）" << std::endl;
    }
    std::cout << "共 " << count << " 个词" << std::endl;
    return 0;
}
```

坑版与修复版只差一行（`std::cin.ignore(10000, '\n');`），逐行记账的 `line_loop.cpp` 见本目录——它就是黄金搭档的完整实现。

### 5.3 运行与观察

坑与修复（同样喂入 `20` 和 `Xiao Ming`，本机真实输出）：

```bash
g++ -Wall getline_pitfall.cpp -o gp && printf '20\nXiao Ming\n' | ./gp
```

```text
年龄：姓名：读到的 name 是【】，长度 0
```

```bash
g++ -Wall getline_fixed.cpp -o gf && printf '20\nXiao Ming\n' | ./gf
```

```text
年龄：姓名：读到的 name 是【Xiao Ming】，长度 9
```

（管道喂入时提示语挤行属正常现象。）切词与逐行记账：

```bash
g++ -Wall word_split.cpp -o ws && ./ws
```

```text
句子：【the quick   brown fox	jumps over】
切出的词：
  1. the（长度 3）
  2. quick（长度 5）
  3. brown（长度 5）
  4. fox（长度 3）
  5. jumps（长度 5）
  6. over（长度 4）
共 6 个词
```

```bash
g++ -Wall line_loop.cpp -o ll && printf 'hello world\nthe quick brown fox\nsingle\n' | ./ll
```

```text
逐行输入（管道喂入或 Ctrl+D 结束）：
第 1 行：长度 11，2 个词
第 2 行：长度 19，4 个词
第 3 行：长度 6，1 个词
合计：3 行，7 个词
```

![两条命运、六个词与三行账本的真实输出](images/03-example.svg)

**图 3**：左上是坑/修复对照（同输入、差一行、天壤之别）；右上是切词战报（三个连续空格加一个制表符全部自动跳过）；下半蓝卡是逐行记账——每行报长度和词数，EOF 自然收工。底部黄卡是明天的组装图纸：行数、词数、字符数、频率分别从哪来。

### 5.4 修改一个输入再观察：坑的三种触发姿势

混用坑不止一种触发方式。再试两个（本机真实结果）：第一，`>>` 读整数后**连续两次** getline——第一次拿到空串，第二次才拿到真内容（残留换行只挡一枪）；第二，输入数字后**跟一个空格再回车**（`20 ⏎`）——`>>` 停在空格，ignore 冲掉"空格+换行"整段残余，getline 照样正常。两个实验共同指向同一条规则：**只要 `>>` 和 getline 交界，就固定插一句 ignore**——不必记忆各种触发姿势，一把钥匙开所有锁。

## 6. 常见错误与排查

**错误 1：`>>` 后直接 getline，拿到空串**

- 现象：姓名读到空、长度 0；
- 原因：残留换行符被 getline 当成"空行"整行读走（今天的坑版实验）；
- 定位：打印 `line.size()`——0 就是踩坑了；
- 修复：交界处插 `std::cin.ignore(10000, '\n');`。

**错误 2：想用 `>>` 读带空格的整句**

- 现象：`"hello world"` 只拿到 `hello`；
- 原因：`>>` 的取件规则就是"跳空白、取到下一段空白前"；
- 定位：打印读到的内容和长度；
- 修复：读整行用 `getline`；读完确实要切词，再上 stringstream。

**错误 3：stringstream 切完一轮发现"词没了"，以为程序坏了**

- 现象：对同一个 ss 再跑一遍 `while (ss >> word)`，一个词都切不出；
- 原因：传送带已经被读完，流状态停在"耗尽"；
- 定位：在第二轮前打印 `ss.eof()`（为 1 说明带已空）；
- 修复：重新造一条带——用原字符串再构造一个新的 `istringstream`。

## 7. 今日练习

### 必做练习

把昨天 `reverse_words.cpp` 的手工切词（find/substr 十行）替换成 `istringstream` 版本，输出必须和昨天**完全一致**（`fox brown quick the`）。预期产出：新版程序 + 两版行数对比（用 `wc -l` 数源文件行数），体会"同一件事的两种成本"。

### 选做挑战

写 `word_count.cpp`：读入多行（getline 循环到 EOF），统计**每个词出现的次数**——用 `string words[100]` + `int counts[100]` 两个平行数组（Day 13 的思路），新词追加、旧词计数 +1，最后按出现顺序打印词频表。这就是周六字母频率的"单词版"预演。

## 8. 今日验收

- [ ] 我能说出 getline 和 `>>` 的取件规则差别，并各举一个适用场景
- [ ] 我能复现混用坑（拿到空串）并写出那一行修复
- [ ] 我能用 stringstream 一行核心循环切词，且多空格/制表符自动免疫
- [ ] 我能默写黄金搭档结构：外层 getline 读行、内层 ss 切词、EOF 出口
- [ ] 我理解为什么昨天 reverse_words 的连续空格特判今天可以整段删掉

## 9. 一句话总结与明天预告

今天的问题是"整行怎么读、行内怎么切"；机制上，getline 整行抱走（换行符丢弃），stringstream 让 `>>` 的跳空白本行在字符串上复用，混用坑的钥匙是 ignore；实践上，坑/修复对照、六词切分、三行账本三组实验全部落地；验证标准是任何输入都能预判 getline 和 `>>` 各拿到什么。明天是**第 3 周收官项目日：文本统计器**——行数、字符数、单词数、各字母频率四本账一次结清，本周四块零件（数组、C 串、string、分词）的组装大考。
