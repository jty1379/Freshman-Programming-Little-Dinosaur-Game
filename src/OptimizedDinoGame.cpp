#include "OptimizedDinoGame.h"
#include <conio.h>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <string>

// Dinosaur 实现
Dinosaur::Dinosaur() : x(50), y(340 - DINO_HEIGHT), velocityY(0), isJumping(false), isDucking(false), groundLevel(340) {}

Dinosaur::~Dinosaur() {}

void Dinosaur::jump() {
    if (!isJumping && !isDucking) {
        isJumping = true;
        velocityY = -15;
    }
}

void Dinosaur::duck() {
    if (!isJumping) {
        isDucking = true;
        y = groundLevel - DINO_HEIGHT_DUCK;
    }
}

void Dinosaur::stand() {
    isDucking = false;
    y = groundLevel - DINO_HEIGHT;
}

void Dinosaur::update() {
    if (isJumping) {
        y += velocityY;
        velocityY += 1;
        
        if (y >= groundLevel - DINO_HEIGHT) {
            y = groundLevel - DINO_HEIGHT;
            isJumping = false;
            velocityY = 0;
        }
    }
}

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

// Obstacle 实现
Obstacle::Obstacle(float x, float y, float width, float height) 
    : x(x), y(y), width(width), height(height), speed(5) {}

Obstacle::~Obstacle() {}

void Obstacle::update(float gameSpeed) {
    x -= (speed + gameSpeed * 0.15f);
}

void Obstacle::render() {
    setfillcolor(BLACK);
    solidrect(x, y, x + width, y + height);
}

bool Obstacle::checkCollision(const Dinosaur& dino) {
    return (dino.getX() + dino.getWidth() > x &&
            dino.getX() < x + width &&
            dino.getY() + dino.getHeight() > y &&
            dino.getY() < y + height);
}

// Cactus 实现
Cactus::Cactus(float x, float y) : Obstacle(x, y, 20, 40) {}

Cactus::~Cactus() {}

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
    return Obstacle::checkCollision(dino);
}

// Bird 实现
Bird::Bird(float x, float y) : Obstacle(x, y, 30, 20), wingPosition(0), animationCounter(0) {}

Bird::~Bird() {}

void Bird::update(float gameSpeed) {
    Obstacle::update(gameSpeed);
    
    animationCounter++;
    if (animationCounter % 5 == 0) {
        wingPosition = (wingPosition + 1) % 2;
    }
}

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

bool Bird::checkCollision(const Dinosaur& dino) {
    bool isLowFlying = (y >= 310);
    
    if (isLowFlying) {
        if (dino.getIsJumping()) {
            if (dino.getY() + dino.getHeight() <= y) {
                return false;
            }
        }
    } else {
        if (dino.getIsDucking()) {
            if (dino.getY() + dino.getHeight() <= y + 20) {
                return false;
            }
        }
    }
    
    return (dino.getX() + dino.getWidth() > x &&
            dino.getX() < x + width &&
            dino.getY() + dino.getHeight() > y &&
            dino.getY() < y + height);
}

// Background 实现
Background::Background() : scrollSpeed(2), groundOffset(0), isNightMode(false) {}

Background::~Background() {}

void Background::update() {
    groundOffset += scrollSpeed;
    if (groundOffset >= 20) {
        groundOffset = 0;
    }
}

