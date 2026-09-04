# 二维空间矩阵变换、视口操作与几何裁剪数学原理深度专著

本文档全面阐述 2D 矢量图形渲染引擎中涉及的坐标系统分层、齐次坐标、仿射变换矩阵复合、任意轴心点动量变换、定向包围盒（OBB）8向拉伸算法、视口变换与几何裁剪等底层数学原理，并结合 Qt `QTransform` 与 `QPainter` 的实际工业实现进行严格推导。

---

## 1. 多层级坐标系统模型 (Hierarchical Coordinate Systems)

在现代交互式 2D 图形与 CAD 系统中，点与几何对象并非存在于单一坐标系中，而是穿梭于以下 4 个层级的坐标空间：

```
+-------------------------------------------------------------+
| 物理设备 / 屏幕坐标系 (Device / Screen Coordinate Space)    |
| 原点: 屏幕左上角 (0, 0), 单位: 物理像素 (Physical Pixels)   |
+------------------------------+------------------------------+
                               | Widget::mapFromGlobal / mapToGlobal
                               v
+-------------------------------------------------------------+
| 控件窗口坐标系 (Widget Coordinate Space)                     |
| 原点: QWidget 左上角 (0, 0), X轴向右, Y轴向下               |
+------------------------------+------------------------------+
                               | Viewport Transform & Clipping
                               v
+-------------------------------------------------------------+
| 自定义外部视口坐标系 (Custom Viewport Space, V)             |
| 视口矩形 R_vp = [vx, vy, vw, vh]，承载局部观察与裁剪边界    |
+------------------------------+------------------------------+
                               | Affine Transform (M)
                               v
+-------------------------------------------------------------+
| 图元局部/模型坐标系 (Local / Model Coordinate Space, L)     |
| 原点位于图元几何中心 (0, 0), 本地尺寸宽 w, 高 h             |
| 局部包围盒: [-w/2, -h/2, w, h]                             |
+-------------------------------------------------------------+
```

### 1.1 为什么局部坐标系以中心为原点？
若以局部左上角为原点 $[0, 0, w, h]$，则在进行旋转 $\theta$ 或等比缩放 $s$ 时，物体必然产生非预期的偏心公转。若将原点定义在图元几何中心：
$$P_{local} \in \left[-\frac{w}{2}, \frac{w}{2}\right] \times \left[-\frac{h}{2}, \frac{h}{2}\right]$$
则旋转矩阵可直接作用于局部坐标，且图元中心与局部原点天然重合，极大地简化了矩阵推导与数值稳定性。

---

## 2. 齐次坐标与二维仿射变换矩阵 (2D Affine Transformation)

### 2.1 齐次坐标 (Homogeneous Coordinates)
在二维笛卡尔坐标系中，点表示为 $(x, y)$。为了将**非线性平移（Translation）**与**线性变换（Rotation / Scaling / Shear）**统一在同一矩阵乘法运算下，引入齐次坐标，将二维向量升维至三维：
$$\mathbf{P} = \begin{bmatrix} x \\ y \\ 1 \end{bmatrix}$$

### 2.2 基础变换矩阵（标准列向量约定）
1. **平移矩阵 (Translation) $T(d_x, d_y)$**：
   $$T(d_x, d_y) = \begin{bmatrix} 1 & 0 & d_x \\ 0 & 1 & d_y \\ 0 & 0 & 1 \end{bmatrix}$$
2. **旋转矩阵 (Rotation) $R(\theta)$**（顺时针为正，符合计算机屏幕 Y 轴向下的屏幕坐标系）：
   在屏幕坐标系（X 轴向右、Y 轴向下）下，旋转角 $\theta$ 的变换公式为：
   $$x' = x \cos\theta - y \sin\theta$$
   $$y' = x \sin\theta + y \cos\theta$$
   矩阵形式：
   $$R(\theta) = \begin{bmatrix} \cos\theta & -\sin\theta & 0 \\ \sin\theta & \cos\theta & 0 \\ 0 & 0 & 1 \end{bmatrix}$$
3. **缩放矩阵 (Scaling) $S(s_x, s_y)$**：
   $$S(s_x, s_y) = \begin{bmatrix} s_x & 0 & 0 \\ 0 & s_y & 0 \\ 0 & 0 & 1 \end{bmatrix}$$

