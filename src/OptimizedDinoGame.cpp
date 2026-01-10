/**
 * @file OptimizedDinoGame.cpp
 * @brief Chrome离线小恐龙跑酷游戏实现文件
 * @details 包含所有游戏类的实现：恐龙、障碍物、背景、分数管理器和游戏控制器
 */

#include "OptimizedDinoGame.h"
#include <conio.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>

// ==================== Dinosaur类实现 ====================

/**
 * @brief Dinosaur类构造函数
 * @details 初始化恐龙位置、速度和状态标志
 */
Dinosaur::Dinosaur() : x(50), y(340 - 60), velocityY(0), isJumping(false), isDucking(false), groundLevel(340) {}

Dinosaur::~Dinosaur() {}

/**
 * @brief 执行跳跃动作
 * @details 只有在非跳跃且非下蹲状态下才能跳跃。设置isJumping=true并赋予初始向上速度-15
 */
void Dinosaur::jump() {
    if (!isJumping && !isDucking) {
        isJumping = true;
        velocityY = -15;  // 负数表示向上，初始跳跃速度
    }
}

/**
 * @brief 执行下蹲动作
 * @details 只有在非跳跃状态下才能下蹲。设置isDucking=true并调整y坐标使恐龙高度变为30
 */
void Dinosaur::duck() {
    if (!isJumping) {
        isDucking = true;
        y = groundLevel - DINO_HEIGHT_DUCK;  // 下蹲后高度变为30像素
    }
}

/**
 * @brief 恢复站立状态
 * @details 取消下蹲标志并恢复正常高度60
 */
void Dinosaur::stand() {
    isDucking = false;
    y = groundLevel - DINO_HEIGHT;  // 恢复正常高度60像素
}

/**
 * @brief 更新恐龙状态（每帧调用）
 * @details 实现跳跃物理模拟
 * 
 * 物理模型：
 *   - 跳跃时velocityY初始为-15（向上）
 *   - 每帧velocityY增加1（模拟重力加速度）
 *   - y坐标每帧增加velocityY（负值向上移动）
 * 
 * 落地检测：
 *   - 当y坐标≥地面位置时，恐龙着陆
 *   - 重置isJumping标志和velocityY
 */
void Dinosaur::update() {
    if (isJumping) {
        y += velocityY;          // 根据垂直速度更新位置
        velocityY += 1;          // 每帧速度增加1，模拟重力加速度
        
        // 落地检测：当y坐标超过或等于地面位置时
        if (y >= groundLevel - DINO_HEIGHT) {
            y = groundLevel - DINO_HEIGHT;  // 修正到准确的地面位置
            isJumping = false;               // 结束跳跃状态
            velocityY = 0;                   // 重置速度
        }
    }
}

/**
 * @brief 渲染恐龙到屏幕
 * @details 根据当前状态（站立/下蹲/跳跃）绘制不同形态的恐龙图形，包括身体、眼睛和腿部装饰
 */
void Dinosaur::render() {
    setfillcolor(BLACK);
    solidrect(x, y, x + getWidth(), y + getHeight());
    
    if (isDucking) {
        solidrect(x + getWidth() - 8, y + 5, x + getWidth() - 5, y + 8);
        solidrect(x, y + 10, x + 5, y + 15);
        solidrect(x + getWidth() - 5, y + 10, x + getWidth(), y + 15);
    } else if (!isJumping) {
        solidrect(x + getWidth() - 10, y + 8, x + getWidth() - 5, y + 13);
        solidrect(x + 5, y + getHeight(), x + 10, y + getHeight() + 5);
        solidrect(x + getWidth() - 10, y + getHeight(), x + getWidth() - 5, y + getHeight() + 5);
    } else {
        solidrect(x + getWidth() - 8, y + 10, x + getWidth() - 6, y + 12);
    }
}

void Dinosaur::setPosition(float x, float y) {
    this->x = x;
    this->y = y;
}

