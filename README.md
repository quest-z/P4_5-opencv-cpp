# P4.5: OpenCV Color Detection — C++ Rewrite with CMake

将 P4 (Python) 的视觉检测管线完整迁移到 C++，基于 CMake 构建，在 WSL2 Ubuntu 环境下编译运行。验证跨平台开发流程。

## 技术栈

C++17 · OpenCV 4.x · CMake · WSL2 / Ubuntu 2.04

## 项目结构

```
P4_5-opencv-cpp/
├── CMakeLists.txt          # CMake 构建配置
├── main.cpp                # 主程序：读取 → HSV阈值 → 轮廓 → 面积过滤 → 输出
├── test.jpg                # 测试输入
├── mask.jpg                # 二值掩码输出
└── result.jpg              # 检测结果输出
```

## 构建与运行

```bash
# 1. 进入项目目录
cd P4_5-opencv-cpp

# 2. CMake 生成构建文件 + 编译
cmake -B build && cmake --build build

# 3. 运行
./build/vision
```

环境要求：
- WSL2 + Ubuntu 22.04（或原生 Linux）
- OpenCV 4.x 开发库：`sudo apt install libopencv-dev`
- CMake >= 3.10：`sudo apt install cmake`

## 核心逻辑（与 P4 Python 对照）

| 步骤 | Python (P4) | C++ (P4.5) | 备注 |
|------|-------------|------------|------|
| 读取图像 | `cv2.imread()` | `cv::imread()` | API 一致 |
| 色彩空间转换 | `cv2.cvtColor(..., COLOR_BGR2HSV)` | `cv::cvtColor(..., cv::COLOR_BGR2HSV)` | OpenCV 恒为 BGR |
| HSV 阈值分割 | `cv2.inRange(hsv, lower, upper)` | `cv::inRange(hsv, lower, upper, mask)` | 参数直接沿用 P4 校准值 |
| 轮廓检测 | `cv2.findContours(...)` | `cv::findContours(...)` | C++ 类型显式声明 |
| 最大轮廓筛选 | `max(contours, key=cv2.contourArea)` | 手写循环比较 | Python 语法糖 → C++ 显式控制 |
| 面积过滤 | `if area > 500:` | `if (maxArea > 500)` | 分层防御第二层，原封不动迁移 |
| 绘制结果 | `cv2.rectangle(...)` | `cv::rectangle(...)` | BGR 顺序一致 |
| 结果保存 | `cv2.imwrite(...)` | `cv::imwrite(...)` | WSL2 写文件，回 Windows 查看 |

## 调参记录

- **HSV 阈值**：沿用 P4 校准值 `H: [86, 106], S: [92, 255], V: [40, 255]`
- **S 下限收紧至 92**：第一版 mask 中淡蓝色背景大面积通过阈值，最大轮廓被背景抢走；提高饱和度下限后墙面被排除,框选恢复正常。
- **面积过滤**：维持 `> 500`，隔离 mask 残余噪点，不影响目标检测。

## 工程化收获

1. **CMake 是现代 C++ 的标配**：从手动 `g++` 到 `cmake -B build`，项目结构更清晰，可复现性更强。
2. **类型显式化是 C++ 的代价与优势**：`std::vector<std::vector<cv::Point>>` 写起来繁琐，但编译期就能捕获类型错误。
3. **跨平台验证**：同一套 CMake 配置在 WSL2 下编译通过，为后续树莓派（ARM Linux）部署打下基础。
4. **阈值法具有场景依赖性**：调试步骤固定为「看数据 → 提假设 → 修改 → 验证」，与语言无关。

## 下一步

- [ ] P5: PyTorch 模型导出 ONNX + INT8 量化
- [ ] P6: 树莓派 5 端侧部署 + Benchmark（Latency / FPS / 内存）

---

*本项目为厦门大学自动化专业本科生的自主项目链的一部分，完整项目索引见 [github.com/quest-z](https://github.com/quest-z)。*
