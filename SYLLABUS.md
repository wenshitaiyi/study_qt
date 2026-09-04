# Qt 知识学习与实战大纲

> **学习目标**：构建扎实的 Qt/C++ 桌面端与跨平台开发能力，掌握从底层原理（元对象、事件循环、多线程模型）、核心框架（Model/View、Graphics View、网络与数据库）到工程实战项目的完整技术链路。
> 
> **目录与命名规范**：
> - 采用三级工程目录结构：`code/src/<顶级章节>/<二级章节>/<demo工程>`
> - 所有目录名、源文件及头文件名统一使用**全小写 + 下划线**（如 `main_window.h`, `main_window.cpp`）
> - C++ 类名使用**大驼峰命名法**（如 `MainWindow`, `DataManager`）
> - 源码文件全部使用 **UTF-8** 编码
>
> **图例说明**：
> - `✅ [已实现]`：代码已编写、通过 CMake 编译与运行验证，可直接进入对应源码目录查看与运行。
> - `⏳ [待实现]`：已规划好技术方案与应用场景，等待后续逐步实现。

---

## 1 基础篇 (01_basic)

**章节概述**：本章节面向初学者及希望巩固基础的开发者。内容涵盖 Qt 开发环境与工程创建规范、C++ 核心数据结构封装、文件与配置输入输出、常用 GUI 控件与自适应布局管理器。通过本章节学习，能够熟练使用 Qt 基础组件独立搭建各类标准桌面窗口程序。

### 1.1 工程类型与构建体系 (01_project_types)

**章节概述**：理解 Qt 针对不同应用形态提供的工程模板（控制台、纯 Widget、带菜单栏的 MainWindow、对话框等），掌握 `uic`（UI 编译器）、`moc`（元对象编译器）工作流，以及在不同工程中加载和使用 `.ui` 文件的各种范式。