// ==================== Obstacle类实现 ====================

/**
 * @brief Obstacle类构造函数
 * @details 初始化障碍物位置、尺寸和基础速度
 */
Obstacle::Obstacle(float x, float y, float width, float height) 
    : x(x), y(y), width(width), height(height), speed(5) {}  // 基础速度5像素/帧

Obstacle::~Obstacle() {}

/**
 * @brief 更新障碍物位置（每帧调用）
 * @param gameSpeed 游戏速度等级（5-12）
 * @details 实际速度 = speed(5) + gameSpeed * 0.15，向左移动（X坐标减少）
 */
void Obstacle::update(float gameSpeed) {
    x -= (speed + gameSpeed * 0.15f);  // 综合基础速度和游戏速度加成
}

/**
 * @brief 渲染障碍物到屏幕
 * @details 绘制基础矩形，子类可重写以实现特殊样式
 */
void Obstacle::render() {
    setfillcolor(BLACK);
    solidrect(x, y, x + width, y + height);
}

/**
 * @brief 检测与恐龙的碰撞
 * @param dino 恐龙对象引用
 * @return 是否发生碰撞
 * @details 使用AABB（轴对齐包围盒）矩形碰撞检测算法，判断两个矩形是否相交
 */
bool Obstacle::checkCollision(const Dinosaur& dino) {
    return (dino.getX() + dino.getWidth() > x &&   // 恐龙右侧 > 障碍物左侧
            dino.getX() < x + width &&              // 恐龙左侧 < 障碍物右侧
            dino.getY() + dino.getHeight() > y &&   // 恐龙底部 > 障碍物顶部
            dino.getY() < y + height);              // 恐龙顶部 < 障碍物底部
}

// ==================== Cactus类实现 ====================

/**
 * @brief Cactus类构造函数（默认高度）
 * @details 创建宽度20、高度40的仙人掌
 */
Cactus::Cactus(float x, float y) : Obstacle(x, y, 20, 40) {}

/**
 * @brief Cactus类构造函数（自定义高度）
 * @param height 仙人掌高度（20-80像素）
 * @details 创建宽度20、高度可变的仙人掌，y坐标自动调整
 */
Cactus::Cactus(float x, float y, float height) : Obstacle(x, y - height, 20, height) {}

Cactus::~Cactus() {}

/**
 * @brief 渲染仙人掌到屏幕
 * @details 绘制主体和分支装饰，形成仙人掌形状
 */
void Cactus::render() {
    setfillcolor(BLACK);
    solidrect(x, y, x + width, y + height);
    
    setfillcolor(BLACK);
    solidrect(x - 5, y + 10, x, y + 15);
    solidrect(x + width, y + 10, x + width + 5, y + 15);
    solidrect(x + 5, y - 5, x + 10, y);
    solidrect(x + 10, y - 10, x + 15, y - 5);
}

bool Cactus::checkCollision(const Dinosaur& dino) {
    return Obstacle::checkCollision(dino);  // 使用基类的标准AABB碰撞检测
}

// ==================== Bird类实现 ====================

/**
 * @brief Bird类构造函数
 * @details 创建宽度30、高度20的飞鸟，初始化翅膀动画参数
 */
Bird::Bird(float x, float y) : Obstacle(x, y, 30, 20), wingPosition(0), animationCounter(0) {}

Bird::~Bird() {}

/**
 * @brief 更新飞鸟状态（每帧调用）
 * @param gameSpeed 游戏速度等级
 * @details 调用基类更新位置，同时更新翅膀扇动动画（每5帧切换一次）
 */
void Bird::update(float gameSpeed) {
    Obstacle::update(gameSpeed);  // 调用基类的位置更新
    
    animationCounter++;
    if (animationCounter % 5 == 0) {  // 每5帧切换一次翅膀状态
        wingPosition = (wingPosition + 1) % 2;  // 0和1之间切换
    }
}

