# demo_ui_usage (界面组织形式全对比工程)

## 1. 简介与功能
本 Demo 系统演示在 Qt / C++ 开发以及 Visual Studio (Qt VS Tools) 中构建界面的 **4 种主流组织形式**，对比各形式在头文件依赖、编译隔离、内存分配及代码书写上的差异。

## 2. 核心技术点与 4 种形式对比

| 组织形式 | 头文件包含 | 成员变量声明 | 初始化调用 | 控件访问方式 | 核心优劣势分析 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **1. 指针成员方式**<br>(Pointer Member) | 前置声明<br>`namespace Ui { class Form; }` | `std::unique_ptr<Ui::Form> m_ui;` | `m_ui->setupUi(this)` | `m_ui->btn->setText(...)` | **优势**：编译隔离性最佳（PIMPL），修改 UI 文件不引发外部头文件重编。<br>**劣势**：需管理堆指针。 |
| **2. 值对象成员方式**<br>(Direct Member) | 直接包含<br>`#include "ui_form.h"` | `Ui::Form m_ui;` | `m_ui.setupUi(this)` | `m_ui.btn->setText(...)` | **优势**：无堆内存开销，生命周期与主对象完全绑定。<br>**劣势**：头文件耦合度高。 |
| **3. 多重继承方式**<br>(Multiple Inheritance) | 直接包含<br>`#include "ui_form.h"` | 无需声明变量<br>(继承 `private Ui::Form`) | `setupUi(this)` | `btn->setText(...)` | **优势**：访问控件无需任何前缀，书写最简。<br>**劣势**：类命名空间污染，可能与私有成员名冲突。 |
| **4. 纯代码手工构建**<br>(Pure Code Layout) | 仅包含所需控件头文件<br>`#include <QLabel>` 等 | 直接定义控件指针<br>`QLabel *m_label;` | 在构造函数直接 `new` 组装布局 | `m_label->setText(...)` | **优势**：动态构建能力强，无需 `uic` 预编译。<br>**劣势**：无可视化排版，代码量相对较大。 |

## 3. 适用场景
- 深入理解 `uic` 工具的代码生成机理。
- 兼容 Visual Studio Qt 插件新建类向导中的不同 UI 选项。
- 针对项目架构（大型工程防编译级联重编 vs 动态动态插件化组件开发）选择最佳 UI 构建模式。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_ui_usage

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_ui_usage
```
