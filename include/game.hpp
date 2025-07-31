#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <filesystem>

#include "gameIA.hpp"

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, int width, int height);
    void run();
    void cleanup();

private:
    bool isRunning, hasWinner;
    GameBoard gameBoard;
    GameIA gameIA;

    SDL_Window* window;
    SDL_Renderer* renderer;

    // textures
    SDL_Texture *mause, *line, *cross, *circle;

    void processInput();
    void update();
    void render();

    void getBoardIndex(int mouseX, int mouseY, int& row, int& col);
};