/**
 * @brief 渲染飞鸟到屏幕
 * @details 绘制身体、翅膀（根据wingPosition切换位置）和眼睛
 */
void Bird::render() {
    setfillcolor(BLACK);
    solidrect(x, y, x + width, y + height);
    
    setfillcolor(WHITE);
    if (wingPosition == 0) {
        solidrect(x + 5, y + 5, x + 15, y + 10);
    } else {
        solidrect(x + 15, y + 5, x + 25, y + 10);
    }
    
    setfillcolor(WHITE);
    solidrect(x + 20, y + 5, x + 22, y + 7);
}

/**
 * @brief 飞鸟与恐龙的碰撞检测（特殊逻辑）
 * @param dino 恐龙对象引用
 * @return 是否发生碰撞
 * @details 区分低飞鸟和高飞鸟，实现不同的躲避机制
 * 
 * 低飞鸟判定（Y≥3310）：
 *   - 恐龙跳跃时，如果跃过飞鸟底部则不碰撞
 * 
 * 高飞鸟判定（Y<310）：
 *   - 恐龙下蹲时，如果低于飞鸟下沿则不碰撞
 * 
 * 碰撞算法：
 *   - 使用AABB矩形碰撞检测（轴对齐包围盒）
 *   - 检测恐龙和飞鸟矩形是否相交
 */
bool Bird::checkCollision(const Dinosaur& dino) {
    bool isLowFlying = (y >= 310);  // 低飞鸟判定阈值
    
    if (isLowFlying) {
        // 低飞鸟：恐龙跳跃可躲避
        if (dino.getIsJumping()) {
            if (dino.getY() + dino.getHeight() <= y) {  // 恐龙底部高于飞鸟顶部
                return false;  // 成功躲避
            }
        }
    } else {
        // 高飞鸟：恐龙下蹲可躲避
        if (dino.getIsDucking()) {
            if (dino.getY() + dino.getHeight() <= y + 20) {  // 恐龙高度低于飞鸟下方
                return false;  // 成功躲避
            }
        }
    }
    
    // 标准AABB矩形碰撞检测
    return (dino.getX() + dino.getWidth() > x &&
            dino.getX() < x + width &&
            dino.getY() + dino.getHeight() > y &&
            dino.getY() < y + height);
}

// ==================== Background类实现 ====================

/**
 * @brief Background类构造函数
 * @details 初始化滚动速度、偏移量和昼夜模式
 */
Background::Background() : scrollSpeed(2), groundOffset(0), isNightMode(false) {}

Background::~Background() {}

/**
 * @brief 更新背景状态（每帧调用）
 * @details 更新地面滚动偏移量，实现地面滚动动画效果，每20像素循环一次
 */
void Background::update() {
    groundOffset += scrollSpeed;  // 每帧增加2像素
    if (groundOffset >= 20) {
        groundOffset = 0;  // 循环重置，实现无限滚动
    }
}

/**
 * @brief 渲染背景到屏幕
 * @details 绘制背景色、地面、滚动纹理和云朵装饰，根据昼夜模式调整颜色
 */
void Background::render() {
    if (isNightMode) {
        setbkcolor(RGB(50, 50, 50));  // 夜间模式：深灰色背景
    } else {
        setbkcolor(WHITE);  // 白天模式：白色背景
    }
    
    // 绘制地面区域（Y=340-400）
    setfillcolor(RGB(100, 100, 100));
    solidrect(0, 340, 800, 400);
    
    // 绘制地面滚动纹理
    setfillcolor(RGB(80, 80, 80));
    for (int i = 0; i < 800; i += 20) {
        int offset = (int)groundOffset % 20;
        solidrect(i - offset, 340, i - offset + 10, 350);  // 滚动的地面装饰块
    }
    
    // 绘制静态云朵装饰（多个位置）
    setfillcolor(RGB(200, 200, 200));
    solidellipse(100, 80, 140, 100);
    solidellipse(120, 70, 160, 90);
    solidellipse(140, 80, 180, 100);
    
    solidellipse(300, 60, 340, 80);
    solidellipse(320, 50, 360, 70);
    solidellipse(340, 60, 380, 80);
    
    solidellipse(500, 70, 540, 90);
    solidellipse(520, 60, 560, 80);
    solidellipse(540, 70, 580, 90);
    
    solidellipse(700, 80, 740, 100);
    solidellipse(720, 70, 760, 90);
    solidellipse(740, 80, 780, 100);
}

