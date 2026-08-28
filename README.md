# study_qt: 系统化 Qt / C++ 学习与实战项目库

本项目是一个系统化、工程化的 Qt / C++ 学习与实战代码仓库，涵盖从基础控件与语法、底层核心机制（元对象系统、事件驱动、多线程并发、Model/View 架构）、模块专题（网络、数据库、Graphics View、图表、QML/Quick、国际化与插件架构）到企业级真实场景实战的完整进阶链路。

详细学习规划与场景说明请参阅 [SYLLABUS.md](file:///d:/zcode/study/study_qt/SYLLABUS.md)。

---

## 🛠️ 开发环境与技术栈

| 组件 / 工具 | 推荐版本 / 配置 | 路径参考 (本项目环境) |
| :--- | :--- | :--- |
| **操作系统** | Windows 10 / 11 (x64) | - |
| **C++ 标准** | C++17 | MSVC `/utf-8` 编译参数 |
| **Qt 框架** | Qt 5.15.2 (MSVC 2019 x64) | `D:\app\qt\qt-5.15.2\5.15.2\msvc2019_64` |
| **编译器/工具集**| MSVC (Visual Studio / v143) | x64 架构 |
| **构建工具** | CMake 3.16 及以上 | `D:\app\CMake\bin\cmake.exe` |

---

## 📁 目录组织架构

本项目严格采用**三级工程目录结构**，所有目录与源码文件名称均采用**全小写 + 下划线**：

```text
study_qt/
├── .gitignore                     # Git 忽略配置（已忽略 app/、build/、.agents 等）
├── CMakeLists.txt                 # 根构建文件（自动扫描与注册子工程）
├── README.md                      # 项目说明文档
├── SYLLABUS.md                     # 详细的系统化学习路线与 Demo 场景说明
│
├── scripts/                       # 快速构建与运行辅助脚本
│   ├── build.ps1 / build.bat      # 一键编译脚本（支持全工程或单个 Demo）
│   └── run.ps1 / run.bat          # 运行脚本（自动配置 Qt 运行时 PATH）
│
├── app/                           # 编译输出目录（不纳入版本控制）
│   ├── debug/                     # Debug 模式生成的可执行程序与依赖
│   └── release/                   # Release 模式生成的可执行程序
│
├── code/
│   ├── 3rd/                       # 第三方依赖库（如 QCustomPlot 等）
│   └── src/                       # 核心源码（三级目录结构）
│       ├── 01_basic/              # 1 基础篇
│       │   ├── 01_project_types/  # 1.1 工程类型与构建体系
│       │   │   ├── demo_console/  # 控制台工程 Demo
│       │   │   └── demo_widget/   # 基础 Widget 窗口 Demo
│       │   ├── 02_core_types/     # 1.2 核心数据类型与工具
│       │   ├── 03_io_storage/     # 1.3 文件读写与配置持久化
│       │   └── 04_widgets/        # 1.4 常用基础控件与布局
│       │
│       ├── 02_advanced/           # 2 进阶篇
│       │   ├── 01_meta_object/    # 2.1 元对象系统与对象模型
│       │   ├── 02_event_system/   # 2.2 事件系统与事件循环
│       │   ├── 03_multithreading/ # 2.3 多线程与并发编程
│       │   ├── 04_model_view/     # 2.4 Model/View 架构体系
│       │   └── 05_painting_ui/    # 2.5 2D 绘图与界面定制
│       │
│       ├── 03_modules/            # 3 模块专题篇
│       │   ├── 01_network/        # 3.1 网络通信
│       │   ├── 02_database/       # 3.2 数据库访问
│       │   ├── 03_graphics/       # 3.3 图形视图体系
│       │   ├── 04_charts/         # 3.4 数据可视化与图表
│       │   ├── 05_ipc_process/    # 3.5 进程与系统交互
│       │   ├── 06_qml_quick/      # 3.6 现代 UI - QML & Quick
│       │   └── 07_i18n_plugins/   # 3.7 国际化与插件架构
│       │
│       └── 04_scenarios/          # 4 综合实战案例篇
│           ├── 01_tool_apps/      # 4.1 实用工具类（批量重命名、Hex 查看器等）
│           ├── 02_media_graphics/ # 4.2 图形与多媒体类（看图、播放器）
│           ├── 03_business_system/# 4.3 业务系统（仓储库存、任务看板、监控大屏）
│           └── 04_ui_frameworks/  # 4.4 界面框架（侧边栏导航、Dock工作台、无边框底座）
```

---

## 📜 编码与开发规范

1. **文件与目录命名**：
   - 文件夹全小写：`01_project_types`, `demo_console`
   - 代码文件全小写：`main.cpp`, `main_window.h`, `main_window.cpp`, `main_window.ui`
   - 每个 Demo 目录下**均包含专属的 `README.md`**，详细介绍该 Demo 的功能、技术点与运行方式。
2. **C++ 类与符号命名**：
   - 类名使用**大驼峰法**：`class MainWindow : public QMainWindow`
   - 成员变量推荐使用 `m_` 前缀：`QLabel *m_label;`
   - 槽函数命名语义清晰：`onButtonClicked()`, `handleDataReceived()`
3. **编码格式**：
   - 所有源码文件均使用 **UTF-8** 编码保存。
   - CMake 中默认开启 MSVC `/utf-8` 编译参数，杜绝中文硬编码乱码问题。
4. **子工程 CMake 注册**：
   - 每个 Demo 目录下创建专属 `CMakeLists.txt`，使用封装好的 `study_add_demo` 宏即可完成注册：
   ```cmake
   cmake_minimum_required(VERSION 3.16)

   study_add_demo(demo_name
       SRCS
           main.cpp
           main_widget.h
           main_widget.cpp
       QT_LIBS
           Qt5::Core
           Qt5::Gui
           Qt5::Widgets
   )
   ```

---

## 🚀 编译与运行指南

本项目提供了极为灵活的构建方式，支持**一键整体编译**与**按需单工程编译**。

### 方式一：使用 PowerShell 脚本（推荐）

```powershell
# 1. 编译所有 Demo（默认 Debug）
.\scripts\build.ps1

# 2. 单独编译指定 Demo
.\scripts\build.ps1 -Target demo_console
.\scripts\build.ps1 -Target demo_widget

# 3. 编译 Release 模式
.\scripts\build.ps1 -Target demo_console -Config Release

# 4. 清理并重新配置生成
.\scripts\build.ps1 -Clean

# 5. 自动拉取依赖环境 (windeployqt)
.\scripts\deploy.ps1 demo_widget
# 或者编译时直接拉取: .\scripts\build.ps1 -Target demo_widget -Deploy

# 6. 直接运行指定 Demo（自动添加 Qt DLL 到运行时 PATH）
.\scripts\run.ps1 demo_console
.\scripts\run.ps1 demo_widget
```

### 方式二：使用 Batch 批处理脚本

```cmd
:: 编译所有工程 (Debug)
scripts\build.bat

:: 编译指定工程 (Debug)
scripts\build.bat demo_console

:: 编译指定工程 (Release)
scripts\build.bat demo_console Release

:: 自动拉取依赖环境 (windeployqt)
scripts\deploy.bat demo_widget

:: 运行指定工程
scripts\run.bat demo_console
```

### 方式三：使用 CMake 原生命令

```powershell
# 配置工程
cmake -S . -B build -A x64 -T v143 -DCMAKE_PREFIX_PATH="D:\app\qt\qt-5.15.2\5.15.2\msvc2019_64"

# 编译全部
cmake --build build --config Debug

# 编译指定目标
cmake --build build --config Debug --target demo_console
```

---

## 🎯 学习路线与 Demo 概览

| 模块 | 核心内容 | 代表性 Demo |
| :--- | :--- | :--- |
| **01 基础篇** | 工程形态、常用数据类型、文件配置 I/O、基础控件与布局 | `demo_console`, `demo_widget`, `demo_mainwindow`, `demo_debug`, `demo_file`, `demo_settings`, `demo_layouts` |
| **02 进阶篇** | 元对象系统、信号槽本质、事件拦截与过滤、多线程并发、Model/View 体系、QPainter 自绘、QSS 换肤 | `demo_metaobject`, `demo_signalslot`, `demo_eventfilter`, `demo_qthread_worker`, `demo_custom_table_model`, `demo_custom_widget`, `demo_qss_styling` |
| **03 模块篇** | 网络通信、数据库、Graphics View 绘图、数据图表可视化、IPC 进程间通信、QML/Quick、国际化与插件 | `demo_tcp_server_client`, `demo_sqlite_crud`, `demo_flowchart_canvas`, `demo_qtcharts`, `demo_qcustomplot`, `demo_qml_basic`, `demo_plugins` |
| **04 实战篇** | 实用小工具（批量重命名/Hex查看器/剪贴板管理）、多媒体应用、端到端业务管理系统（仓储库存/任务番茄钟/设备看板）、现代化通用 UI 界面底座框架（Fluent侧边栏/Dock工作台/无边框） | `demo_batch_rename`, `demo_hex_viewer`, `demo_clipboard_manager`, `demo_image_viewer`, `demo_inventory_manager`, `demo_fluent_sidebar_nav`, `demo_dock_workbench` |

详细目录与场景介绍见 [SYLLABUS.md](file:///d:/zcode/study/study_qt/SYLLABUS.md)。
