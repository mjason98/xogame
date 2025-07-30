#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <filesystem>

#include "gameMath.hpp"

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, int width, int height);
    void run();
    void cleanup();

private:
    bool isRunning;
    int gameboard[3][3]; // 3x3 game board for XO game
    GamePoint boardPositions[3][3]; // Store positions for each cell

    SDL_Window* window;
    SDL_Renderer* renderer;

    // textures
    SDL_Texture *mause, *line, *cross, *circle;

    void processInput();
    void update();
    void render();

    void getBoardIndex(int mouseX, int mouseY, int& row, int& col);
};