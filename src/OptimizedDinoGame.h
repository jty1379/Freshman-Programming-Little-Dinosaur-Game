#ifndef OPTIMIZED_DINO_GAME_H
#define OPTIMIZED_DINO_GAME_H

#include <vector>
#include <memory>
#include <graphics.h>
#include <ege.h>

// 前向声明
class Dinosaur;
class Obstacle;

// 恐龙类
class Dinosaur {
private:
    float x, y;
    float velocityY;
    bool isJumping;
    bool isDucking;
    int groundLevel;
    static const int DINO_WIDTH = 40;
    static const int DINO_HEIGHT = 50;
    static const int DINO_HEIGHT_DUCK = 30;

public:
    Dinosaur();
    ~Dinosaur();

    void jump();
    void duck();
    void stand();
    void update();
    void render();

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return DINO_WIDTH; }
    float getHeight() const { return isDucking ? DINO_HEIGHT_DUCK : DINO_HEIGHT; }
    bool getIsJumping() const { return isJumping; }
    bool getIsDucking() const { return isDucking; }

    // Setters
    void setPosition(float x, float y);
};

// 障碍物基类
class Obstacle {
protected:
    float x, y;
    float width, height;
    float speed;

public:
    Obstacle(float x, float y, float width, float height);
    virtual ~Obstacle();

    virtual void update(float gameSpeed = 0.0f);
    virtual void render();
    virtual bool checkCollision(const Dinosaur& dino);

    // Getters
    float getX() const { return x; }
    float getY() const { return y; }
    float getWidth() const { return width; }
    float getHeight() const { return height; }
};

// 仙人掌类
class Cactus : public Obstacle {
public:
    Cactus(float x, float y);
    virtual ~Cactus();

    virtual void render() override;
    virtual bool checkCollision(const Dinosaur& dino) override;
};

// 飞鸟类
class Bird : public Obstacle {
private:
    int wingPosition;
    int animationCounter;

public:
    Bird(float x, float y);
    virtual ~Bird();

    virtual void update(float gameSpeed = 0.0f) override;
    virtual void render() override;
    virtual bool checkCollision(const Dinosaur& dino) override;
};

// 背景类
class Background {
private:
    float scrollSpeed;
    float groundOffset;
    bool isNightMode;

public:
    Background();
    ~Background();

    void update();
    void render();
    void toggleNightMode(bool night);

    bool getIsNightMode() const { return isNightMode; }
};

// 得分类
class ScoreManager {
private:
    int currentScore;
    int highScore;
    bool isNightMode;

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

// 主游戏类
class DinoGame {
private:
    Dinosaur player;
    std::vector<std::unique_ptr<Obstacle>> obstacles;
    Background background;
    ScoreManager score;
    bool isRunning;
    bool isGameOver;
    int gameSpeed;
    int frameCount;

public:
    DinoGame();
    ~DinoGame();

    void initialize();
    void update();
    void render();
    void handleInput();
    void cleanup();

    bool isGameRunning() const { return isRunning; }
    int getCurrentScore() const { return score.getCurrentScore(); }

private:
    void generateObstacle();
    void checkCollisions();
    void updateGameSpeed();
    void showGameOverScreen();
};

#endif // OPTIMIZED_DINO_GAME_H