#### 1.1.1 demo_console ✅ [已实现]
- **源码工程**：[code/src/01_basic/01_project_types/demo_console](file:///d:/zcode/study/study_qt/code/src/01_basic/01_project_types/demo_console/)
- **描述**：创建无 GUI 界面的 `QCoreApplication` 控制台应用程序，展示事件循环的初始化与退出、命令行参数解析（`QCommandLineParser`）。  
- **应用场景**：后台无界面服务程序、命令行批处理工具、Linux 守护进程。

#### 1.1.2 demo_widget ✅ [已实现]
- **源码工程**：[code/src/01_basic/01_project_types/demo_widget](file:///d:/zcode/study/study_qt/code/src/01_basic/01_project_types/demo_widget/)
- **描述**：基于 `QWidget` 与 Qt Designer 可视化设计文件（`main_widget.ui`）创建基础窗口部件，展示窗口坐标系、自适应垂直布局与按钮点击交互。  
- **应用场景**：轻量级悬浮窗、桌面小挂件、内嵌到其他系统中的子窗口部件。

#### 1.1.3 demo_mainwindow ✅ [已实现]
- **源码工程**：[code/src/01_basic/01_project_types/demo_mainwindow](file:///d:/zcode/study/study_qt/code/src/01_basic/01_project_types/demo_mainwindow/)
- **描述**：基于 `QMainWindow` 的经典桌面应用框架，系统演示菜单栏（QMenuBar）、工具栏（QToolBar）、状态栏（QStatusBar）、中心部件（QPlainTextEdit）以及停靠面板（QDockWidget）的协同工作机制与文件流保存打开。  
- **应用场景**：IDE 编辑器、数据分析客户端、工业监控控制台等大型桌面主程序。

#### 1.1.4 demo_dialog ✅ [已实现]
- **源码工程**：[code/src/01_basic/01_project_types/demo_dialog](file:///d:/zcode/study/study_qt/code/src/01_basic/01_project_types/demo_dialog/)
- **描述**：系统演示自定义对话框的模态（`exec()` 阻塞）与非模态（`show()` 异步）运行模式与数据交互，以及 5 种常用内置标准对话框（QFileDialog, QMessageBox, QColorDialog, QFontDialog, QInputDialog）的封装调用。  
- **应用场景**：软件设置弹窗、文件保存与导出提示、用户登录鉴权确认框。

#### 1.1.5 demo_ui_usage ✅ [已实现]
- **源码工程**：[code/src/01_basic/01_project_types/demo_ui_usage](file:///d:/zcode/study/study_qt/code/src/01_basic/01_project_types/demo_ui_usage/)
- **描述**：系统演示在 C++ 与 Visual Studio (Qt VS Tools) 中构建与使用界面的 4 种主流组织形式：
  1. **指针成员方式 (Pointer Member)**：头文件前置声明 `namespace Ui { class FormName; }`，类内持有 `Ui::FormName *ui`（或智能指针），在构造函数中 `new` 并调用 `ui->setupUi(this)`，具备最佳编译隔离性与 PIMPL 优势。
  2. **值对象成员方式 (Direct Member)**：头文件引入 `#include "ui_form_name.h"`，类内直接定义 `Ui::FormName ui;` 值对象成员，构造函数中调用 `ui.setupUi(this)`，免除堆内存分配。
  3. **多重继承方式 (Multiple Inheritance)**：类直接多重继承 `public QWidget, private Ui::FormName`，在构造函数中直接调用 `setupUi(this)`，类方法内可直接访问所有 UI 控件，无需 `ui->` 或 `ui.` 前缀。
  4. **纯代码手工构建方式 (Pure Code Layout)**：不依赖 `.ui` 文件与 `uic` 编译器，在 C++ 代码中直接 `new` 控件、配置属性、创建布局管理器（`QHBoxLayout`, `QVBoxLayout`, `QGridLayout` 等）并组织层级关系。  
- **应用场景**：深入理解 `uic` 代码生成机制、兼容 Visual Studio Qt 插件向导生成的不同代码风格、根据动态生成需求与编译耗时选择最适宜的 UI 组织范式。

### 1.2 核心数据类型与基础工具 (02_core_types)

**章节概述**：Qt 提供了独立于 STL 的高效、跨平台数据类型与辅助类，具备隐式数据共享（写时复制 Copy-on-Write）特性。本节深入掌握 Qt 常用字符串、容器、时间日期、类型抽象与日志系统的使用规范与最佳实践。

#### 1.2.1 demo_debug ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_debug](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_debug/)
- **描述**：创建纯控制台应用程序，使用 `qInstallMessageHandler` 实现全局日志重定向，支持结构化日志排版（时间戳/级别/线程ID/源文件行号）、按日期每日文件滚存（`app_YYYY-MM-DD.log`）、多线程并发写入安全保护（`QMutex`）以及控制台 ANSI 彩色输出。  
- **应用场景**：生产环境客户端的运行日志落盘、现场异常崩溃定位排查。

#### 1.2.2 demo_string ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_string](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_string/)
- **描述**：创建纯控制台应用程序，全面演示 6 大核心模块：`QString` 高效格式化与切片、`QStringView` 零拷贝视图、`QStringList` 列表批处理、`QByteArray` 字节流与 HEX/Base64 编解码、UTF-8 / GBK 编码转换及 `QRegularExpression` 正则模式提取。  
- **应用场景**：解析文本报文、跨平台中文路径读取、串口/网络字节流与字符串互相转换。

#### 1.2.3 demo_containers ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_containers](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_containers/)
- **描述**：创建纯控制台应用程序，全面演示 5 大模块：`QVector` 与 `reserve()` 内存预分配提速、`QMap` (红黑树有序) vs `QHash` (哈希表) 性能比对与自定义结构体 Key 注入 `qHash()`、`QSet` 集合运算 (交/并/差) 与高效去重、STL 风格与 Java 风格迭代器、Qt 隐式数据共享 (Copy-On-Write) 与 Detach 避坑准则。  
- **应用场景**：高频内存数据缓存、字典键值快速检索、去重与集合运算。

#### 1.2.4 demo_datetime ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_datetime](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_datetime/)
- **描述**：创建纯控制台应用程序，全面演示 5 大核心模块：`QDateTime`, `QDate`, `QTime` 常用构造与 ISO/RFC/自定义多格式逆向解析、时间加减与跨度天数/秒数差值、Unix 秒级与毫秒级时间戳互转、多时区（UTC/Local/QTimeZone）转换及 `QElapsedTimer` 高精度纳秒/微秒耗时评测。  
- **应用场景**：日志时间标记、定时任务调度判定、跨时区业务适配与算法性能基准测试。

#### 1.2.5 demo_color ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_color](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_color/)
- **描述**：系统演示 `QColor` 与 `QRgb` / `uint32_t`（无符号 32 位整型）双向转换，ARGB 与 RGBA 字节序差异与位运算通道提取合成（`qAlpha()`, `qRed()`, `(argb >> 24) & 0xFF`, `qRgba()`），HSV / HSL 色彩空间色相旋转调色，十六进制颜色字符串（`#RRGGBB`, `#AARRGGBB`）解析，以及基于感知亮度公式的 `lighter()`, `darker()` 与自动黑白文字反色判定。  
- **应用场景**：图像底层像素处理、自定义换肤与动态主题调色板、图表色彩梯度生成、网络/硬件颜色数据协议编解码。

#### 1.2.6 demo_geometry ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_geometry](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_geometry/)
- **描述**：全面演示 Qt 几何与空间计算类型体系（`QPoint / QPointF`, `QSize / QSizeF`, `QRect / QRectF`, `QMargins / QMarginsF`, `QPolygon / QPolygonF`），掌握矩形碰撞/交集/包含检测（`contains()`, `intersects()`, `intersected()`, `united()`）、保持纵横比等比例缩放（`scaled(KeepAspectRatio)`）、浮点精度对齐转换（`toAlignedRect()`）以及内/外边距边界扩张与收缩。  
- **应用场景**：自定义控件绘制排版（`paintEvent`）、画布视口缩放与平移、图像居中自适应裁剪、鼠标选区检测与图形碰撞判定。

#### 1.2.7 demo_url ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_url](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_url/)
- **描述**：使用 `QUrl` 与 `QUrlQuery` 实现网络 URL 与本地 URI 的全面解析与构建，掌握本地文件路径与 `file:///` 协议双向互转（`fromLocalFile()`, `toLocalFile()`）、Query 参数键值对提取与编码、Percent Encoding 百分号编码与特殊字符转义。  
- **应用场景**：RESTful HTTP API 请求参数拼装、网络文件下载器、跨平台本地文件协议解析、WebView 混合应用数据交互。

#### 1.2.8 demo_smart_pointers ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_smart_pointers](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_smart_pointers/)
- **描述**：深入掌握 Qt 专属智能指针（`QPointer`, `QScopedPointer`, `QSharedPointer`, `QWeakPointer`, `QSharedDataPointer`）与现代 C++ 标准库智能指针（`std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`）横向全方位对比，重点掌握 `QPointer` 针对 `QObject` 自动置 `nullptr` 的防悬挂指针机制，以及利用 `QSharedDataPointer` 手写具备隐式数据共享（写时复制 COW）特性的自定义值类型。  
- **应用场景**：异步跨线程对象安全监听、RAII 异常安全内存释放、构建高性能轻量级自定义业务数据类。

#### 1.2.9 demo_variant ✅ [已实现]
- **源码工程**：[code/src/01_basic/02_core_types/demo_variant](file:///d:/zcode/study/study_qt/code/src/01_basic/02_core_types/demo_variant/)
- **描述**：借用经典 RPG 游戏角色、背包系统、装备道具与副本掉落等概念，深入浅出地演示 Qt 万能类型 `QVariant`、`QVariantMap`、`QVariantList` 的多态存储能力，以及使用 `Q_DECLARE_METATYPE` 和 `qRegisterMetaType` 注册自定义结构体并安全打包/解包流转。  
- **应用场景**：游戏与复杂业务动态属性字典、跨线程万能消息载体、Model/View 多角色数据返回。

### 1.3 文件读写与配置持久化 (03_io_storage)

**章节概述**：掌握 Qt 对本地文件系统、路径遍历、文本流、二进制序列化、JSON 解析与配置文件存储的完整封装，掌握数据持久化与跨平台路径规范。

#### 1.3.1 demo_file ✅ [已实现]
- **源码工程**：[code/src/01_basic/03_io_storage/demo_file](file:///d:/zcode/study/study_qt/code/src/01_basic/03_io_storage/demo_file/)
- **描述**：全面演示 Qt 文件与目录常用操作核心 API：`QFile` 文件读写模式（只读、覆盖写、追加）、移动/重命名/删除/拷贝；`QFileInfo` 提取文件元数据（大小、扩展名、完整路径、修改时间、权限）；`QDir` 目录创建（`mkpath`）、删除及过滤遍历（`entryInfoList`），并提供基础递归目录遍历树形打印。  
- **应用场景**：日志文件读写、工程配置路径提取、文件属性快速核验与轻量目录扫描。

#### 1.3.2 demo_text_stream ✅ [已实现]
- **源码工程**：[code/src/01_basic/03_io_storage/demo_text_stream](file:///d:/zcode/study/study_qt/code/src/01_basic/03_io_storage/demo_text_stream/)
- **描述**：使用 `QTextStream` 实现流操纵符（Manipulators）格式化报表表格输出、多字符集编码（UTF-8, UTF-8 带 BOM, GBK/GB18030）读写无乱码处理、跨平台换行符（CRLF/LF）透明适配、CSV 报表文件结构化导出与逆向解析、以及内存字符串流（`QString` 内存缓冲）。  
- **应用场景**：读取与导出 CSV 数据报表、导出系统巡检日志文本、导入脚本文件。

#### 1.3.3 demo_datastream ✅ [已实现]
- **源码工程**：[code/src/01_basic/03_io_storage/demo_datastream](file:///d:/zcode/study/study_qt/code/src/01_basic/03_io_storage/demo_datastream/)
- **描述**：全面演示 `QDataStream` 二进制序列化体系：大小端字节序（BigEndian/LittleEndian）、Qt 容器与内置对象流式存取、操作符重载（`<<` / `>>`）实现 CAD 复杂工程文件持久化存档（`.cadproj`）、魔数（Magic Header）校验与数据格式版本向前/向后兼容机制、以及内存二进制私有网络协议封包与解包。  
- **应用场景**：自定义工程文件持久化存档（CAD/设计器项目保存）、私有协议网络/串口数据包打包与解包。

#### 1.3.4 demo_settings ✅ [已实现]
- **源码工程**：[code/src/01_basic/03_io_storage/demo_settings](file:///d:/zcode/study/study_qt/code/src/01_basic/03_io_storage/demo_settings/)
- **描述**：带 UI 界面的企业级配置管理工程。对标工业级项目架构，将 `QSettings` 深度封装为强类型、带默认值回退、支持单例访问的 `AppSettings` 配置管理类；支持 INI 格式读写、UTF-8 字符集编码、实时 INI 文件双向同步预览、一键重置默认配置，以及窗口位置与几何尺寸（Geometry / State）的自动持久化与还原。  
- **应用场景**：企业级软件全局偏好设置、服务器通信参数配置、窗口上次关闭位置与布局记忆。

#### 1.3.5 demo_json ✅ [已实现]
- **源码工程**：[code/src/01_basic/03_io_storage/demo_json](file:///d:/zcode/study/study_qt/code/src/01_basic/03_io_storage/demo_json/)
- **描述**：全面演示 `QJsonDocument`, `QJsonObject`, `QJsonArray`, `QJsonValue` 的解析、序列化、格式化缩进输出与类型校验；提供高级实用工具库：**JSON 对象深度递归合并（Deep Merge）** 与 **基于点分路径（Path-based/Dot-notation）的数据安全读取与写入更新**（如 `database.cluster[0].port`）。  
- **应用场景**：RESTful API 接口通信、前后端数据交互、差异补丁（JSON Patch）合并与配置深层项快速读写。

#### 1.3.6 demo_xml ✅ [已实现]
- **源码工程**：[code/src/01_basic/03_io_storage/demo_xml](file:///d:/zcode/study/study_qt/code/src/01_basic/03_io_storage/demo_xml/)
- **描述**：采用游戏开发思维（RPG 游戏任务系统、装备神装图鉴数据库与 NPC 分支对话树），使用基于流的高性能 `QXmlStreamReader` 与 `QXmlStreamWriter` 进行 XML 树状文档的增量高效解析与流式构造，掌握标记流处理（`StartElement`, `EndElement`, `Characters`, `Attributes`）与 XML 语法异常精准行/列号定位。  
- **应用场景**：游戏任务/装备/剧情对话配置表读取、SVG 矢量图元数据解析、Office/工业标准 XML 报文解析。

### 1.4 常用基础控件与布局管理 (04_widgets)

**章节概述**：系统掌握 Qt 提供的经典 Widget 控件库，理解布局管理器（Layout）的弹性拉伸、间距控制与自适应响应原理，能够快速拼装复杂且规范的软件交互界面。

#### 1.4.1 demo_buttons ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_buttons](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_buttons/)
- **描述**：采用 RPG 游戏战斗与控制台设计思维（技能施放 CD 倒计时、副本难度弹出菜单、阵营/画质单选、三态自动拾取勾选），综合展示 `QPushButton`, `QToolButton`（三种 PopupMode 下拉菜单与 ArrowType 箭头）, `QRadioButton`（`QButtonGroup` 互斥管理与 ID 绑定）, `QCheckBox`（双态与三态勾选）；并系统化集成与展示 Qt 消息弹窗族（`QMessageBox`：信息公告、体力警告、严重错误、分解确认与自定义复活按钮弹窗）。  
- **应用场景**：游戏技能释放控制台、系统设置开关面板、各类确认/拦截消息弹窗与操作日志展示。

#### 1.4.2 demo_layouts ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_layouts](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_layouts/)
- **描述**：系统化罗列并演示 Qt 五大核心布局管理器标准用法与 API：
  1. `QHBoxLayout` & `QVBoxLayout`：弹性拉伸（`addStretch`）、权重比例（`setStretch(1:2:1)`）、外边距与间距、单项独立对齐（`setAlignment`）。
  2. `QGridLayout`：网格行列定位、跨行跨列合并（`addWidget(w, r, c, rowSpan, colSpan)`）、行列拉伸比例。
  3. `QFormLayout`：标准两列键值表单（`addRow(label, field)`）、跨列提交按钮、响应式换行策略与标签对齐。
  4. `QStackedWidget` / `QStackedLayout`：多步骤向导分步切换（`setCurrentIndex`、`currentChanged`）。
  5. 动态调参实验室：运行时实时调节 `setContentsMargins`、`setSpacing` 与 `setStretch` 比例。  
- **应用场景**：多分辨率窗口自适应适配、分步向导（Wizard）、表单对齐与网格排版。

#### 1.4.3 demo_editors ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_editors](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_editors/)
- **描述**：系统化罗列并演示 Qt 常用输入与编辑器组件标准用法与 API：
  1. `QLineEdit`：回显模式（`Normal / Password / PasswordEchoOnEdit`）、一键清除小图标、IP/序列号格式掩码（`setInputMask`）、整型与正则校验器（`setValidator`）、自动补全（`QCompleter`）。
  2. `QSpinBox` / `QDoubleSpinBox`：前缀/后缀（`Lv.`、`¥`、`ms`）、步长（`singleStep`）、极值循环滚动（`wrapping`）、零值特殊文本（`specialValueText`）、浮点小数精度（`decimals`）。
  3. 下拉与按键捕获：`QComboBox`（绑定 `UserData`）、`QFontComboBox`（系统字体选择）、`QKeySequenceEdit`（快捷键捕获）、`QDateTimeEdit`（日历弹出视图）。
  4. 多行文本与富文本：`QTextEdit`（HTML 排版与选区光标格式化）、`QPlainTextEdit`（海量日志/代码高性能渲染与最大行数防溢出控制）。  
- **应用场景**：账号密码安全输入、参数微调与快捷键绑定、富文本排版与代码/日志控制台。

#### 1.4.4 demo_displays ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_displays](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_displays/)
- **描述**：系统化罗列并演示 Qt 常用展示与输出控件标准用法与 API：
  1. `QLabel`：富文本 HTML 排版、超链接点击拦截（`linkActivated`）、`QPixmap` 位图绘制、`QMovie` 动图播放/暂停/倍速控制。
  2. `QProgressBar`：确定进度模式、自定义格式化文本（`setFormat("%p% (%v/%m)")`）、不确定跑马灯流动模式（`setRange(0, 0)`）、垂直方向与反向增长（`setInvertedAppearance`）。
  3. `QLCDNumber`：仿真液晶时钟（`display("HH:mm:ss")`）、液晶线框风格切换（`Outline / Filled / Flat`）、进制切换（`DEC / HEX / BIN`）。
  4. 辅助与状态展示：`QToolTip` 富文本悬停气泡提示、`QWhatsThis` 上下文帮助说明（`Shift + F1`）。  
- **应用场景**：软件公告图文与超链接拦截、耗时任务进度指示与跑马灯、数字时钟与高频计数器仪表。

#### 1.4.5 demo_containers_ui ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_containers_ui](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_containers_ui/)
- **描述**：采用**主 UI + 多子 UI 组合架构**，以顶部 `QComboBox` 下拉导航为视图调度器，中央 `QStackedWidget` 承载 5 个独立子页面，系统化罗列并演示 Qt 常用容器控件：
  1. `QGroupBox`：标准分组、扁平无边框分组（`setFlat`）、带复选框自锁分组（`setCheckable`，取消勾选时自动批量禁用内部子控件）。
  2. `QTabWidget`：上/下/左/右方位切换（`setTabPosition`）、标签可关闭（`setTabsClosable`）、拖拽排序（`setMovable`）、右上角角标控件挂载（`setCornerWidget`）。
  3. `QToolBox`：抽屉式手风琴折叠面板（`addItem`、`removeItem`、`setCurrentIndex`、`currentChanged`）。
  4. `QScrollArea`：滚动视图视口、自适应缩放（`setWidgetResizable`）、滚动条策略与快速滚底。
  5. `QSplitter`：水平/垂直动态拖拽分割条（`setStretchFactor`、`setCollapsible(false)` 防止收缩为 0、`setSizes`）。  
- **应用场景**：模块化系统设置多页面路由、类似 QQ/通讯录抽屉面板、大画布滚动区域与 IDE/工作区左右分栏。

#### 1.4.6 demo_list_widget ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_list_widget](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_list_widget/)
- **描述**：`QListWidget` 专属深度实战工程（游戏道具背包与装备仓库）：
  1. **数据增删改查 (CRUD)**：动态追加（`addItem`）、指定位置插入（`insertItem`）、安全移除（`takeItem` + `delete`）、一键清空（`clear`）。
  2. **编辑与外观修饰**：双击就地编辑（`Qt::ItemIsEditable`）、品质图标徽章（`setIcon`）、多选复选框（`Qt::ItemIsUserCheckable`）、背景色与对齐、自定义隐藏元数据绑定（`setData(Qt::UserRole, ...)`）。
  3. **高级交互与避坑指南**：
     - **选择模式**：单选（`SingleSelection`）与 `ExtendedSelection`（Ctrl/Shift 连续与多选）。
     - **右键菜单与 Viewport 避坑**：`setContextMenuPolicy(Qt::CustomContextMenu)`，使用 `viewport()->mapToGlobal(pos)` 正确换算屏幕绝对坐标。
     - **信号防递归**：代码批量更新数据时使用 `blockSignals(true/false)` 防止 `itemChanged` 信号陷入死循环。
     - **实时过滤与检索**：搜索框输入关键字时动态遍历并隐藏不匹配项（`item->setHidden(!matched)`）。  
- **应用场景**：道具背包列表、下载任务清单、玩家好友列表、配置项选择器。

#### 1.4.7 demo_tree_widget ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_tree_widget](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_tree_widget/)
- **描述**：`QTreeWidget` 专属深度实战工程（游戏技能天赋树与世界地下城多级目录）：
  1. **多级层次树构建**：顶层节点（`addTopLevelItem`）与子节点层级挂载（`addChild`）、自定义多列标题（`setHeaderLabels`）。
  2. **父子级联智能勾选**：勾选父节点自动联动所有子节点全选/全不选，子节点部分勾选自动将父节点置为三态半选（`Qt::PartiallyChecked`）。
  3. **高级交互与避坑指南**：
     - **节点过滤与递归展开**：输入搜索词高亮目标节点，并自动递归展开其所有父级分支（`setExpanded(true)`）。
     - **右键动态菜单**：根据当前右键选中的是“根节点”还是“叶子技能节点”，动态弹出不同的上下文菜单项。
     - **树节点排序避坑**：`sortItems(column, order)`，重载或绑定 `Qt::UserRole` 解决数值按字符串字典序排序的经典 Bug。  
- **应用场景**：技能天赋分支树、多级关卡目录、文件目录树形结构、权限组织架构。

#### 1.4.8 demo_table_widget ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_table_widget](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_table_widget/)
- **描述**：`QTableWidget` 专属深度实战工程（游戏全服天梯排行榜与装备数值矩阵大表）：
  1. **二维表格矩阵管理**：行列动态设置（`setRowCount`, `setColumnCount`）、表头定制（`setHorizontalHeaderLabels`）、整行选中（`SelectRows`）、整列选中（`SelectColumns`）与单个单元格选择（`SelectItems`）无缝切换。
  2. **编辑触发与列宽模式**：双击编辑（`DoubleClicked`）、单击选中编辑（`SelectedClicked`）、只读锁定（`NoEditTriggers`）；列宽均分拉伸（`Stretch`）、根据内容紧凑自适应（`ResizeToContents`）与自由拖拽调节（`Interactive`）。
  3. **单元格富控件与样式**：文本对齐、只读锁定、单元格嵌入操作按钮组（`setCellWidget` 嵌入“⚔️ 挑战”/“🔍 查房”）与胜率进度条（`QProgressBar`）、金银铜牌榜首前景色与战力紫色高亮。
  4. **高级交互与避坑指南**：
     - **数值科学排序避坑**：数字列（战力、等级、排名）必须使用 `setData(Qt::DisplayRole, intVal)` 存入整型 `QVariant`，彻底避免默认按字符串字典序比较导致 `"100" < "20"` 的经典 Bug。
     - **大批量刷表防卡顿刷新**：大批量插入行数据时调用 `setUpdatesEnabled(false)` 阻断重绘，插入完成后恢复开启，耗时由数百毫秒骤降至个位数毫秒。
     - **多行安全删除**：使用降序排序 `std::greater<int>()` 倒序删除选中行，避免行索引动态偏移 Bug。
     - **右键菜单与 Viewport 避坑**：`tableWidget->viewport()->mapToGlobal(pos)` 换算屏幕绝对坐标。  
- **应用场景**：全服天梯排行榜、物品装备数值大表、学生/员工信息台账、传感器多通道监控报表。

#### 1.4.9 demo_json_tree_editor ✅ [已实现]
- **源码工程**：[code/src/01_basic/04_widgets/demo_json_tree_editor](file:///d:/zcode/study/study_qt/code/src/01_basic/04_widgets/demo_json_tree_editor/)
- **描述**：结合 `QTreeWidget` 与 `QJsonDocument` 构建专业级 JSON 树形结构可视化编辑器：
  1. **纯右键驱动与双击就地编辑**：摒弃多余侧边栏，将全部节点创建、子字段追加、重命名 Key、值修改、数据类型转换（String ↔ Number ↔ Bool ↔ Null）与删除操作完全收敛于**右键上下文菜单（Context Menu）与就地双击编辑**。
  2. **毫秒级双向实时同步**：左侧树形结构任何增删改自动触发递归逆向序列化为 `QJsonDocument`，格式化更新至右侧 `QPlainTextEdit` 源码视图；修改右侧源码亦可一键反向解析刷新树形。
  3. **持久化文件 I/O 与测试样例**：支持打开磁盘任意 `.json` 配置文件、内置 RPG 游戏配置预置样例、修改后一键同步持久化保存回写到磁盘。  
- **应用场景**：游戏/应用 JSON 配置文件可视化修改器、RESTful 响应报文结构可视化调试工具、配置热更编辑器。

---

## 2 进阶篇 (02_advanced)

**章节概述**：本章节直击 Qt 的核心底层灵魂与高级架构设计。涵盖元对象系统、信号槽本质、Qt 事件分发与过滤机制、多线程并发模型、Model/View 架构体系、QPainter 高性能自绘以及 QSS 换肤与动画系统。掌握本章内容后，具备解决卡顿、复杂架构设计及编写高性能组件的能力。

### 2.1 元对象系统与对象模型 (01_meta_object)

**章节概述**：理解 Qt 区别于标准 C++ 的核心基础设施——MOC（Meta-Object Compiler）、动态反射机制、对象树生命周期管理与信号槽连接的底层机制。

#### 2.1.1 demo_metaobject ✅ [已实现]
- **源码工程**：[code/src/02_advanced/01_meta_object/demo_metaobject](file:///d:/zcode/study/study_qt/code/src/02_advanced/01_meta_object/demo_metaobject/)
- **描述**：`QMetaObject` 元对象系统与反射机制深度实战（控制台程序）：
  1. **类层次与类元信息内省**：`className()`, `superClass()`, `inherits()`, `Q_CLASSINFO` 附加类级键值对。
  2. **静态属性系统 (Q_PROPERTY)**：声明 `READ / WRITE / NOTIFY / RESET` 属性，通过 `QMetaProperty` 遍历与 `QObject::property / setProperty` 反射读写。
  3. **运行时动态属性**：无侵入式动态附加自定义业务属性（`dynamicPropertyNames`），广泛用于 QSS 状态选择器与业务标签。
  4. **枚举双向反射 (Q_ENUM / QMetaEnum)**：字符串与枚举常量的双向高可靠无缝转换（`keyToValue` / `valueToKey`）。
  5. **方法/信号/槽内省与动态调用**：通过 `QMetaMethod` 内省方法签名与类型，通过 `QMetaObject::invokeMethod` 实现按字符串函数名动态调度带参及带返回值的方法（`Q_ARG` / `Q_RETURN_ARG`）。  
- **应用场景**：低代码属性配置面板、脚本引擎动态绑定 C++ 对象、QSS 动态状态选择器绑定、自动化测试与序列化框架。

#### 2.1.2 demo_signalslot ✅ [已实现]
- **源码工程**：[code/src/02_advanced/01_meta_object/demo_signalslot](file:///d:/zcode/study/study_qt/code/src/02_advanced/01_meta_object/demo_signalslot/)
- **描述**：`Qt` 信号与槽机制深度探索与全景实战（可视化 GUI 程序）：
  1. **语法范式与重载二义性**：Qt4 字符串宏语法（`SIGNAL/SLOT`）对比 Qt5 函数指针语法（`&Sender::sig, &Receiver::slot`）；`QOverload<int>::of` 消除同名重载二义性。
  2. **Lambda 表达式安全绑定**：无 Context 对象的潜在野指针悬空风险 vs 传入 Context（`this`）的生命周期自动断开与线程安全托管。
  3. **连接句柄与信号转发**：`QMetaObject::Connection` 句柄精准单点断开；信号转发链（`SigA -> SigB -> Slot`）。
  4. **5 种连接模式跨线程对比**：`AutoConnection`、`DirectConnection`、`QueuedConnection`、`BlockingQueuedConnection`（阻塞同步与死锁防范警告）、`UniqueConnection`（防重复绑定）。
  5. **qApp 调度与线程无锁切换**：在独立后台 `std::thread` / 子线程中调用 `QMetaObject::invokeMethod(qApp, ...)` 极速安全切回主 UI 线程更新控件。
  6. **自动命名绑定与高级特性**：`connectSlotsByName` 自动命名规范绑定（`on_btn_clicked`）、`blockSignals` 静音信号、`sender()` 触发源探测与 `receivers()` 槽计数。  
- **应用场景**：后台工作线程与主 UI 线程解耦安全通信、低代码事件总线、三方库异步回调无锁调度。

#### 2.1.3 demo_object_tree ⏳ [待实现]
**描述**：演示 `QObject` 父子关系构成的对象树系统，探索子对象析构顺序、`deleteLater` 的延迟销毁时机与野指针防范（`QPointer` 弱引用）。  
**应用场景**：动态增删 UI 控件时的内存防泄漏管理、生命周期由框架安全托管的复合对象设计。

### 2.2 事件系统与事件循环 (02_event_system)

**章节概述**：彻底理清 Qt 的事件驱动模型：系统事件如何打包为 `QEvent`，事件如何经过 `notify -> eventFilter -> event -> 专属handler` 进行层层传递与拦截，以及局部事件循环的应用技巧。

#### 2.2.1 demo_event_flow ⏳ [待实现]
**描述**：通过继承核心类重写 `QCoreApplication::notify`、`QObject::event` 以及鼠标/键盘专有事件处理器（`mousePressEvent`, `keyPressEvent`），追踪并可视化打印事件流动与消费过程。  
**应用场景**：全局输入行为监控、底层输入调试分析。

#### 2.2.2 demo_custom_event ⏳ [待实现]
**描述**：继承 `QEvent` 定义私有业务事件类型，使用 `QCoreApplication::sendEvent`（同步立即执行）与 `QCoreApplication::postEvent`（异步压入事件队列）进行组件间通信。  
**应用场景**：解耦的跨模块业务总线、非 QObject 派生类向主窗口投递业务通知。

#### 2.2.3 demo_eventfilter ⏳ [待实现]
**描述**：在目标对象或 `qApp` 上安装 `eventFilter`，实现对子控件键盘按键、焦点丢失、鼠标滚轮等事件的提前拦截与防抖。  
**应用场景**：全局快捷键拦截、表单回车自动跳到下一行输入、控件拖拽行为无侵入式增强。

#### 2.2.4 demo_eventloop ⏳ [待实现]
**描述**：使用 `QEventLoop` 在保持 UI 响应不假死的前提下实现异步操作（如等待网络请求返回、等待用户弹窗输入）的同步阻塞式编码。  
**应用场景**：同步获取远程鉴权结果的 API 封装、模态操作等待器。

### 2.3 多线程与并发编程 (03_multithreading)

**章节概述**：掌握 Qt 体系下的多线程开发规范，坚决杜绝“UI 线程阻塞”与“多线程直接操作 UI 崩溃”问题，熟练运用 Worker-Object 模式、线程池与高级并发框架。

#### 2.3.1 demo_qthread_worker ⏳ [待实现]
**描述**：使用 `QThread` 结合 `moveToThread` 将任务对象移动至独立工作线程，通过信号槽进行跨线程数据双向流转，演示规范的线程启停与退出流程。  
**应用场景**：后台长时间文件拷贝、大量数据解析、串口高频数据采集。

#### 2.3.2 demo_qthread_subclass ⏳ [待实现]
**描述**：演示继承 `QThread` 并重写 `run()` 方法的经典模式，分析其适用场景、生命周期管理以及与 `moveToThread` 的本质区别。  
**应用场景**：死循环式常驻数据监听器、硬件驱动协议轮询。

#### 2.3.3 demo_runnable_pool ⏳ [待实现]
**描述**：利用 `QRunnable` 与 `QThreadPool` 实现轻量级无事件循环的批处理任务并发执行，支持设置最大并发线程数与任务超时。  
**应用场景**：海量缩略图并发生成、批量文件哈希值计算。

#### 2.3.4 demo_qtconcurrent ⏳ [待实现]
**描述**：使用 `QtConcurrent::run`, `QtConcurrent::mapped`, `QtConcurrent::filtered` 配合 `QFuture` 和 `QFutureWatcher` 实现无需手动管理线程的高性能并行计算。  
**应用场景**：批量图像矩阵滤镜处理、大规模数据并行排序与过滤。

#### 2.3.5 demo_thread_sync ⏳ [待实现]
**描述**：实战演示 `QMutex` / `QMutexLocker`, `QReadWriteLock`（读写分离锁）, `QSemaphore`（信号量控制并发槽位）, `QWaitCondition`（条件变量生产者-消费者模型）。  
**应用场景**：多线程数据环形缓冲区、多任务协同生产与消费流水线。

### 2.4 Model/View 架构体系 (04_model_view)

**章节概述**：Qt 的 Model/View/Delegate 架构将数据存储、业务逻辑与界面渲染彻底解耦。本节掌握标准模型、自定义模型、自定义代理控件以及代理排序过滤的高级用法，解决百万级海量数据流畅展示的痛点。

#### 2.4.1 demo_standard_item_model ⏳ [待实现]
**描述**：使用 `QStandardItemModel` 分别驱动 `QListView`, `QTreeView`, `QTableView`，演示多角色（DisplayRole, CheckStateRole, DecorationRole, UserRole）的数据绑定与修改。  
**应用场景**：组织架构树形管理、商品列表带图标复选框展示。

#### 2.4.2 demo_custom_list_model ⏳ [待实现]
**描述**：继承 `QAbstractListModel` 实现轻量高效的列表模型，重写 `rowCount()`, `data()`, `flags()`, `setData()` 支持用户交互编辑与数据同步更新。  
**应用场景**：即时通讯聊天消息流列表、系统操作历史记录面板。

#### 2.4.3 demo_custom_table_model ⏳ [待实现]
**描述**：继承 `QAbstractTableModel` 绑定内存结构体数组，重写表头与数据接口，演示 10 万行级数据的毫秒级秒开渲染与动态增删行（`beginInsertRows`/`endInsertRows`）。  
**应用场景**：金融行情自选股列表、工业传感器时序数据实时监测表。

#### 2.4.4 demo_custom_tree_model ⏳ [待实现]
**描述**：基于自定义树节点（TreeNode）指针结构，完整实现支持无限层级的 `QAbstractItemModel`（重写 `index`, `parent`, `rowCount`, `columnCount`, `data`）。  
**应用场景**：复杂设备层级拓扑树、工程资源管理器目录树。

#### 2.4.5 demo_custom_delegate ⏳ [待实现]
**描述**：继承 `QStyledItemDelegate`，重写 `paint()` 自定义绘制进度条与状态标签，重写 `createEditor()` / `setEditorData()` / `setModelData()` 在单元格内嵌入下拉框与微调框编辑器。  
**应用场景**：表格行内直接嵌入下载进度条、行内状态下拉快速切换。

#### 2.4.6 demo_proxy_model ⏳ [待实现]
**描述**：使用 `QSortFilterProxyModel` 在不改变底层原始数据的情况下，实现表格的多列组合排序、模糊文本即时搜索与动态正则过滤。  
**应用场景**：表格搜索框实时筛选数据、多条件组合过滤展示。

### 2.5 2D 绘图、QSS 样式与界面定制 (05_painting_ui)

**章节概述**：脱离常规控件的外观束缚，掌握利用 `QPainter` 绘制高颜值现代自绘组件，掌握 QSS 样式表编写与动态换肤机制，以及属性动画系统。

#### 2.5.1 demo_painter_basics ⏳ [待实现]
**描述**：使用 `QPainter` 演示画笔（QPen）、画刷（QBrush）、线性/径向渐变（QGradient）、抗锯齿渲染（RenderHints）以及基本几何图形绘制。  
**应用场景**：自绘网格背景、刻度标尺、几何图元与水印绘制。

#### 2.5.2 demo_coordinate_system ✅ [已实现]
- **源码工程**：[code/src/02_advanced/05_painting_ui/demo_coordinate_system](file:///d:/zcode/study/study_qt/code/src/02_advanced/05_painting_ui/demo_coordinate_system/)
- **描述**：深入剖析 Qt 二维空间矩阵变换、视口操作与图元交互绘制系统（附专属数学原理专著）：
  1. **坐标系与矩阵变换**：齐次坐标、`QTransform` 仿射变换矩阵（平移、旋转、缩放、错切），支持光标锚点与图元中心双模式无级缩放（`Ctrl + 滚轮`、`Ctrl + Shift + 滚轮`）与平滑旋转（`Alt + 滚轮`、`Alt + Shift + 滚轮`）。
  2. **定向包围盒（OBB）与交互手柄**：图元包围盒显隐控制、8 向拉伸控制手柄（对角锚点锁死拉伸）、鼠标命中检测与自由拖拽平移。
  3. **内部图元替换**：矩形、等腰/等边三角形、正六边形及位图图片（`QPixmap`）无缝切换与自适应绘制。
  4. **保持比例重置定位**：角度归零摆正，支持自适应范围（Fit In View）、宽度自适应居中（Fit Width & Center）与高度自适应居中（Fit Height & Center）。
  5. **自定义外部视口**：可拖拽与拉伸的外部视口矩形、视口边框显隐控制，内部图元在视口区域内严格执行几何裁剪（`setClipRect`）与事件拦截。
  6. **数学原理专著**：配套独立专著文档 `MATH_PRINCIPLES.md`，深度推导二维矩阵变换、光标不动点方程、OBB 旋转拉伸与视口裁剪数学算法。  
- **应用场景**：CAD/矢量制图软件画布核心、图形排版与图像标注编辑工具、工业组态图元变换引擎。

#### 2.5.3 demo_custom_widget ⏳ [待实现]
**描述**：重写 `paintEvent` 封装一个高复用性、带动态刻度与平滑指针动画的汽车速度仪表盘与环形渐变进度条组件。  
**应用场景**：车载 HMI 仪表盘、工控上位机状态指示仪、圆形健康度打分盘。

#### 2.5.4 demo_qss_styling ⏳ [待实现]
**描述**：演示盒模型（Margin/Border/Padding）、状态伪类（`:hover`, `:pressed`, `:checked`）、子控件选择器（`::drop-down`）以及结合动态属性的一键深色/浅色主题无缝切换。  
**应用场景**：企业级客户端主题切换系统、现代扁平化与磨砂玻璃质感 UI 打造。

#### 2.5.5 demo_animation ⏳ [待实现]
**描述**：使用 `QPropertyAnimation`, `QParallelAnimationGroup`, `QSequentialAnimationGroup` 配合缓动曲线（QEasingCurve）实现窗口平滑展开、淡入淡出与弹性位移动效。  
**应用场景**：折叠侧边栏平滑弹出收起、卡片翻转与弹窗弹性弹出效果。

---

## 3 模块专题篇 (03_modules)

**章节概述**：本章节聚焦 Qt 丰富的专业扩展模块，包括网络通信（HTTP/TCP/UDP/WebSocket）、数据库访问（SQL）、图形视图框架（Graphics View）、数据可视化图表（QtCharts / QCustomPlot）、进程与 IPC 通信、现代 QML/Quick 声明式 UI 以及国际化与插件化架构。

### 3.1 网络通信 (01_network)

**章节概述**：掌握 Qt 提供的跨平台网络协议栈，从应用层的 HTTP/WebSocket 到传输层的 TCP/UDP 套接字编程。

#### 3.1.1 demo_http_client ⏳ [待实现]
**描述**：使用 `QNetworkAccessManager`, `QNetworkRequest`, `QNetworkReply` 实现 GET/POST 请求、Header 鉴权、JSON 数据提交以及大文件分块流式下载与进度反馈。  
**应用场景**：客户端软件在线检查更新与安装包下载、调用 Web API 后端服务。

#### 3.1.2 demo_tcp_server_client ⏳ [待实现]
**描述**：使用 `QTcpServer` 和 `QTcpSocket` 实现高可靠 C/S 通信架构，设计带“包头（魔数+数据长度）+ 包体”的二进制协议帧，彻底解决 TCP 粘包与分包问题。  
**应用场景**：局域网设备数据采集网关、私有协议指令交互控制台。

#### 3.1.3 demo_udp_multicast ⏳ [待实现]
**描述**：使用 `QUdpSocket` 实现无连接的高效数据传输，演示广播（Broadcast）局域网设备发现与组播（Multicast）音视频流/传感器数据同步分发。  
**应用场景**：局域网内同网段设备自动搜索与配对、多屏协同同步显示。

#### 3.1.4 demo_websocket ⏳ [待实现]
**描述**：使用 `QWebSocket` 与 `QWebSocketServer` 实现基于文本与二进制帧的双向长连接通信，支持心跳保活机制。  
**应用场景**：金融即时行情推送客户端、多人协同看板画板。

### 3.2 数据库访问 (02_database)

**章节概述**：掌握 `QtSql` 模块连接操作主流关系型数据库（SQLite, MySQL, PostgreSQL），理解事务机制与模型绑定。

#### 3.2.1 demo_sqlite_crud ⏳ [待实现]
**描述**：使用 `QSqlDatabase`, `QSqlQuery` 进行 SQLite 数据库创建、表结构初始化、预编译参数化查询（防 SQL 注入）与批量事务提交优化。  
**应用场景**：本地客户端用户数据缓存、离线数据记录存储。

#### 3.2.2 demo_sql_model ⏳ [待实现]
**描述**：使用 `QSqlTableModel` 和 `QSqlRelationalTableModel` 将数据库表直接绑定到 `QTableView`，实现 UI 表格上的直接行内编辑、增删与外键关联下拉选择。  
**应用场景**：企业内部信息管理系统（MIS）、人事/仓储库存数据管理界面。

#### 3.2.3 demo_connection_pool ⏳ [待实现]
**描述**：封装线程安全的数据库连接池单例，按需分配与回收 `QSqlDatabase` 实例，解决多线程环境下并发访问数据库报错问题。  
**应用场景**：多线程高并发数据入库服务、后台分布式日志存储。

### 3.3 图形视图体系 (03_graphics)

**章节概述**：掌握 Graphics View 框架（Scene / View / Item），针对成千上万个独立图元的快速碰撞检测、缩放变换与交互。

#### 3.3.1 demo_graphics_scene ⏳ [待实现]
**描述**：演示 `QGraphicsScene`, `QGraphicsView`, `QGraphicsItem` 的三层坐标映射，实现视口的滚轮缩放、鼠标中键拖拽平移与多图元框选。  
**应用场景**：电路原理图查看器、GIS 地图瓦片展示器。

#### 3.3.2 demo_custom_item ⏳ [待实现]
**描述**：继承 `QGraphicsItem` 绘制矢量图元，实现选中边框、八向控制点拉伸缩放、旋转以及移动时网格自动吸附对齐。  
**应用场景**：矢量绘图软件图元编辑、组态软件控件拖放设计。

#### 3.3.3 demo_flowchart_canvas ⏳ [待实现]
**描述**：实现图元节点之间的动态贝塞尔曲线连接线（Connection Line），支持连线端口吸附、节点移动时连线端点自适应重绘。  
**应用场景**：工业自动化 PLC 梯形图设计器、可视化工作流引擎编辑器。

### 3.4 数据可视化与图表 (04_charts)

**章节概述**：掌握 Qt 官方 `QtCharts` 模块与开源界最流行的高性能图表库 `QCustomPlot`，应对各种工业与金融可视化需求。

#### 3.4.1 demo_qtcharts ⏳ [待实现]
**描述**：使用 `QChartView`, `QChart`, `QSplineSeries`, `QBarSeries`, `QPieSeries` 绘制折线图、曲线图、柱状图与饼图，实现动态数据实时追加与动态坐标轴范围滚动。  
**应用场景**：系统 CPU/内存占用率实时监控曲线、销售额统计环状占比图。

#### 3.4.2 demo_qcustomplot ⏳ [待实现]
**描述**：集成第三方 `QCustomPlot` 库，演示 100 万数据点的毫秒级快速重绘、曲线游标测量（Cursor/Tracer）、区域放大缩小与多坐标轴联动。  
**应用场景**：示波器波形采集分析仪、高频振动与声学信号分析系统。

### 3.5 进程与系统交互 (05_ipc_process)

**章节概述**：掌握跨进程调用、跨进程通信（IPC）与操作系统底层能力对接。

#### 3.5.1 demo_process ⏳ [待实现]
**描述**：使用 `QProcess` 启动外部可执行程序或命令行脚本（如 ping, ffmpeg），异步捕获标准输出（stdout）和错误输出（stderr），实现命令行工具的 GUI 封装。  
**应用场景**：音视频格式转换器前端 GUI、自动化打包发布脚本执行器。

#### 3.5.2 demo_shared_memory ⏳ [待实现]
**描述**：利用 `QSharedMemory` 实现程序单实例运行（防重复启动）并向已运行实例发送唤醒指令，同时演示跨进程大块数据（如图像帧）的高速共享读写。  
**应用场景**：软件单例运行防冲突、本地多进程高速图像/视频流传输。

#### 3.5.3 demo_local_socket ⏳ [待实现]
**描述**：使用本地套接字（Windows 命名管道 / Unix Domain Socket）实现本地不同进程间的高可靠双向命令通信。  
**应用场景**：客户端主界面进程与后台服务守护进程之间的指令下发与状态同步。

#### 3.5.4 demo_system_tray ⏳ [待实现]
**描述**：使用 `QSystemTrayIcon` 创建任务栏右下角托盘图标、托盘右键菜单、最小化到托盘以及操作系统原生气泡通知（Balloon Message）。  
**应用场景**：通讯软件后台驻留、后台下载完成时的系统右下角弹窗通知。

### 3.6 现代 UI - QML 与 Qt Quick (06_qml_quick)

**章节概述**：掌握 Qt 的现代声明式 UI 开发框架 QML/Quick，理解 C++ 负责核心业务逻辑与高性能计算、QML 负责高颜值动效 UI 的混合编程范式。

#### 3.6.1 demo_qml_basic ⏳ [待实现]
**描述**：展示 QML 的核心语法、Item 基础元素、Rectangle, Text, Image, Anchors 锚点布局与 State/Transition 状态过渡动画。  
**应用场景**：移动端触控界面设计、炫酷交互卡片组件。

#### 3.6.2 demo_cpp_qml_interact ⏳ [待实现]
**描述**：通过 `setContextProperty` 向 QML 注入 C++ 单例对象、使用 `Q_PROPERTY` 实现双向属性绑定、C++ 发射信号通知 QML 更新、QML 调用 C++ `Q_INVOKABLE` 方法。  
**应用场景**：现代车载大屏 UI、智能家居中控系统的前后端分离开发架构。

#### 3.6.3 demo_quick_controls ⏳ [待实现]
**描述**：使用 Qt Quick Controls 2（Button, Slider, Switch, SwipeView, Dialog）结合 Material / Universal 风格搭建现代化桌面/移动端应用。  
**应用场景**：扁平化多端自适应工具软件、触屏控制台。

### 3.7 国际化与插件架构 (07_i18n_plugins)

**章节概述**：掌握软件走向国际化出海所需的多语言动态切换能力，以及大型桌面软件必须具备的松耦合插件化扩展体系。

#### 3.7.1 demo_i18n ⏳ [待实现]
**描述**：使用 `tr()` 标记待翻译文本，通过 `lupdate` 提取 `.ts` 文件、`lrelease` 编译 `.qm` 文件，结合 `QTranslator` 和 `qApp->installTranslator` 实现软件运行时无需重启一键无缝切换中/英/日多语言。  
**应用场景**：出海软件多语言切换、国际化商用大型系统。

#### 3.7.2 demo_plugins ⏳ [待实现]
**描述**：定义纯虚 C++ 接口类（`Q_DECLARE_INTERFACE`），编写独立的动态链接库插件（`Q_PLUGIN_METADATA`），主程序通过 `QPluginLoader` 运行时扫描目录并动态加载调用插件功能。  
**应用场景**：类似 Photoshop 滤镜插件系统、可扩展工控算法插件平台。

---

## 4 综合实战案例篇 (04_scenarios)

**章节概述**：本章节将前序所有基础与模块知识融会贯通，涵盖桌面实用小工具、图形多媒体应用、独立业务管理系统以及纯粹的通用客户端开发界面框架。重点考察架构设计能力、界面与逻辑解耦、健壮的错误处理与高质量代码规范。

### 4.1 实用工具类 (01_tool_apps)

**章节概述**：面向日常开发与桌面操作中最常用、无需特定硬件依赖的高频生产力小工具，重点训练文件处理、多线程加速、数据解析与高效 UI 交互。

#### 4.1.1 demo_batch_rename ⏳ [待实现]
**描述**：实现多规则批量文件重命名工具，支持前后缀追加、字符替换、正则表达式匹配、数字递增序号插入、扩展名修改以及实时重命名效果预览与一键撤销。  
**应用场景**：海量图片/文档批量归档整理、摄影照片按日期和序号统一格式化命名。

#### 4.1.2 demo_hex_viewer ⏳ [待实现]
**描述**：基于 Model/View 或纯自绘实现的高性能二进制/十六进制数据查看器（Hex Viewer），支持超大文件内存映射（QFile::map）快速加载、按字节/字/双字切换查看、十六进制与 ASCII 双栏联动高亮、指定偏移快速跳转与十六进制特征码搜索。  
**应用场景**：底层文件格式逆向分析、网络抓包原始数据包解析、固件及加密文件内容核验。

#### 4.1.3 demo_file_hash_tool ⏳ [待实现]
**描述**：采用多线程（QtConcurrent / QThreadPool）并行计算大文件的 MD5 / SHA1 / SHA256 / CRC32 校验和，支持拖拽文件识别、哈希值实时比对与批量导出为 CSV/JSON 报表。  
**应用场景**：软件安装包完整性校验、防篡改比对、磁盘重复大文件排查。

#### 4.1.4 demo_clipboard_manager ⏳ [待实现]
**描述**：系统级剪贴板历史管理器，监听系统剪贴板（QClipboard）变动，支持文本/富文本/图片等多类型历史记录自动缓存、模糊快速搜索、分类标签置顶与双击快捷回填。  
**应用场景**：日常高频文本与截图剪贴板历史追溯、常用代码片段/文本模板快速调取。

#### 4.1.5 demo_file_watcher ⏳ [待实现]
**描述**：基于 `QFileSystemWatcher` 开发带界面的实时文件系统监控器，支持动态添加/移除受控目录与文件，实时侦听文件修改、文件删除、目录增删子文件事件，并以带时间戳彩色日志流与状态面板可视化告警。  
**应用场景**：热重载服务配置监听、日志目录新文件自动捕获、代码防篡改监控。

#### 4.1.6 demo_file_dialog_system ⏳ [待实现]
**描述**：基于自定义递归遍历算法深度扫描目录树，结合 `QFileDialog` 实现全功能文件交互面板：包含特定文件类型后缀过滤器、单选与批量多选、保存/覆盖防误删确认、智能自动定位默认路径与高亮预选、以及整目录选取与文件类型占比统计。  
**应用场景**：IDE 打开工程与导出向导、批量图片导入选择器、文件类型分析报表。

#### 4.1.7 demo_file_explorer ⏳ [待实现]
**描述**：基于 `QFileSystemModel` 定制开发类 Windows 文件资源管理器：支持左侧目录导航树 + 右侧多视图联动切换（大图标/小图标/列表/详细信息表格）、多 Tab 标签页浏览、顶部地址栏面包屑导航与手动绝对路径跳转、前进/后退/上一级历史栈、以及深浅主题外观切换。  
**应用场景**：桌面端跨平台定制文件管理器、嵌入式工控机本地文件浏览模块。

#### 4.1.8 demo_ditto_clipboard ⏳ [待实现]
**描述**：完全对标经典开源神器 Ditto 的桌面级工业增强剪贴板：
1. **多模态数据监听**：通过 `QClipboard` 与原生 Win32 消息全面捕获纯文本、富文本/HTML、图片位图（Blob 缓存）与文件路径列表。
2. **SQLite 数据库持久化**：历史记录无损存盘、分页异步检索、置顶收藏（Star/Pin）与分类标签。
3. **全局唤醒热键**：基于 Windows API `RegisterHotKey` 与 `QAbstractNativeEventFilter` 实现全局快捷键（如 `Ctrl + ~` / `Alt + V`）快速呼出无边框悬浮窗。
4. **智能秒级回填粘贴**：用户双击或回车选中记录后，窗口自动失焦隐藏，并调用 Win32 `SendInput` 自动向目标激活窗口模拟 `Ctrl + V` 粘贴。
5. **系统托盘与自适应交互**：`QSystemTrayIcon` 常驻托盘、鼠标邻近自动弹窗与失焦自动隐藏。  
**应用场景**：跨应用极速代码与文案多段复制粘贴、高频日常桌面生产力增强工具。

#### 4.1.9 demo_tree_editor ⏳ [待实现]
**描述**：通用树形配置引擎与可视化编辑器（带完整 UI 界面）：
1. **通用树形模型 (`GenericTreeNode`)**：设计内部层次化树形节点数据结构，抽象统一节点标签、属性映射表（Attributes）与子节点列表。
2. **多配置格式无缝双向导入导出**：原生支持 **JSON 树与 XML 树双向无损互转**（Import JSON -> Export XML / Import XML -> Export JSON）。
3. **可视化交互与实时渲染**：基于 `QTreeWidget` / `QTreeView` 提供节点可视化增删改查、属性就地编辑修改、以及左右分栏实时预览生成的 JSON 和 XML 文本流。  
**应用场景**：游戏剧情/关卡/技能树通用可视化配置器、跨平台多格式配置文件转换工具。

#### 4.1.10 demo_ui_notifier ⏳ [待实现]
**描述**：工业级通用桌面 UI 消息悬浮通知/Toast 动画小框架（带完整 UI 演示）：
1. **4 级状态通知**：集成 Info（蓝色通知）、Success（绿色成功）、Warning（黄色警告）、Error（红色异常）。
2. **多气泡层叠管理与平滑动画**：支持屏幕右下角/右上角多气泡排队堆叠、进入/移出平滑淡入淡出与位移动画（`QPropertyAnimation` / `QParallelAnimationGroup`）、鼠标悬浮暂停倒计时、定时自动关闭。
3. **单例与跨线程极速调用**：封装极简调用 API（如 `UiNotifier::success("强化成功 +15！")`、`UiNotifier::warning("体力不足！")`），支持子线程通过事件派发安全弹窗。  
**应用场景**：游戏/桌面应用全局非阻塞浮窗消息提醒、操作成功反馈 Toast、下载/网络状态异步提醒。

### 4.2 图形与多媒体类 (02_media_graphics)

**章节概述**：实现桌面端常见的高性能图片浏览与媒体播放器应用，强化多媒体、双缓冲绘制与复杂图形交互的掌控力。

#### 4.2.1 demo_image_viewer ⏳ [待实现]
**描述**：基于 Graphics View 框架实现支持超大图片平滑无级缩放、双缓冲拖拽平移、底部缩略图悬浮列表、基础滤镜（灰度/反色/亮度调节）以及矩形/箭头/文字轻量标注功能。  
**应用场景**：医疗影像查看器、工业质检瑕疵图像快速复核工具、日常轻量看图标注软件。

#### 4.2.2 demo_media_player ⏳ [待实现]
**描述**：使用 `QMediaPlayer` 与 `QVideoWidget` 构建多媒体播放器，支持本地音频/视频及网络流媒体播放、播放列表增删与排序、音量平滑渐变调节、进度条悬停时间预览与全屏无边框切换。  
**应用场景**：本地影音播放器、远程监控 RTSP/MP4 视频流回放客户端。

#### 4.2.3 demo_array2d_visualizer ⏳ [待实现]
**描述**：面向科学计算、传感器阵列与热力图分析的海量 2D 数组交互可视化工作台：
1. **多重连续过程噪声数据引擎**：内置 Perlin 梯度噪声、分形布朗运动 (fBm)、Worley 细胞噪声与多频正余弦谐波干涉场，支持自定义空间矩形映射范围与百万级阵列维度。
2. **专业科学色带体系**：集成 ScientificColourMaps8 标准，封装公共色带管理模块（`code/public/colourmap`），支持色阶采样、离散/连续插值与色条实时渲染。
3. **LOD 多级视距高性能自绘画布**：利用离屏贴图快照与视口裁剪（Viewport Culling），实现百万方格在平移缩放下的 60 FPS 流畅重绘。
4. **全套交互与动效**：支持以鼠标光标为锚点的无级缩放（Ctrl + 滚轮）、拖拽平移、数值区间色彩过滤、单元格边框定制、鹰眼小地图视野联动、单元格内自适应数值反色绘制以及光标圆域网格浮起与立体投影动效。  
**应用场景**：地质地理热力分布图、气象雷达回波图、工况温度云图、高维传感器点阵监测。

### 4.3 业务系统 (03_business_system)

**章节概述**：面向实际业务需求场景构建的完整小型端到端业务系统，重点打通数据库持久化、业务逻辑层（Service/DAO）、表单校验、多状态流转与数据可视化报表。

#### 4.3.1 demo_inventory_manager ⏳ [待实现]
**描述**：轻量级商品/资产仓储库存管理系统，包含商品出入库登记、库存预警阈值提醒、分类统计柱状图/饼图分析、SQLite 数据库驱动的增删改查与 Excel/PDF 报表一键导出。  
**应用场景**：小微企业资产管理、电子元器件库房出入库台账系统。

#### 4.3.2 demo_task_pomodoro ⏳ [待实现]
**描述**：个人待办任务清单与番茄钟时间管理系统（GTD + Pomodoro），支持多分类任务看板（待办/进行中/已完成拖拽流转）、任务到期提醒、番茄钟倒计时与每日/每周专注时长统计热力图。  
**应用场景**：个人生产力效率提升工具、团队日常轻量任务看板。

#### 4.3.3 demo_device_monitor ⏳ [待实现]
**描述**：模拟工业设备状态监控大屏系统（无需外部物理硬件），通过内置模拟数据发生器生成时序数据，包含自定义仪表盘显示、多通道传感器实时动态曲线、设备在线状态与告警日志滚动展示。  
**应用场景**：上位机运行状态监控中心、新能源测试台数据看板。

### 4.4 界面框架 (04_ui_frameworks)

**章节概述**：聚焦于纯粹的桌面客户端架构底座与通用界面框架设计，剥离具体业务逻辑，提供高复用、易扩展、模块化现代客户端 UI 骨架。

#### 4.4.1 demo_fluent_sidebar_nav ⏳ [待实现]
**描述**：现代扁平化侧边栏导航框架（类似 Win11 Fluent / Web 现代后台），支持侧边栏折叠/展开动画、多页面路由切换（QStackedWidget）、面包屑导航、全局深浅色主题无缝切换与徽标通知。  
**应用场景**：现代化后台管理系统客户端、多功能综合软件主界面底座。

#### 4.4.2 demo_dock_workbench ⏳ [待实现]
**描述**：对标 Visual Studio 2022 / Qt Creator 专业级多窗口停靠拖拽合并工作台框架：
1. **任意拖拽与停靠定位**：支持上下左右任意停靠区（`Qt::DockWidgetAreas`）、从主窗口脱离独立悬浮（Floating）、拖动时高亮停靠锚点指示。
2. **窗体合并与分栏拆分**：支持多 Dock 窗体拖拽合并为标签页组（`tabifyDockWidget`）、嵌套动态水平/垂直分栏拆分（`splitDockWidget`）。
3. **中心多文档编辑区**：基于 `QMdiArea` / 多 Tab 标签页提供多文档/代码/视图编辑区，支持视图拆分与快捷关闭。
4. **布局方案持久化保存与一键恢复**：基于 `saveState()` / `restoreState()` 将用户自定义拖拽排版的布局方案（如【默认开发布局】、【调试分析布局】、【纯净代码布局】）序列化保存至本地配置，并支持一键重置与方案切换。
5. **全局视图菜单联动**：菜单栏自动生成所有停靠窗体的显隐勾选控制（`toggleViewAction()`）。  
**应用场景**：IDE、工业仿真上位机、CAD/GIS 专业建模软件、数据分析多屏看板主框架。

#### 4.4.3 demo_frameless_window ⏳ [待实现]
**描述**：跨平台无边框高颜值窗口框架，实现 Windows 原生级阴影、八向鼠标拉伸缩放、标题栏双击最大化、系统贴边吸附（Aero Snap）以及自定义标题栏按钮。  
**应用场景**：高颜值现代客户端、沉浸式播放器/游戏启动器外壳。
