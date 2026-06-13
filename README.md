# codestothings

#### 介绍
这是我的《从代码到实物》课程作业仓库，用来记录本学期从 Git/Gitee 基础操作、网页搭建、三维建模、3D 打印、嵌入式硬件、智能装置到 PCB 设计的学习过程。

本仓库会按照课程进度逐步更新。每完成一个阶段，我会先整理文字说明，再补充自己的截图、模型、代码、实物照片和演示材料，并通过 Git 提交到 Gitee 远程仓库。

#### 软件架构
这是一个 html 的展示网站，包含课程作业记录和项目开发过程。

#### 安装教程
1. git clone https://gitee.com/palomadove/codestothings.git

#### 使用说明
1. 浏览器打开 HTML/index.html 即可查看网站

---

## 一、课程过程与 DIY 成果

### 1. 3D 打印
- **学习目标**：学习 3D 建模与打印技术，掌握从建模软件操作到打印机操作的完整流程
- **已完成内容**：
  - 交大书签的设计与打印
  - 中国象棋的设计与打印
- **设计思路**：通过 Fusion 360 等建模软件设计可打印模型，导出 STL 格式，使用切片软件优化打印参数
- **技术要点**：3D 建模软件操作、STL 文件处理、切片软件、打印参数优化
- **成果展示**：
  - 交大书签设计与打印完成
  - 中国象棋设计与打印完成
