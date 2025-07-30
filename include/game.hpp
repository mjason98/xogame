#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <string>
#include <filesystem>

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

    // textures
    SDL_Texture* texture;
    SDL_Texture* mause;

    void processInput();
    void update();
    void render();
};