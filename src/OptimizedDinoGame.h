/**
 * @file OptimizedDinoGame.h
 * @brief Chrome离线小恐龙跑酷游戏头文件
 * @details 包含所有游戏类的声明：恐龙、障碍物、背景、分数管理器和游戏控制器
 */

#ifndef OPTIMIZED_DINO_GAME_H
#define OPTIMIZED_DINO_GAME_H

#include <vector>
#include <memory>
#include <graphics.h>
#include <ege.h>

class Dinosaur;
class Obstacle;

/**
 * @class Dinosaur
 * @brief 恐龙玩家类
 * @details 负责恐龙角色的跳跃、下蹲、移动和渲染，实现简化的物理模拟
 */
class Dinosaur {
private:
    float x, y;                          // 恐龙在屏幕上的位置坐标（x固定为50，y根据跳跃状态变化）
    float velocityY;                     // 垂直方向速度，用于跳跃物理模拟（负数向上，正数向下）
    bool isJumping;                      // 是否处于跳跃状态
    bool isDucking;                      // 是否处于下蹲状态
    int groundLevel;                     // 地面基准线Y=340，所有地面实体的参考坐标
    static const int DINO_WIDTH = 40;    // 恐龙正常宽度，用于碰撞检测
    static const int DINO_HEIGHT = 60;   // 恐龙站立高度，影响跳跃判定
    static const int DINO_HEIGHT_DUCK = 30; // 恐龙下蹲高度，用于躲避飞鸟

public:
    Dinosaur();
    ~Dinosaur();

    /**
     * @brief 执行跳跃动作
     * @details 设置isJumping标志并给予初始向上速度-15，只有在非跳跃和非下蹲状态下才能跳跃
     */
    void jump();
    
    /**
     * @brief 执行下蹲动作
     * @details 设置isDucking标志并调整高度，只有在非跳跃状态下才能下蹲
     */
    void duck();
    
    /**
     * @brief 恢复站立状态
     * @details 取消下蹲状态，恢复正常高度
     */
    void stand();
    
    /**
     * @brief 更新恐龙状态（每帧调用）
     * @details 实现跳跃物理模拟：更新垂直速度、位置，处理重力加速度和落地检测
     */
    void update();
    
    /**
     * @brief 渲染恐龙到屏幕
     * @details 根据当前状态（站立/下蹲/跳跃）绘制不同形态的恐龙图形
     */
    void render();

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return DINO_WIDTH; }
    float getHeight() const { return isDucking ? DINO_HEIGHT_DUCK : DINO_HEIGHT; }
    bool getIsJumping() const { return isJumping; }
    bool getIsDucking() const { return isDucking; }

    void setPosition(float x, float y);
};

/**
 * @class Obstacle
 * @brief 障碍物基类
 * @details 定义障碍物的通用行为：移动、渲染、碰撞检测。派生类包括Cactus和Bird
 */
class Obstacle {
protected:
    float x, y;              // 障碍物位置坐标
    float width, height;     // 障碍物尺寸
    float speed;             // 基础移动速度（固定为5像素/帧）

public:
    Obstacle(float x, float y, float width, float height);
    virtual ~Obstacle();

    /**
     * @brief 更新障碍物位置（每帧调用）
     * @param gameSpeed 游戏速度等级，影响实际移动速度
     * @details 实际速度 = speed + gameSpeed * 0.15，向左移动（X坐标减少）
     */
    virtual void update(float gameSpeed = 0.0f);
    
    /**
     * @brief 渲染障碍物到屏幕
     */
    virtual void render();
    
    /**
     * @brief 检测与恐龙的碰撞
     * @param dino 恐龙对象引用
     * @return 是否发生碰撞
     * @details 使用AABB（轴对齐包围盒）矩形碰撞检测算法
     */
    virtual bool checkCollision(const Dinosaur& dino);

    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
};

/**
 * @class Cactus
 * @brief 仙人掌障碍物类（地面障碍物）
 * @details 继承自Obstacle，固定宽度20像素，高度可变（20-80像素）
 */
class Cactus : public Obstacle {
public:
    Cactus(float x, float y);
    Cactus(float x, float y, float height);
    virtual ~Cactus();

    virtual void render() override;
    virtual bool checkCollision(const Dinosaur& dino) override;
};

/**
 * @class Bird
 * @brief 飞鸟障碍物类（空中障碍物）
 * @details 继承自Obstacle，具有翅膀扇动动画，高度随机（260-320像素）
 */
