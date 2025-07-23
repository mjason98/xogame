#pragma once
#include <SDL.h>
#include <string>

class Game {
public:
    Game();
    ~Game();

    bool init(const std::string& title, int width, int height);
    void run();
    void cleanup();

private:
    bool isRunning;
    SDL_Window* window;
    SDL_Renderer* renderer;

    void processInput();
    void update();
    void render();
};