void Background::toggleNightMode(bool night) {
    isNightMode = night;
}

// ==================== ScoreManager类实现 ====================

/**
 * @brief ScoreManager类构造函数
 * @details 初始化当前分数和最高分为0
 */
ScoreManager::ScoreManager() : currentScore(0), highScore(0) {}

ScoreManager::~ScoreManager() {}

/**
 * @brief 更新分数（每帧调用）
 * @details 当前分数每帧+1，并更新最高分记录
 */
void ScoreManager::update() {
    currentScore++;  // 每帧增加1分
    
    if (currentScore > highScore) {
        highScore = currentScore;  // 更新最高分
    }
}

/**
 * @brief 渲染分数到屏幕
 * @details 在屏幕右上角显示当前分数和最高分，根据昼夜模式调整文字颜色
 */
void ScoreManager::render() {
    setfont(20, 0, "Arial");
    
    if (isNightMode) {
        setcolor(WHITE);  // 夜间模式白色文字
    } else {
        setcolor(BLACK);  // 白天模式黑色文字
    }
    
    std::string scoreText = "Score: " + std::to_string(currentScore);
    outtextxy(650, 20, scoreText.c_str());
    
    std::string highScoreText = "Best: " + std::to_string(highScore);
    outtextxy(650, 50, highScoreText.c_str());
}

void ScoreManager::reset() {
    currentScore = 0;  // 重置分数为0，最高分保留
}

void ScoreManager::incrementScore(int points) {
    currentScore += points;
}

// ==================== DinoGame类实现 ====================

/**
 * @brief DinoGame类构造函数
 * @details 初始化游戏状态、速度和计数器
 */
DinoGame::DinoGame() : isRunning(false), isGameOver(false), gameSpeed(5), frameCount(0), gameOverDelay(0) {}

DinoGame::~DinoGame() {
    cleanup();
}

/**
 * @brief 初始化游戏
 * @details 创建图形窗口，设置标题，初始化随机数种子，重置游戏状态
 */
void DinoGame::initialize() {
    if (isRunning) {
        // 如果已运行，只重置游戏状态
        obstacles.clear();
        score.reset();
    } else {
        // 首次运行，创建窗口
        initgraph(800, 400);  // 创建800x400的窗口
        setcaption("Scu Dino Game");  // 设置窗口标题
        setbkcolor(WHITE);
        cleardevice();
        ege::setrendermode(RENDER_MANUAL);  // 手动渲染模式
        
        srand((unsigned int)time(nullptr));  // 初始化随机数生成器
    }
    
    // 重置恐龙位置和游戏状态
    player.setPosition(50, 340 - 60);
    isRunning = true;
    isGameOver = false;
    frameCount = 0;
    gameOverDelay = 0;
    gameSpeed = 5;  // 重置为初始速度
}

/**
 * @brief 更新游戏逻辑（每帧调用）
 * @details 更新所有游戏对象、生成障碍物、检测碰撞、调整难度
 */
void DinoGame::update() {
    if (!isRunning) return;  // 游戏未运行时直接返回
    
    if (isGameOver) {
        // 游戏结束状态，只增加延迟计数
        gameOverDelay++;
        if (gameOverDelay > 90) {  // 90帧（约3秒）后允许重启
            gameOverDelay = 91;
        }
        return;
    }
    
    // 更新所有游戏对象
    player.update();          // 更新恐龙状态（跳跃物理）
    background.update();      // 更新背景（地面滚动）
    score.update();           // 更新分数
    generateObstacle();       // 生成障碍物
    
    // 更新所有障碍物位置
    for (auto& obstacle : obstacles) {
        obstacle->update(gameSpeed);  // 传入游戏速度等级
    }
    
    checkCollisions();    // 检测碰撞
    updateGameSpeed();    // 调整游戏速度和昼夜模式
    
    frameCount++;  // 帧计数器递增
}

