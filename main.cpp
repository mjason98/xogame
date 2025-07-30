#include "game.hpp"

int main(int argc, char* argv[]) {
    Game game;
    // if (!game.init("My Cross Platform Game", 800, 600)) return -1;
    if (!game.init("My Cross Platform Game", 512, 512)) return -1;
    game.run();
    game.cleanup();
    return 0;
}