void Background::render() {
    if (isNightMode) {
        setbkcolor(RGB(50, 50, 50));
    } else {
        setbkcolor(WHITE);
    }
    
    setfillcolor(RGB(100, 100, 100));
    solidrect(0, 340, 800, 400);
    
    setfillcolor(RGB(80, 80, 80));
    for (int i = 0; i < 800; i += 20) {
        int offset = (int)groundOffset % 20;
        solidrect(i - offset, 340, i - offset + 10, 350);
    }
    
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

// ScoreManager 实现
ScoreManager::ScoreManager() : currentScore(0), highScore(0) {}

ScoreManager::~ScoreManager() {}

void ScoreManager::update() {
    currentScore++;
    
    if (currentScore > highScore) {
        highScore = currentScore;
    }
}

void ScoreManager::render() {
    setfont(20, 0, "Arial");
    
    if (isNightMode) {
        setcolor(WHITE);
    } else {
        setcolor(BLACK);
    }
    
    std::string scoreText = "Score: " + std::to_string(currentScore);
    outtextxy(650, 20, scoreText.c_str());
    
    std::string highScoreText = "Best: " + std::to_string(highScore);
    outtextxy(650, 50, highScoreText.c_str());
}

void ScoreManager::reset() {
    currentScore = 0;
}

void ScoreManager::incrementScore(int points) {
    currentScore += points;
}

// DinoGame 实现
DinoGame::DinoGame() : isRunning(false), isGameOver(false), gameSpeed(5), frameCount(0) {}

DinoGame::~DinoGame() {
    cleanup();
}

void DinoGame::initialize() {
    if (isRunning) {
        obstacles.clear();
        score.reset();
    } else {
        initgraph(800, 400);
        setbkcolor(WHITE);
        cleardevice();
        ege::setrendermode(RENDER_MANUAL);
        
        srand((unsigned int)time(nullptr));
    }
    
    player.setPosition(50, 340 - 50);
    isRunning = true;
    isGameOver = false;
    frameCount = 0;
    gameSpeed = 5;
}

void DinoGame::update() {
    if (!isRunning) return;
    
    if (isGameOver) return;
    
    player.update();
    background.update();
    score.update();
    generateObstacle();
    
    for (auto& obstacle : obstacles) {
        obstacle->update(gameSpeed);
    }
    
    checkCollisions();
    updateGameSpeed();
    
    frameCount++;
}

void DinoGame::render() {
    if (!isRunning) return;
    
    cleardevice();
    background.render();
    player.render();
    
    for (const auto& obstacle : obstacles) {
        obstacle->render();
    }
    
    score.render();
    
    if (isGameOver) {
        showGameOverScreen();
    }
    
    ege::delay_ms(30);
}

void DinoGame::handleInput() {
    if (kbhit()) {
        char key = getch();
        
        if (isGameOver) {
            initialize();
            isGameOver = false;
            return;
        }
        
        switch (key) {
        case ' ':
        case 'w':
        case 'W':
            if (!player.getIsJumping() && !player.getIsDucking()) {
                player.jump();
            } else if (player.getIsDucking()) {
                player.stand();
            }
            break;
        case 's':
        case 'S':
        case 80:
            if (!player.getIsJumping()) {
                if (player.getIsDucking()) {
                    player.stand();
                } else {
                    player.duck();
                }
            }
            break;
        case 27:
            isRunning = false;
            break;
        }
    }
}

void DinoGame::showGameOverScreen() {
    setfont(30, 0, "Arial");
    setcolor(RED);
    
    outtextxy(300, 150, "Game Over");
    
    std::string scoreText = "Score: " + std::to_string(score.getCurrentScore());
    outtextxy(300, 200, scoreText.c_str());
    
    setfont(20, 0, "Arial");
    outtextxy(250, 250, "Press any key to restart");
}

void DinoGame::cleanup() {
    closegraph();
}

void DinoGame::generateObstacle() {
    if (frameCount % std::max(20, 80 - gameSpeed * 2) == 0) {
        int type = rand() % 4;
        if (type == 0) {
            obstacles.push_back(std::make_unique<Cactus>(800, 340 - 40));
        } else if (type <= 2) {
            obstacles.push_back(std::make_unique<Bird>(800, 320));
        } else {
            obstacles.push_back(std::make_unique<Bird>(800, 280));
        }
    }
    
    obstacles.erase(
        std::remove_if(obstacles.begin(), obstacles.end(),
            [](const std::unique_ptr<Obstacle>& obs) {
                return obs->getX() < -50;
            }),
        obstacles.end()
    );
}

void DinoGame::checkCollisions() {
    for (const auto& obstacle : obstacles) {
        if (obstacle->checkCollision(player)) {
            isGameOver = true;
            break;
        }
    }
}

void DinoGame::updateGameSpeed() {
    int newSpeed = 5 + (score.getCurrentScore() / 200);
    if (newSpeed > 12) newSpeed = 12;
    gameSpeed = newSpeed;
    
    bool isNight = (score.getCurrentScore() / 700) % 2 == 1;
    background.toggleNightMode(isNight);
    score.setNightMode(isNight);
}