/**
 * @brief 渲染游戏画面（每帧调用）
 * @details 清空画布，依次渲染背景、恐龙、障碍物、分数和游戏结束界面
 */
void DinoGame::render() {
    if (!isRunning) return;  // 游戏未运行时直接返回
    
    cleardevice();        // 清空画布
    background.render();  // 渲染背景
    player.render();      // 渲染恐龙
    
    // 渲染所有障碍物
    for (const auto& obstacle : obstacles) {
        obstacle->render();
    }
    
    score.render();  // 渲染分数
    
    if (isGameOver) {
        showGameOverScreen();  // 显示游戏结束界面
    }
    
    ege::delay_ms(30);  // 延迟30毫秒，控制帧率约33FPS
}

/**
 * @brief 处理键盘输入（每帧调用）
 * @details 检测键盘输入，处理跳跃、下蹲、重启和退出操作
 */
void DinoGame::handleInput() {
    if (kbhit()) {  // 检测是否有键盘输入
        char key = getch();  // 获取按键
        
        if (isGameOver) {
            // 游戏结束状态，按任意键重启（延迟后）
            if (gameOverDelay >= 91) {
                initialize();  // 重新初始化游戏
                isGameOver = false;
            }
            return;
        }
        
        // 游戏运行中的按键处理
        switch (key) {
        case ' ':   // 空格键
        case 'w':   // W键
        case 'W':
            if (!player.getIsJumping() && !player.getIsDucking()) {
                player.jump();  // 跳跃
            } else if (player.getIsDucking()) {
                player.stand();  // 下蹲中按空格恢复站立
            }
            break;
        case 's':   // S键
        case 'S':
        case 80:    // 下箭头键
            if (!player.getIsJumping()) {
                if (player.getIsDucking()) {
                    player.stand();  // 再次按S恢复站立
                } else {
                    player.duck();   // 下蹲
                }
            }
            break;
        case 27:    // ESC键
            isRunning = false;  // 退出游戏
            break;
        }
    }
}

/**
 * @brief 显示游戏结束界面
 * @details 显示Game Over文字、分数和重启倒计时提示
 */
void DinoGame::showGameOverScreen() {
    setfont(30, 0, "Arial Bold");
    setcolor(RED);
    
    setbkmode(TRANSPARENT);  // 透明背景
    
    // 显示Game Over文字（居中）
    const char* gameOverText = "Game Over";
    int gameOverTextWidth = textwidth(gameOverText);
    int gameOverXPos = (800 - gameOverTextWidth) / 2;
    outtextxy(gameOverXPos, 150, gameOverText);
    
    // 显示分数（居中）
    std::string scoreText = "Score: " + std::to_string(score.getCurrentScore());
    int scoreTextWidth = textwidth(scoreText.c_str());
    int scoreXPos = (800 - scoreTextWidth) / 2;
    outtextxy(scoreXPos, 200, scoreText.c_str());
    
    setfont(20, 0, "Arial Bold");
    
    // 显示重启提示（带倒计时）
    if (gameOverDelay > 0 && gameOverDelay <= 90) {
        int remainingTime = (90 - gameOverDelay) / 30 + 1;  // 3秒倒计时
        std::string restartText = "Press any key to restart in " + std::to_string(remainingTime) + "s";
        int textWidth = textwidth(restartText.c_str());
        int xPos = (800 - textWidth) / 2;
        outtextxy(xPos, 250, restartText.c_str());
    } else if (gameOverDelay >= 91) {
        // 延迟结束，显示可以重启
        const char* restartText = "Press any key to restart";
        int textWidth = textwidth(restartText);
        int xPos = (800 - textWidth) / 2;
        outtextxy(xPos, 250, restartText);
    }
}

