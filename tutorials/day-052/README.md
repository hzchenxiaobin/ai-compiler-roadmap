# 第 52 天：const 成员函数、mutable、static 成员

> **今日目标**：完成本教程后，你能做到：解释 const 对象为什么只能调 const 成员函数（实测编译拦截）；用 mutable 实现缓存/计数器（const 函数里的合法后门）；用 static 成员 + 静态函数实现实例计数器（类外定义）；给出三者的选型判断（什么时候标 const、开 mutable、上 static）。
> **预计时间**：学习 25 分钟＋实践 45 分钟
> **前置准备**：完成 Day 49~51（const 成员函数昨天已在用；逆序销毁是计数器实验的对照系）。
> **今日产出**：const 对象执法记录、mutable 缓存实验（调 3 次算 1 遍）、带实例计数器的类（呼吸曲线实录）。

## 1. 今天解决什么问题？

三个看似零散的关键词其实是同一主题：**类的访问控制从"读写"升级到"状态"**。const 成员函数回答"这个操作改不改对象"；const 对象把承诺升级成强制（冻结）；mutable 回答"const 里有没有例外"（有——实现细节）；static 回答"有没有不属于任何个体、全类共享的数据"（有——计数器就是）。

三个概念都直接服务周六的银行账户项目日：利率全类一份（static）、查询不改账（const 函数）、操作日志藏在 const 里也能记（mutable）。今天的带实例计数器的类正是计划指定的练习。

## 2. 先看全貌：今天的内容位于哪里？

![const 对象、mutable 后门与 static 共享](images/01-overview.svg)

**图 1**：左卡是 const 对象的边界（const 函数 ✔、非 const ✘ 编译拦截——实测报错原文）；中卡是 mutable 的适用边界（逻辑状态不许改、实现细节可以改）；下方红卡是 static 的共享模型（类外定义 + 静态函数免对象调用）。

三步概括今天：

1. const 对象 → 只能调 const 成员函数 → this 变 const → 改成员即编译错（三级联动）；
2. mutable：const 函数里唯一能改的成员——只放计数器/缓存这类实现细节；
3. static：全类一份，类外定义，静态成员函数不吃 this。

## 3. 核心概念：先用直觉，再给准确定义

### 3.1 const 成员函数与 const 对象：承诺与冻结

- **一句通俗解释**：const 成员函数是函数级的承诺（"我不改成员"）；const 对象是对象级的冻结（"谁都不许改我"）。
- **一个类比**：博物馆展品——展签写着"可触摸"的（const 函数）观众能碰；进了恒温展柜的（const 对象）只有"可触摸"标签的才被允许——柜子外面的"可触摸"权限自动失效。
- **对应到技术上是**：本机实测——`const Rectangle frozen(10,20);` 后 `frozen.area()` ✔（const 函数）、`frozen.scale(2)` ✘，报错 `'this' argument to member function 'scale' has type 'const Rectangle', but function is not marked const`。类比的边界：冻结的是**对象**不是类——别的 Rectangle 实例照常 scale（实测 normal 放大到宽 6，frozen 仍是 10）。
- **为什么 const 函数里改成员会错**：编译器把 this 的类型从 `Rectangle*` 变成 `const Rectangle*`——Day 22 锁物的成员函数版。
- **常见误解**："const 函数就是不变的函数"——不；它是不改**成员**的函数（函数内可以有局部变量、可以打印）。

### 3.2 mutable：const 里的合法后门

- **一句通俗解释**：mutable 成员在 const 函数里也允许改——专为"观察不到的实现细节"开的门。
- **一个类比**：博物馆的讲解员——展品（逻辑状态）一件没动，但讲解次数计数器（实现细节）一直在走；观众看到的行为没有任何变化。
- **对应到技术上是**：本机实测——const Circle 的 `area() const` 里 `++area_calls_`、写 `cached_area_`（都是 mutable），三次调用只算一遍（首次计算并缓存，后两次直取）。类比的边界：**判断标准是"观察行为变不变"**——返回值、可见效果不变的改动可 mutable（缓存/计数/日志）；一旦影响观察行为（改 radius_），const 承诺就破了，编译器拦。
- **常见误解**："mutable 就是去掉 const 的随便改"——不是；滥用 mutable（把逻辑状态标成 mutable）等于把锁锯了，code review 会被打回。