### 2.3 复合变换与非对易性 (Matrix Non-commutativity)
矩阵乘法不满足交换律（$A \cdot B \neq B \cdot A$）。一个位于局部中心原点的图元，经过缩放 $S$、旋转 $R$、再平移至世界坐标系中心 $C(c_x, c_y)$，其变换顺序必须是：
$$\mathbf{P}_{world} = T(c_x, c_y) \cdot R(\theta) \cdot S(s_x, s_y) \cdot \mathbf{P}_{local}$$
展开复合矩阵 $M$：
$$M = \begin{bmatrix} 1 & 0 & c_x \\ 0 & 1 & c_y \\ 0 & 0 & 1 \end{bmatrix} \begin{bmatrix} \cos\theta & -\sin\theta & 0 \\ \sin\theta & \cos\theta & 0 \\ 0 & 0 & 1 \end{bmatrix} \begin{bmatrix} s_x & 0 & 0 \\ 0 & s_y & 0 \\ 0 & 0 & 1 \end{bmatrix} = \begin{bmatrix} s_x \cos\theta & -s_y \sin\theta & c_x \\ s_x \sin\theta & s_y \cos\theta & c_y \\ 0 & 0 & 1 \end{bmatrix}$$

### 2.4 Qt `QTransform` 行向量体系对照
> [!IMPORTANT]
> 数学教材通常采用**列向量前乘约定**：$\mathbf{P}' = M \mathbf{P}$。
> 而 Qt 的 `QTransform` 内部采用**行向量后乘约定**：
> $$\mathbf{P}' = \mathbf{P} \cdot M_{Qt}$$
> 其 $3 \times 3$ 矩阵定义为：
> $$\begin{bmatrix} x' & y' & 1 \end{bmatrix} = \begin{bmatrix} x & y & 1 \end{bmatrix} \begin{bmatrix} m_{11} & m_{12} & m_{13} \\ m_{21} & m_{22} & m_{23} \\ m_{31} & m_{32} & m_{33} \end{bmatrix}$$
> 其中平移项位于最后一行（$m_{31} = d_x, m_{32} = d_y$），它与列向量矩阵互为**转置关系**。在 Qt 中调用 `transform.translate(dx, dy).rotate(deg).scale(sx, sy)` 时，底层按行向量链式后乘生效。

---

## 3. 围绕任意轴心点 (Pivot Point) 的变换原理

无论是 CAD 旋转、还是以鼠标为锚点的地图缩放，本质都是**围绕非原点的任意参考点 $P_{pivot} = (p_x, p_y)$ 进行变换**。

### 3.1 任意轴心点变换定理
设任意变换矩阵为 $F$（如纯旋转 $R$ 或纯缩放 $S$），若要求变换围绕点 $P_{pivot}$ 发生，则变换矩阵必须分解为三步：
1. **平移基底**：将轴心点平移至坐标系原点：$T(-p_x, -p_y)$
2. **执行核心变换**：在原点处应用变换：$F$
3. **逆平移恢复**：将坐标原点平移回原始轴心点：$T(p_x, p_y)$

数学算式：
$$M_{pivot} = T(p_x, p_y) \cdot F \cdot T(-p_x, -p_y)$$

### 3.2 鼠标滚轮光标不动点缩放推导
在滚轮缩放时，用户的心理模型是：**“鼠标光标指向画布上的哪个特征点，缩放后该特征点依然精确停留在该光标下方”**。
设当前鼠标在屏幕/画布上的坐标为 $P_{cursor} = (x_c, y_c)$。
图元当前中心为 $C_{old}$，当前缩放比为 $s_{old}$，滚轮带来缩放倍率因子 $k$（如放大 $1.15$，缩小 $1/1.15$）。
新缩放比为：
$$s_{new} = s_{old} \cdot k$$
图元中心 $C_{new}$ 的补偿公式推导：
由于缩放以 $P_{cursor}$ 为不动点：
$$P_{cursor} - C_{new} = k \cdot (P_{cursor} - C_{old})$$
移项解得图元新中心位置：
$$C_{new} = P_{cursor} - k \cdot (P_{cursor} - C_{old}) = P_{cursor} \cdot (1 - k) + C_{old} \cdot k$$
- 当 $k = 1$ 时，$C_{new} = C_{old}$（无变化）。
- 当放大 $k > 1$ 时，中心 $C_{new}$ 远离光标点移动，使得光标处的局部细节向外膨胀。