/**
 * @brief 清理游戏资源
 * @details 关闭图形窗口，释放资源
 */
void DinoGame::cleanup() {
    closegraph();  // 关闭EGE图形窗口
}

/**
 * @brief 动态生成障碍物
 * @details 按帧计数生成仙人掌或飞鸟，生成间隔随速度缩短，自动清理超出屏幕的障碍物
 * 
 * 生成间隔：max(20, 80 - gameSpeed*2)帧，速度越快间隔越短
 * 类型选择：
 *   - 随机数0-5，<3生成仙人掌，否则生成飞鸟
 *   - 仙人掌高度随机：20-80像素（7个等级）
 *   - 飞鸟高度随机：260-320像素（7个等级）
 * 内存管理：
 *   - 使用remove_if清理X<-50的障碍物
 *   - 智能指针自动释放内存
 */
void DinoGame::generateObstacle() {
    // 根据游戏速度计算生成间隔，速度越快间隔越短
    if (frameCount % std::max(20, 80 - gameSpeed * 2) == 0) {
        int type = rand() % 6;  // 随机生成类型0-5
        if (type < 3) {
            // 生成仙人掌（概率50%）
            int cactusHeight = 20 + (rand() % 7) * 10;  // 高度随机20-80
            obstacles.push_back(std::make_unique<Cactus>(800, 340, cactusHeight));
        } else {
            // 生成飞鸟（概率50%）
            int birdHeight = 260 + (rand() % 7) * 10;  // 高度随机260-320
            obstacles.push_back(std::make_unique<Bird>(800, birdHeight));
        }
    }
    
    // 清理已经移出屏幕左侧的障碍物（X<-50）
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
            [](const std::unique_ptr<Obstacle>& obs) {
                return obs->getX() < -50;  // 判断是否超出屏幕
            }),
        obstacles.end()
    );  // erase-remove习惯用法清理容器
}

/**
 * @brief 检测所有障碍物与恐龙的碰撞
 * @details 遍历障碍物容器，一旦检测到碰撞立即设置游戏结束状态
 * 
 * 检测流程：
 *   - 遍历obstacles容器中的每个障碍物
 *   - 调用障碍物的checkCollision方法
 *   - 传入player对象进行碰撞判定
 * 终止逻辑：
 *   - 一旦检测到碰撞，设置isGameOver=true
 *   - 立即break跳出循环，无需继续检测
 */
void DinoGame::checkCollisions() {
    for (const auto& obstacle : obstacles) {
        if (obstacle->checkCollision(player)) {  // 调用障碍物的碰撞检测方法
            isGameOver = true;  // 设置游戏结束标志
            break;              // 立即终止检测
        }
    }
}

/**
 * @brief 根据分数动态调整游戏速度和昼夜模式
 * @details 实现游戏难度递增机制
 * 
 * 速度计算：
 *   - gameSpeed = 5 + (分数/200)
 *   - 每200分增加1级速度
 *   - 上限为12级（防止过快无法游戏）
 * 
 * 昼夜切换：
 *   - 每700分切换一次模式
 *   - 通过 (分数/700) % 2 判断奇偶
 *   - 奇数为夜间模式，偶数为白天模式
 */
void DinoGame::updateGameSpeed() {
    // 根据分数计算新速度等级
    int newSpeed = 5 + (score.getCurrentScore() / 200);  // 每200分+1级
    if (newSpeed > 12) newSpeed = 12;  // 限制最高速度12级
    gameSpeed = newSpeed;
    
    // 根据分数切换昼夜模式
    bool isNight = (score.getCurrentScore() / 700) % 2 == 1;  // 每700分切换，奇数为夜间
    background.toggleNightMode(isNight);  // 设置背景模式
    score.setNightMode(isNight);          // 设置分数显示模式
}