### 3.3 static 成员：全类共享的账本

- **一句通俗解释**：static 成员属于类本身，不属于任何对象——一千个对象也共享这一份。
- **一个类比**：班级的到课人数——不属于任何一个学生，但每个学生进出都影响它。
- **对应到技术上是**：本机实测 Widget 计数器——构造 `++alive_`、析构 `--alive_`，进出块数字精确呼应 Day 51 的逆序销毁（开局 0 → 块内 3 → 出块 1 → 终局 2 → 析构回 0）。两个语法点：**类外定义**（`int Widget::alive_ = 0;`——静态成员的家在类外，头文件声明、源文件定义是标准分法）；**静态成员函数** `static int alive()` 不吃 this——没有对象也能调（实测开局就调了）。类比的边界：计数器在多线程下要考虑竞争（进阶话题，学到并发时回头加固）。
- **与 Day 11 局部 static 的关系**：同关键字两种用法——函数内的 static 局部变量（跨调用保持）和类内的 static 成员（全类共享）；共同点是"生命期超出常规作用域"。
- **常见误解**："static 成员在每个对象里都有一份"——恰恰相反；sizeof(对象) 不含 static 成员。

## 4. 图解核心机制

![const 三级火箭与 static 共享屋](images/02-mechanism.svg)

**图 2**：上半部分是 const 的成长史（三级火箭）——Day 22 锁变量/指针、Day 49 锁成员函数、今天冻结整个对象，三级联动的传导链（const 对象 → 只能调 const 函数 → this 是 const → 改成员编译错）；左下绿卡是 static 的共享屋（对象各带 name_，共享一份 alive_，类外定义是家）；右下灰卡是三者的选型速查。

按图逐步讲：

1. **看传导链理解 const**：为什么 const 对象调非 const 函数是编译错？非 const 函数的 this 没有 const——编译器无法保证它不改冻结对象——干脆禁止调用；
2. **看共享屋记 static**：类内声明是"图纸上的公共设施"，类外定义才是"真正建出来"——忘记定义会报 undefined reference（Day 27 的老朋友）；
3. **选型三问**：函数不改成员？标 const。成员是纯实现细节？mutable。数据属于全类？static——三问覆盖今天全部决策；
4. **呼吸曲线的意义**：计数器精确呼应逆序销毁——Day 51 的时刻表今天变成了可查询的数字。

## 5. 动手实践：完成一个最小可运行例子

### 5.1 准备文件

```bash
cd ai-compiler-roadmap/tutorials/day-052/examples
```

三个程序：`const_object.cpp`、`mutable_cache.cpp`、`instance_counter.cpp`。

### 5.2 编写代码

计数器主角 `instance_counter.cpp` 的类核心（完整代码见本目录）：

```cpp
class Widget {
private:
    std::string name_;
    static int alive_;                 // 声明：当前存活的对象数（全类一份）

public:
    explicit Widget(const std::string& name) : name_(name) {
        ++alive_;
        std::cout << "  +" << name_ << "　存活数 = " << alive_ << std::endl;
    }

    ~Widget() {
        --alive_;
        std::cout << "  -" << name_ << "　存活数 = " << alive_ << std::endl;
    }

    static int alive() {               // 静态成员函数：不需要对象就能调
        return alive_;
    }
};

int Widget::alive_ = 0;                // 定义：类外初始化（静态成员的家在类外）
```

### 5.3 运行与观察

```bash
g++ -Wall const_object.cpp -o co && ./co
```

实际执行结果（本机）：

```text
normal 面积 = 12
frozen 面积 = 200
normal 放大后宽 = 6
frozen 宽仍是 10
```

```bash
g++ -Wall mutable_cache.cpp -o mc && ./mc
```

```text
第一次 area() =   （首次计算并缓存）
12.5664
第二次 area() = 12.5664
第三次 area() = 12.5664
area 被调了 3 次，但只算了一遍
```

```bash
g++ -Wall instance_counter.cpp -o ic && ./ic
```