### 3.3 图元自身中心缩放与旋转（Shift 组合键）
当按下 `Shift` 键时，轴心点直接取图元当前中心：
$$P_{pivot} = C(c_x, c_y)$$
此时：
$$C_{new} = C(c_x, c_y) \cdot (1 - k) + C(c_x, c_y) \cdot k = C(c_x, c_y)$$
图元中心保持绝对不动，完全在原地完成自发性的自转或中心缩放。

---

## 4. 定向包围盒 (OBB) 与 8 向手柄拉伸算法

### 4.1 OBB (Oriented Bounding Box) 几何定义
轴对齐包围盒（AABB）无法表达旋转物体的真实轮廓。定向包围盒拥有中心点 $C(c_x, c_y)$、未旋转时的局部宽度 $W$ 和高度 $H$、以及旋转角度 $\theta$。

图元局部 4 个顶点坐标定义：
$$V_0 = \left(-\frac{W}{2}, -\frac{H}{2}\right), \quad V_1 = \left(\frac{W}{2}, -\frac{H}{2}\right), \quad V_2 = \left(\frac{W}{2}, \frac{H}{2}\right), \quad V_3 = \left(-\frac{W}{2}, \frac{H}{2}\right)$$
通过变换矩阵 $M = T(C) \cdot R(\theta)$，即可精确映射出世界空间下的 4 个角点与 4 个边缘中点。

### 4.2 旋转状态下的 8 向拉伸推导（对角锚点固定）
用户拉伸某个控制点（例如右边缘中点、或右下角点）时，最符合直觉的几何约束是：**被拉伸点的“对角锚点（Opposite Anchor Point）”在世界坐标系中保持绝对不动**。

设对角锚点在世界坐标系下的绝对坐标为 $P_{anchor}$。
拉伸时鼠标当前在世界坐标系下的位置为 $P_{mouse}$。

1. **将鼠标向量转换到图元局部轴向**：
   图元的局部两个主轴单位向量为：
   $$\vec{u} = \begin{bmatrix} \cos\theta \\ \sin\theta \end{bmatrix}, \quad \vec{v} = \begin{bmatrix} -\sin\theta \\ \cos\theta \end{bmatrix}$$
   从固定锚点到当前鼠标位置的位移向量：
   $$\vec{D} = P_{mouse} - P_{anchor}$$
2. **向局部主轴投影获取新尺寸**：
   - 沿宽度主轴投影：$L_u = \vec{D} \cdot \vec{u}$
   - 沿高度主轴投影：$L_v = \vec{D} \cdot \vec{v}$
3. **手柄类型分类解算**：
   - **单轴拉伸（如东手柄 E、西手柄 W）**：保持高度 $H$ 不变，宽度 $W_{new} = |L_u|$。
   - **单轴拉伸（如北手柄 N、南手柄 S）**：保持宽度 $W$ 不变，高度 $H_{new} = |L_v|$。
   - **角点双轴拉伸（NE, SE, SW, NW）**：同时更新 $W_{new} = |L_u|$ 与 $H_{new} = |L_v|$。
4. **求解图元新中心点 $C_{new}$**：
   由于对角锚点 $P_{anchor}$ 与新鼠标拉伸点 $P_{target}$ 构成新矩形的对角线，新中心点天然为对角线中点：
   $$C_{new} = \frac{P_{anchor} + P_{target}}{2}$$
   此算法完全在旋转坐标系下闭环，**不仅运算量极低，而且完全消除了剪切失真（No Shear Deformation）**。

---

## 5. 保持长宽比的摆正重置定位算法 (Aspect-Ratio Preserving Fit)

