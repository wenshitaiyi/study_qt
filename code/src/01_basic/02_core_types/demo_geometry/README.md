# demo_geometry (几何与空间计算全体系实战工程)

## 1. 简介与功能
本 Demo 是一个纯控制台（`QCoreApplication`）应用程序，深入剖析 Qt 几何与空间计算类型体系（`QPoint`, `QSize`, `QRect`, `QMargins`, `QPolygon`），涵盖空间距离计算、纵横比缩放自适应、矩形相交/包含/并集碰撞算法、边距扩张收缩及多边形点命中检测。

## 2. 核心技术点与模块划分

| 模块序号 | 技术专题 | 核心 API / 知识点 | 解决的工程问题 |
| :--- | :--- | :--- | :--- |
| **模块 1** | **坐标点与距离运算** | `QPoint`, `QPointF`, `manhattanLength()`, `QLineF::length()` | 鼠标拖拽启动阈值判断、欧氏空间精确距离计算。 |
| **模块 2** | **尺寸自适应缩放** | `QSize`, `scaled(KeepAspectRatio)`, `scaled(KeepAspectRatioByExpanding)` | 图像居中自适应展示（留黑边）与铺满视口裁剪排版算法。 |
| **模块 3** | **矩形碰撞与锚点对齐** | `QRect`, `intersects()`, `intersected()`, `united()`, `contains()`, `moveCenter()` | 图元碰撞检测、重叠区域计算、弹窗/子组件快速优雅居中。 |
| **模块 4** | **外边距与内边距** | `QMargins`, `marginsRemoved()`, `marginsAdded()` | 扣除标题栏/工具栏计算主内容工作区（ContentRect）。 |
| **模块 5** | **多边形与选区几何** | `QPolygon`, `boundingRect()`, `containsPoint(Qt::OddEvenFill)` | 复杂异形区域鼠标点击拾取判定、CAD 选区包围盒计算。 |

## 3. 适用场景
- 自定义控件自绘（`paintEvent`）与坐标布局排版。
- 图像查看器视口自适应、居中与缩放显示。
- 图形画布鼠标框选、碰撞判定与几何图元拓扑计算。

## 4. 运行与验证
```powershell
# 编译
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_geometry

# 运行
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_geometry
```
