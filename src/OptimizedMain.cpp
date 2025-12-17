#include "OptimizedDinoGame.h"
#include <iostream>
#include <windows.h>

int main() {
    DinoGame game;
    
    game.initialize();
    
    while (game.isGameRunning()) {
        game.handleInput();
        game.update();
        game.render();
    }
    
    game.cleanup();
    
    std::cout << "Game Over! Final Score: " << game.getCurrentScore() << std::endl;
    
    return 0;
}