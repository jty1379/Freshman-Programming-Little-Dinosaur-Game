/**
 * @file OptimizedMain.cpp
 * @brief Chrome离线小恐龙跑酷游戏主程序
 * @details 程序入口，创建游戏实例，控制游戏主循环
 * 
 * 主循环流程：
 * 1. 创建DinoGame对象
 * 2. 调用initialize初始化游戏窗口和资源
 * 3. while循环每帧执行：handleInput -> update -> render
 * 4. 退出循环后调用cleanup清理资源
 * 5. 输出最终分数
 */

#include "OptimizedDinoGame.h"
#include <iostream>
#include <windows.h>

// 隐藏控制台窗口，只显示游戏图形窗口
#pragma comment(linker, "/subsystem:windows /entry:mainCRTStartup")

/**
 * @brief 程序主入口
 * @return 程序退出状态码
 * @details 创建游戏实例，运行游戏主循环，清理资源并输出最终分数
 */
int main() {
    DinoGame game;  // 创建游戏对象
    
    game.initialize();  // 初始化游戏窗口和资源
    
    // 游戏主循环：持续运行直到用户按ESC退出
    while (game.isGameRunning()) {
        game.handleInput();  // 处理键盘输入
        game.update();       // 更新游戏逻辑
        game.render();       // 渲染游戏画面
    }
    
    game.cleanup();  // 清理资源，关闭窗口
    
    // 输出最终分数到控制台
    std::cout << "Game Over! Final Score: " << game.getCurrentScore() << std::endl;
    
    return 0;  // 程序正常结杞
}