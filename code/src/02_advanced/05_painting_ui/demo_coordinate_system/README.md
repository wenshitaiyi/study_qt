# Demo - 2D 空间矩阵变换、外部视口操作与几何裁剪交互平台 (demo_coordinate_system)

## 1. 简介与功能概览

本项目展示了一个基于 Qt `paintEvent` 纯自绘的工业级 2D 图形矩阵变换与视口操作交互平台。系统完整实现了二维笛卡尔空间下的齐次坐标变换、定向包围盒（OBB）几何约束、多模式光标/中心无级缩放与旋转、长宽比保持自适应重置、以及带几何裁剪沙箱的自定义外部视口体系。

界面采用专业级左右分栏架构：左侧为基于 `QScrollArea` 的参数控制面板（最大宽度支持 700px，支持自由拖拽拉伸），右侧为核心自绘画布，状态栏与微调面板实时双向联动。

本项目配有专门的深度数学原理专著文档：[MATH_PRINCIPLES.md](file:///d:/zcode/study/study_qt/code/src/02_advanced/05_painting_ui/demo_coordinate_system/MATH_PRINCIPLES.md)。

---

## 2. 界面与布局特性

- **左侧参数面板 (`QScrollArea` + `QGroupBox` 架构)**：
  - 通过 `QSplitter` 自由拉伸，最大宽度限制为 700px，确保大屏与小屏均有极佳布局体验。
  - **1. 内部图元与包围盒**：图元形状下拉选择、包围盒（OBB）显隐开关。
  - **2. 几何变换与微调**：旋转角度滑动条（`QSlider`）与微调框（`QDoubleSpinBox`）双向联动，中心坐标 $(cx, cy)$ 与尺寸 $(w, h)$ 实时微调与反向同步。
  - **3. 重置与自适应定位**：自适应范围 (Fit In View)、宽度自适应居中、高度自适应居中、摆正归零。
  - **4. 自定义外部视口**：启用外部视口开关、视口边框显隐控制、视口状态实时提示。
  - **5. 矩阵与快捷手势参考**：实时格式化输出 $3 \times 3$ 仿射矩阵元素。

---

## 3. 核心交互与手势特性

1. **图元定向包围盒（OBB）与 8 向拉伸**：
   - 随图元同步平移、旋转与缩放的虚线外框。
   - 拖拽任意手柄时，对角锚点在世界坐标系中绝对锁定保持不动，位移投影至局部旋转主轴，严格保持旋转矩形无剪切失真（Zero Shear Distortion）。
2. **内部图元无缝切换**：
   - **矩形 (Rectangle)**：圆角双色渐变与轮廓绘制。
   - **三角形 (Triangle)**：等腰/等边矢量多边形绘制。
   - **正六边形 (Hexagon)**：正多边形顶点动态生成与渐变填充。
   - **科技卡片图片 (Image)**：高分辨率矢量质感位图渲染。
3. **自由拖拽平移**：
   - 鼠标左键点击图元或外部视口主体自由平移。
4. **双模式无级滚轮缩放**：
   - `Ctrl + 滚轮`：以**当前鼠标光标点**为不动点锚点缩放。
   - `Ctrl + Shift + 滚轮`：以**图元自身中心点**为不动点缩放。
5. **双模式平滑滚轮旋转**：
   - `Alt + 滚轮`：以**当前鼠标光标点**为轴心公转旋转。
   - `Alt + Shift + 滚轮`：以**图元自身中心点**为轴心原地自转。
   - *（底层拦截 Windows 系统键并融合 `QApplication::keyboardModifiers()` 及水平/垂直滚轮增量检测，彻底保证 Alt 旋转在各平台高可靠响应）*。
6. **三种保持长宽比的重置摆正定位**：
   - 角度一键归零摆正（$\theta = 0^\circ$）。
   - **自适应范围 (Fit In View)**：等比缩放至最大容纳尺寸并正中对齐。
   - **宽度自适应居中 (Fit Width & Center)**：宽度填满目标容器，高度等比例缩放并居中。
   - **高度自适应居中 (Fit Height & Center)**：高度填满目标容器，宽度等比例缩放并居中。
7. **自定义外部视口（Custom External Viewport）**：
   - 启用后外部区域呈暗色半透明遮罩，高亮视口区域。
   - **硬件/光栅化裁剪**：图元在视口外部严格被不可见裁剪（`QPainter::setClipRect`）。
   - **外部视口支持拉伸与拖拽**：外部视口自身拥有 8 个独立调整手柄，支持拖动平移视口、拖动手柄改变视口大小。
   - 视口存在时，图元重置定位与鼠标交互严格受限于外部视口内部。

---

## 4. 核心技术点与数学算法

- **齐次坐标与仿射矩阵复合**：
  - 矩阵运算遵循：$\mathbf{P}_{world} = T(c_x, c_y) \cdot R(\theta) \cdot S(s_x, s_y) \cdot \mathbf{P}_{local}$。
  - Qt `QTransform` 行向量后乘约定与数学列向量前乘约定的转置映射。
- **光标不动点方程推演**：
  $$C_{new} = P_{cursor} \cdot (1 - k) + C_{old} \cdot k$$
- **OBB 对角固定拉伸投影算法**：
  - 计算世界位移 $\vec{D} = P_{mouse} - P_{anchor}$ 向局部单位轴 $\vec{u}, \vec{v}$ 的投影，并直接由对角线中点闭式求出新几何中心：
    $$C_{new} = \frac{P_{anchor} + P_{target}}{2}$$
- **Sutherland-Hodgman 多边形裁剪与 Viewport Culling**。

---

## 5. 编译与运行

### 编译单 Demo
```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\build.ps1 -Target demo_coordinate_system
```

### 部署运行时依赖环境 (windeployqt)
```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\deploy.ps1 demo_coordinate_system
```

### 运行 Demo
```powershell
powershell -ExecutionPolicy Bypass -File .\scripts\run.ps1 demo_coordinate_system
```
