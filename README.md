# 小恐龙游戏

这是一个基于EGE图形库实现的Chrome离线小恐龙游戏。

## 功能特性

- 经典的小恐龙跳跃玩法
- 支持下蹲功能，可以躲避低飞的鸟
- 随机生成不同类型的障碍物（仙人掌和飞鸟）
- 分数计算和难度递增
- 昼夜模式切换

## 操作说明

- **空格键/W键**：跳跃
- **S键/下箭头键**：下蹲（再次按下恢复正常状态）
- **ESC键**：退出游戏

## 编译和运行

### 环境要求

- Windows操作系统
- EGE图形库
- 支持C++17的编译器（如MinGW-w64）

### 编译步骤

1. 确保已安装EGE图形库并正确配置
2. 使用以下命令直接编译：
   ```
   g++ -Wall -Wextra -std=c++17 -I"E:/CLion 2025.2.2/bin/mingw/include" src/OptimizedMain.cpp src/OptimizedDinoGame.cpp -o dino_game.exe -L"E:/CLion 2025.2.2/bin/mingw/lib" -lgraphics -lgdi32 -luser32 -lkernel32 -lgdiplus
   ```
3. 运行生成的可执行文件：
   ```
   ./dino_game.exe
   ```

## 代码结构

- `src/OptimizedMain.cpp` - 程序入口
- `src/OptimizedDinoGame.cpp` - 包含所有游戏类的实现
- `src/OptimizedDinoGame.h` - 包含所有游戏类的声明

## 优化内容

1. **文件合并**：将原来的8个文件合并为3个文件，提高了代码组织性
2. **性能优化**：优化了游戏速度调整机制，使游戏体验更加平衡
3. **代码精简**：删除了不必要的注释，使代码更加简洁易读
4. **下蹲逻辑修复**：确保下蹲时恐龙正确地向下收缩，保持底部贴地
5. **碰撞检测优化**：改进了低飞鸟和高飞鸟的碰撞检测逻辑