当触发“重置定位”时，系统强制将旋转角归零（$\theta = 0$）。
此时需要将原始几何尺寸为 $(w_0, h_0)$ 的图元，在目标容器矩形 $R_{container} = [X_0, Y_0, W_{target}, H_{target}]$ 中按固定长宽比 $AR = \frac{w_0}{h_0}$ 适配。

容器中心为：
$$C_{target} = \left( X_0 + \frac{W_{target}}{2}, Y_0 + \frac{H_{target}}{2} \right)$$
设安全边距保留系数为 $\eta \in (0, 1]$（例如 $\eta = 0.9$）。

### 5.1 自适应范围 (Fit In View / Best Fit)
要求图元在不改变纵横比的前提下，以最大尺寸完整容纳于容器内：
$$s = \min\left( \frac{W_{target} \cdot \eta}{w_0}, \frac{H_{target} \cdot \eta}{h_0} \right)$$
$$W_{new} = w_0 \cdot s, \quad H_{new} = h_0 \cdot s$$
$$C_{new} = C_{target}$$

### 5.2 宽度自适应居中 (Fit Width & Center)
要求图元的宽度填满容器可用宽度，高度等比例缩放，并垂直居中：
$$s = \frac{W_{target} \cdot \eta}{w_0}$$
$$W_{new} = W_{target} \cdot \eta, \quad H_{new} = \frac{W_{new}}{AR}$$
$$C_{new} = C_{target}$$

### 5.3 高度自适应居中 (Fit Height & Center)
要求图元的高度填满容器可用高度，宽度等比例缩放，并水平居中：
$$s = \frac{H_{target} \cdot \eta}{h_0}$$
$$H_{new} = H_{target} \cdot \eta, \quad W_{new} = H_{new} \cdot AR$$
$$C_{new} = C_{target}$$

---

## 6. 自定义外部视口与几何裁剪 (Viewport Mapping & Clipping)

### 6.1 外部视口数学模型
视口矩形定义为：
$$\mathcal{V} = \{ (x, y) \mid x_{min} \le x \le x_{max}, \; y_{min} \le y \le y_{max} \}$$
在 Demo 中，外部视口自身也是一个独立的几何实体，其位置 $(v_x, v_y)$ 与宽高 $(v_w, v_h)$ 支持自由拖拽与 8 向拉伸。

### 6.2 裁剪区光栅化管线 (Rasterization Clipping Pipeline)
当外部视口启用时，绘制流程通过状态栈执行隔离：
```text
painter.save();
painter.setClipRect(m_viewportRect); // 施加凸多边形/轴对齐矩形裁剪
// ---------------------------------------------
// 此区域内所有 QPainter 调用均被硬件/光栅器裁剪
// 矩阵变换 M
// 绘制多边形 / 曲线 / 位图
// 绘制图元包围盒与手柄
// ---------------------------------------------
painter.restore(); // 弹出裁剪区域，恢复全局上下文
```

### 6.3 经典多边形裁剪算法背后的数学
在底层，Qt 或图形管线采用 **Sutherland-Hodgman 算法**对任意几何多边形关于视口 4 条裁剪边进行逐边求交裁剪：
对视口的每条有向边界线 $L$，将平面划分为“内侧半平面”与“外侧半平面”。对于多边形顶点序列 $V_i \to V_{i+1}$：
- 若两点均在内侧：输出 $V_{i+1}$；
- 若起始点在内侧、终止点在外侧：计算边与边界线 $L$ 的交点 $I$，输出 $I$；
- 若两点均在外侧：不输出任何点；
- 若起始点在外侧、终止点在内侧：输出交点 $I$ 与终止点 $V_{i+1}$。

通过四条边界裁剪后，输出受限在外部视口内部的有界凸/凹多边形，确保渲染绝对不越界。

### 6.4 交互事件的视口包容测试 (Viewport Hit Containment)
当外部视口存在时，鼠标滚轮、点击、拖拽操作优先进行视口几何包含测试：
$$\text{Inside}(\mathbf{P}_{mouse}, \mathcal{V}) \iff (x \ge v_x) \land (x \le v_x + v_w) \land (y \ge v_y) \land (y \le v_y + v_h)$$
仅当满足上述条件时，内部图元的变换逻辑才被触发响应，从而在几何层面与交互行为层面达成彻底的视口边界沙箱隔离。