- **相关文件**：
  - [交大书签 STL 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/3D打印作业图纸/3D打印作业交大书签图纸.stl)
  - [中国象棋 STL 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/3D打印作业图纸/3D打印课堂作业中国象棋图纸.stl)
  - [交大书签实物照片](https://gitee.com/palomadove/codestothings/blob/master/照片/3D打印作业图/3D打印作业交大书签实物图.jpg)

### 2. 嵌入式
- **学习目标**：学习 Arduino 嵌入式开发，掌握基本电路搭建和编程基础
- **已完成内容**：完成嵌入式作业项目，熟悉传感器数据采集和控制逻辑的开发流程
- **技术要点**：Arduino 编程、传感器应用、电路搭建、串口调试
- **成果展示**：
  - 完成嵌入式作业代码与硬件连接
  - 功能演示视频
- **相关文件**：
  - [嵌入式作业代码](https://gitee.com/palomadove/codestothings/blob/master/嵌入式代码/嵌入式作业代码.ino)
  - [嵌入式作业实物图](https://gitee.com/palomadove/codestothings/blob/master/照片/嵌入式作业图/嵌入式作业实物图.png)
  - [嵌入式作业演示视频](https://gitee.com/palomadove/codestothings/blob/master/视频/嵌入式作业演示.mp4)

### 3. PCB
- **学习目标**：学习 PCB 设计与制作，掌握电路原理图设计和 PCB 布局布线
- **已完成内容**：完成两个 PCB 作业项目
- **技术要点**：电路原理图设计、PCB 布局布线、PCB 设计软件
- **成果展示**：
  - PCB 第一次作业完成
  - PCB 第二次作业完成
- **相关文件**：
  - [PCB 第一次作业图纸](https://gitee.com/palomadove/codestothings/blob/master/图纸/PCB作业图纸/PCB作业1PCB板图纸.png)
  - [PCB 第二次作业图纸](https://gitee.com/palomadove/codestothings/blob/master/图纸/PCB作业图纸/PCB作业2PCB板图纸.png)
  - [PCB 第二次作业原理图](https://gitee.com/palomadove/codestothings/blob/master/图纸/PCB作业图纸/PCB作业2电路原理图.png)
  - [PCB 第一次实物图](https://gitee.com/palomadove/codestothings/blob/master/照片/PCB作业图/PCB作业1实物图.png)
  - [PCB 第二次实物图](https://gitee.com/palomadove/codestothings/blob/master/照片/PCB作业图/PCB作业2实物图.png)

### 4. 激光切割
- **学习目标**：学习激光切割技术，掌握从设计到切割的完整流程
- **已完成内容**：完成激光切割盒子的设计与制作
- **技术要点**：激光切割设计软件、DXF/SVG 文件处理、激光切割机操作
- **成果展示**：激光切割盒子完成
- **相关文件**：
  - [底板 DXF](https://gitee.com/palomadove/codestothings/blob/master/图纸/激光切割作业图纸/底板.dxf)
  - [长侧板 DXF](https://gitee.com/palomadove/codestothings/blob/master/图纸/激光切割作业图纸/长侧板1.dxf)
  - [短侧板 DXF](https://gitee.com/palomadove/codestothings/blob/master/图纸/激光切割作业图纸/短侧板1.dxf)
  - [激光切割实物图1](https://gitee.com/palomadove/codestothings/blob/master/照片/激光切割作业图/激光切割作业实物图1.jpg)
  - [激光切割实物图2](https://gitee.com/palomadove/codestothings/blob/master/照片/激光切割作业图/激光切割作业实物图2.jpg)

---

## 二、中期项目：WiFi 控制的智能小车

### 1. 项目主题
本项目基于 Arduino 平台和 WiFi 模块，开发了一款可通过手机远程控制的智能小车。

### 2. 项目功能
- 前进、后退、左右转向
- 加速减速功能
- WiFi 无线控制
- 3D 打印底盘和连接器

### 3. 技术架构
- **主控**：Arduino + WiFi 模块
- **驱动**：TT 电机 + 电机驱动模块
- **通信**：WiFi 无线控制
- **结构**：3D 打印底盘和连接器

### 4. 团队分工（个人项目）
- 吕豪：嵌入式程序开发、WiFi 通信、传感器集成、整体功能测试

### 5. 成果文件
- [小车控制代码](https://gitee.com/palomadove/codestothings/blob/master/嵌入式代码/中期代码.ino)
- [底盘 STL 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/中期项目图纸/中期底盘3D打印图纸.stl)
- [TT 电机连接器 3MF 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/中期项目图纸/TT_Motor连接器3D打印图纸.3mf)
- [底盘 3MF 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/中期项目图纸/中期底盘3D打印图纸.3mf)
- [中期项目实物图1](https://gitee.com/palomadove/codestothings/blob/master/照片/中期项目图/中期项目实物图1%20.jpg)
- [中期项目实物图2](https://gitee.com/palomadove/codestothings/blob/master/照片/中期项目图/中期项目实物图2%20.jpg)
- [WiFi 控制演示视频](https://gitee.com/palomadove/codestothings/blob/master/视频/中期演示.mp4)

---

## 三、期末项目：宿舍熄灯后下床夜间感应 LED 灯

### 1. 项目主题
本项目利用光线传感器和 FSR 薄膜压力传感器，设计了一款智能夜间感应 LED 灯。系统通过光线传感器判断环境亮度，在熄灯后自动进入待机状态；当检测到有人下床踩踏 FSR 传感器时，自动点亮 LED 灯，方便夜间行走，避免打扰室友休息。

### 2. 项目功能
- 光线传感器自动检测环境亮度
- FSR 薄膜压力传感器检测踩踏
- 智能 LED 灯控制
- 3D 打印外壳设计

### 3. 团队信息
**设计思维 周五EM 第四组**

- **吕豪**：项目协调 / 集成（成员分工、监督以及最后的成果汇总）
- **张文豪**：硬件与电路（完成传感器、电路焊接和连接）
- **冯国豪**：程序与嵌入式（撰写相关代码，完成功能的实现）
- **陈家兴**：结构与制造（材料选型、3D 打印、部件组装、安装结构、灯带布线）
- **于博涵**：展示与测试（汇报成果的应用场景，成果的意义，成果展示，原理解释）

### 4. 开发时间线
1. **6月12日 15:00 - 项目启动与方案设计**
   - 参与 Hackathon 活动
   - 确定项目主题和技术方案
   - 确定使用 Arduino 作为主控平台
   - 选择光线传感器和 FSR 压力传感器
   - 设计 3D 打印外壳方案

2. **6月12日 16:30 - 硬件组装与电路搭建**
   - 连接 Arduino 主板
   - 搭建光线传感器电路
   - 连接 FSR 薄膜压力传感器
   - 焊接 LED 灯模块

3. **6月12日 18:00 - 软件开发与调试**
   - 实现光线传感器数据读取
   - 开发 FSR 压力检测逻辑
   - 编写 LED 灯控制算法
   - 测试程序功能

4. **6月13日 09:00 - 3D 打印与结构优化**
   - 打印盒子主体结构
   - 打印盖子部件
   - 组装所有零部件
   - 优化传感器布局

5. **6月13日 14:00 - 最终测试与优化**
   - 测试光线感应灵敏度
   - 调整 FSR 压力阈值
   - 优化 LED 亮度控制
   - 录制功能演示视频

### 5. 问题与解决
- **FSR 传感器灵敏度不足**：调整代码中的检测阈值，增加传感器与脚底接触面积
- **光线传感器阈值设定困难**：添加自动校准功能，根据环境自动调整阈值
- **LED 亮度控制不稳定**：更换 LED 灯的库文件，支持更稳定的 RGB 颜色控制
- **3D 打印结构装配问题**：重新测量并调整 3D 打印模型参数，优化装配工艺

### 6. 经验总结
通过这次项目开发，我们学到了很多宝贵的经验：
- 硬件开发中遇到问题是常态，关键是要有耐心和系统的调试方法
- 传感器的选型和参数设置需要根据实际使用场景反复测试
- 3D 打印前要仔细检查模型尺寸，预留装配余量
- 模块化设计有助于快速定位和解决问题
- 及时记录开发过程和遇到的问题非常重要

### 7. 成果文件
- [开发日志网页](https://gitee.com/palomadove/codestothings/blob/master/HTML/final-log.html)
- [期末项目代码](https://gitee.com/palomadove/codestothings/blob/master/嵌入式代码/期末代码.ino)
- [盒子 STL 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/期末项目图纸/期末盒子3D打印图纸.stl)
- [盖子 STL 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/期末项目图纸/期末盖子3D打印图纸.stl)
- [梯子 STL 模型](https://gitee.com/palomadove/codestothings/blob/master/图纸/期末项目图纸/期末梯子3D打印图纸.stl)
- [期末项目实物图](https://gitee.com/palomadove/codestothings/blob/master/照片/期末项目图/期末项目实物图.jpg)
- [开发过程图](https://gitee.com/palomadove/codestothings/blob/master/照片/期末项目完成过程图.jpg)
- [失败图](https://gitee.com/palomadove/codestothings/blob/master/照片/期末项目完成过程图失败图.jpg)
- [功能演示视频](https://gitee.com/palomadove/codestothings/blob/master/视频/期末项目演示.mp4)

---

## 四、课程总结与成长感悟

通过系统性的创客课程学习，我不仅掌握了从基础电子到复杂物联网系统的完整开发流程，更重要的是培养了解决实际问题的能力。从最初的"点灯"实验到最终的智能小车和夜间感应 LED 灯项目，每一次调试都让我对软硬件协同工作有了更深的理解。

### 核心收获
- Arduino/STM32 嵌入式开发
- 多传感器数据融合处理
- WiFi/MQTT 物联网通信
- 电路设计与 PCB 制作
- 3D 建模与结构设计
- Git 版本控制协作

### 未来规划
计划深入学习 RTOS 实时操作系统、边缘计算技术，并探索 AI 与硬件的深度融合应用，为智能物联网领域贡献更多创新解决方案。

---

## 仓库目录规划
```
codestothings/
├── README.md                 # 课程作业总说明
├── HTML/                     # 网页相关源文件
│   ├── index.html           # 课程展示主页
│   └── final-log.html       # 期末项目开发日志
├── 照片/                     # 作业截图、实物照片、过程图片
│   ├── 3D打印作业图/
│   ├── 嵌入式作业图/
│   ├── PCB作业图/
│   ├── 激光切割作业图/
│   ├── 中期项目图/
│   └── 期末项目图/
├── 视频/                     # 项目演示视频
├── 图纸/                     # 3D模型、PCB、激光切割文件
│   ├── 3D打印作业图纸/
│   ├── PCB作业图纸/
│   ├── 激光切割作业图纸/
│   ├── 中期项目图纸/
│   └── 期末项目图纸/
├── 嵌入式代码/               # Arduino/嵌入式程序代码
└── .gitignore               # Git 忽略文件配置
```

---

#### 参与贡献
1. Fork 本仓库
2. 新建 Feat_xxx 分支
3. 提交代码
4. 新建 Pull Request

---

© 2026 吕豪 · 《从代码到实物》课程作品集