class Bird : public Obstacle {
private:
    int wingPosition;        // 翅膀动画帧索引，0或1交替
    int animationCounter;    // 动画计数器，每5帧切换一次翅膀状态

public:
    Bird(float x, float y);
    virtual ~Bird();

    virtual void update(float gameSpeed = 0.0f) override;
    virtual void render() override;
    virtual bool checkCollision(const Dinosaur& dino) override;
};

/**
 * @class Background
 * @brief 背景渲染类
 * @details 负责地面滚动、云朵装饰和昼夜模式切换
 */
class Background {
private:
    float scrollSpeed;       // 地面滚动速度（固定2像素/帧）
    float groundOffset;      // 地面滚动偏移量
    bool isNightMode;        // 是否为夜间模式

public:
    Background();
    ~Background();

    void update();
    void render();
    void toggleNightMode(bool night);

    bool getIsNightMode() const { return isNightMode; }
};

/**
 * @class ScoreManager
 * @brief 分数管理类
 * @details 负责分数累加、最高分记录和分数显示
 */
class ScoreManager {
private:
    int currentScore;        // 当前游戏分数（每帧+1）
    int highScore;           // 历史最高分（单次运行期间）
    bool isNightMode;        // 是否为夜间模式（影响文字颜色）

public:
    ScoreManager();
    ~ScoreManager();

    void update();
    void render();
    void reset();

    void incrementScore(int points = 1);
    int getCurrentScore() const { return currentScore; }
    int getHighScore() const { return highScore; }

    void setNightMode(bool night) { isNightMode = night; }
    bool getNightMode() const { return isNightMode; }
};

/**
 * @class DinoGame
 * @brief 游戏总控制器类
 * @details 协调所有模块，管理游戏状态、输入处理、碰撞检测和障碍物生成
 */
class DinoGame {
private:
    Dinosaur player;                                    // 玩家恐龙实例
    std::vector<std::unique_ptr<Obstacle>> obstacles;   // 障碍物容器（使用智能指针自动管理内存）
    Background background;                              // 背景渲染器
    ScoreManager score;                                 // 分数管理器
    bool isRunning;                                     // 游戏是否正在运行
    bool isGameOver;                                    // 游戏是否结束
    int gameSpeed;                                      // 当前游戏速度等级（5-12）
    int frameCount;                                     // 帧计数器，用于计算障碍物生成时机
    int gameOverDelay;                                  // 游戏结束延迟计数器，控制重启倒计时（3秒）

public:
    DinoGame();
    ~DinoGame();

    /**
     * @brief 初始化游戏
     * @details 创建窗口、重置状态、初始化随机数种子
     */
    void initialize();
    
    /**
     * @brief 更新游戏逻辑（每帧调用）
     * @details 更新恐龙、背景、分数、障碍物，执行碰撞检测和速度调整
     */
    void update();
    
    /**
     * @brief 渲染游戏画面（每帧调用）
     * @details 清空画布，依次渲染背景、恐龙、障碍物、分数，显示游戏结束界面
     */
    void render();
    
    /**
     * @brief 处理键盘输入（每帧调用）
     * @details 检测空格/W/S/ESC键，控制跳跃、下蹲和退出
     */
    void handleInput();
    
    /**
     * @brief 清理资源
     * @details 关闭图形窗口，释放资源
     */
    void cleanup();

    bool isGameRunning() const { return isRunning; }
    int getCurrentScore() const { return score.getCurrentScore(); }

private:
    /**
     * @brief 动态生成障碍物
     * @details 按帧计数生成仙人掌或飞鸟，生成间隔随速度缩短，自动清理超出屏幕的障碍物
     */
    void generateObstacle();
    
    /**
     * @brief 检测所有障碍物与恐龙的碰撞
     * @details 遍历障碍物容器，一旦检测到碰撞立即设置游戏结束状态
     */
    void checkCollisions();
    
    /**
     * @brief 根据分数动态调整游戏速度和昼夜模式
     * @details 每200分增加1级速度（上限12级），每700分切换一次昼夜模式
     */
    void updateGameSpeed();
    
    /**
     * @brief 显示游戏结束界面
     * @details 显示Game Over文字、分数和重启提示
     */
    void showGameOverScreen();
};

#endif // OPTIMIZED_DINO_GAME_H