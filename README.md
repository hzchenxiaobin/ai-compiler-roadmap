# C++ 从基础到深入 · 144 天学习计划

这是一个面向零基础学习者的 **24 周、144 天 C++ 实战学习计划**，目标是从第一行 Hello World 一路走到亲手交付一个带单元测试与 CMake 的 JSON 解析器。

学习主线：

```text
C++ 基础语法（第 1~6 周）
  → 指针与内存（第 7~8 周）
  → 面向对象（第 9~13 周）
  → STL 与泛型（第 14~17 周）
  → 现代 C++（第 18~21 周）
  → JSON 解析器综合实战（第 22~24 周）
```

## 在线阅读

站点部署在 GitHub Pages：<https://hzchenxiaobin.github.io/ai-compiler-roadmap/>

每天都包含：

- **学习**：当天需要理解的概念，配通俗解释、类比与准确技术定义；
- **实践**：可运行的最小例子、真实运行输出与逐步讲解；
- **图解**：每天至少 3 张技术信息图（SVG）；
- **验收**：练习、常见错误与自查清单。

## 文档

完整日程见：[C++按天学习计划.md](./C++按天学习计划.md)

## 项目结构

```text
├── C++按天学习计划.md   # 144 天完整计划（权威来源）
├── tutorials/           # 每日图文教程
│   └── day-001/
│       ├── README.md    # 当天教程正文
│       ├── examples/    # 可编译运行的示例代码
│       └── images/      # 教程配图（SVG）
├── skills/              # 教程生成技能与质量清单
├── static/              # 站点样式与脚本
└── tools/build.py       # 静态站点构建脚本
```

## 本地构建

```bash
python3 tools/build.py
```

构建产物输出到 `public/`，可直接用任意静态服务器预览：

```bash
python3 -m http.server -d public 8000
```

推送到 `main` 分支后，GitHub Actions 会自动重新构建并部署到 GitHub Pages。