```text
开局存活数 = 0
  +甲　存活数 = 1
  +乙　存活数 = 2
  +丙　存活数 = 3
块内存活数 = 3
  -丙　存活数 = 2
  -乙　存活数 = 1
出块后存活数 = 1
  +丁　存活数 = 2
终局存活数 = 2
  -丁　存活数 = 1
  -甲　存活数 = 0
```

![冻结执法、缓存生效与计数器呼吸](images/03-example.svg)

**图 3**：左上 const 对象的边界（frozen 没被放大 + 编译错误原文）；右上 mutable 缓存（调 3 次算 1 遍）；下半橙卡是计数器的呼吸曲线（与 Day 51 逆序销毁精确呼应）；灰卡预告明天的组合和周六的银行账户。

### 5.4 修改一行再观察：冻结执法亲测

把 `const_object.cpp` 里注释的 `frozen.scale(2);` 放开（预期编译失败，本机实测）：

```text
error: 'this' argument to member function 'scale' has type 'const Rectangle', but function is not marked const
```

报错信息本身就是教材：this 的类型是 const Rectangle、函数没标 const——**传导链的三环全部出现在一行错误里**。对照注释前的输出（frozen 宽仍是 10），冻结没有被绕过。再做一个对照：去掉 frozen 的 const（变成普通对象），同一条 scale 调用合法——锁在对象上，不在函数上。

## 6. 常见错误与排查

**错误 1：`error: passing 'const Rectangle' as 'this' argument discards qualifiers`**

- 现象：const 对象调非 const 成员函数（或别的措辞，如今天的 `'this' argument … not marked const`）；
- 原因：非 const 函数的 this 没有 const——可能改冻结对象；
- 定位：看报错里的对象声明处有没有 const；
- 修复：二选一——该函数确实不改成员就标 const；真要改就别把对象声明成 const。

**错误 2：`undefined reference to 'Widget::alive_'`**

- 现象：链接报错找不到静态成员；
- 原因：只写了类内声明，忘了类外定义；
- 定位：链接错误的符号名带类名前缀（Day 27 套路）；
- 修复：源文件加 `int Widget::alive_ = 0;`。

**错误 3：在 const 函数里改普通成员，以为标个 mutable 就行**

- 现象：编译过了，但逻辑状态被"const 函数"改了——设计事故；
- 原因：mutable 用在了逻辑状态上（比如把 radius_ 标 mutable）；
- 定位：问自己"这个改动影响观察行为吗"；
- 修复：撤掉 mutable；确需修改的函数不要标 const。

## 7. 今日练习

### 必做练习（计划指定：带实例计数器的类）

给计数器加"累计出生数"（`static int total_`：只加不减）和"静态报表函数" `report()`（打印当前存活 + 累计出生）。预期产出：把对象收进块作用域、全部析构后调 report()，输出"存活 0、累计出生 4"——alive 归零而 total 留痕，两个 static 各司其职。

### 选做挑战

给 Circle 的缓存做"失效"：加 `void set_radius(double r)`（非 const，改 radius_ 同时 `cached_ok_ = false`）。验证：设新半径后再调 area() 会重算并打印"首次计算"——**缓存的生命周期跟着逻辑状态走**，mutable 的边界在此闭环。

## 8. 今日验收

- [ ] 我能解释 const 三级联动的传导链，并实测了 const 对象的编译拦截
- [ ] 我的 mutable 缓存实验证明"调 3 次只算 1 遍"，且能说出适用边界
- [ ] 我的实例计数器呼吸曲线与逆序销毁精确呼应（含类外定义）
- [ ] 我能默写选型三问（const / mutable / static 各管什么）
- [ ] 我知道 static 成员忘记类外定义会报什么错、怎么修

## 9. 一句话总结与明天预告

今天的问题是"类的状态怎么精细管理"；机制上，const 从变量级一路升级到对象级（三级联动），mutable 给实现细节开后门，static 把全类共享的数据搬出个体；实践上，冻结执法、缓存生效、计数器呼吸三组实验落地；验证标准是看到任何成员声明能答出"要不要 const/mutable/static"。明天 **Day 53：组合（has-a）**——类做成员：Engine + Car + Wheel，构造顺序与"全塞一个类"的优雅对比；周六的银行账户项目日把今天三件兵器全部上阵。
