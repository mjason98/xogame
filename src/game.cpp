#include "game.hpp"
#include <iostream>

Game::Game() : isRunning(false), window(nullptr), renderer(nullptr) {}

Game::~Game() {}

bool Game::init(const std::string& title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL Init Error: " << SDL_GetError() << std::endl;
        return false;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        std::cerr << "SDL_image could not initialize! IMG_Error: " << IMG_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow(title.c_str(),
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window Error: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer Error: " << SDL_GetError() << std::endl;
        return false;
    }
    
    // Set internal resolution
    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
    SDL_RenderSetLogicalSize(renderer, 256, 256);
    SDL_SetWindowGrab(window, SDL_TRUE);

    // load resources here
    // std::string path = std::filesystem::current_path().string() + "/assets/images/npc1.png";
    
    // ============================================================
    mause = IMG_LoadTexture(renderer, "assets/images/mause.png");
    if (!mause) {
        std::cerr << "Failed to load mause image: " << IMG_GetError() << std::endl;
    }
    SDL_ShowCursor(SDL_DISABLE);
    circle = IMG_LoadTexture(renderer, "assets/images/circle.png");
    if (!circle) {
        std::cerr << "Failed to load circle image: " << IMG_GetError() << std::endl;
    }
    cross = IMG_LoadTexture(renderer, "assets/images/cross.png");
    if (!cross) {
        std::cerr << "Failed to load cross image: " << IMG_GetError() << std::endl;
    }
    line = IMG_LoadTexture(renderer, "assets/images/line.png");
    if (!line) {
        std::cerr << "Failed to load line image: " << IMG_GetError() << std::endl;
    }
    // ============================================================

    isRunning = true;
    hasWinner = false;
    return true;
}

void Game::run() {
    while (isRunning) {
        processInput();
        update();
        render();
        SDL_Delay(16);  // ~60 FPS
    }
}

void Game::getBoardIndex(int mouseX, int mouseY, int& row, int& col) {
    mouseX -= 16; // Adjust for board offset
    mouseY -= 16; // Adjust for board offset

    if (mouseX < 0 || mouseY < 0) {
        row = -1;
        col = -1; // Out of bounds
        return;
    }
    
    row = mouseY / 80; // Assuming each cell is 80 pixels high
    col = mouseX / 80; // Assuming each cell is 80 pixels wide
    
    if (row < 0 || row >= 3 || col < 0 || col >= 3) {
        row = -1;
        col = -1; // Out of bounds
    }
    if (mouseX > col*80 + 64 || mouseY > row*80 + 64) {
        row = -1;
        col = -1; // Out of bounds
    }
}

void Game::processInput() {
    SDL_Event event;
    int row, col;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            isRunning = false;
        }
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                std::cout << "Left Click at (" << event.button.x << ", " << event.button.y << ")\n";
                
                if (hasWinner) {
                    std::cout << "Game already finished!\n";
                    continue; // Ignore clicks if game is over
                }

                getBoardIndex(event.button.x*2, event.button.y*2, row, col);
                
                if (row != -1 && col != -1) {
                    std::cout << "Clicked on cell (" << row << ", " << col << ")\n";
                    if (gameBoard.board[row][col] == 0) { // If the cell is empty
                        gameBoard.board[row][col] = 1; // Player's move
                        // Call GameIA to make a move for the AI
                        auto move = gameIA.findBestMove(gameBoard, 2); // AI's move
                        std::cout << "AI's move at (" << move.y << ", " << move.x << ")\n";
                        
                        if (move.y < 0 || move.x < 0) {
                            std::cout << "No valid move found by AI!\n";
                            hasWinner = true;
                        } else {
                            gameBoard.board[move.y][move.x] = 2; // Update board with AI's move
                        }
                    } else {
                        std::cout << "Cell already occupied!\n";
                    }
                }
            } else if (event.button.button == SDL_BUTTON_RIGHT) {
                std::cout << "Right Click\n";
            } else if (event.button.button == SDL_BUTTON_MIDDLE) {
                std::cout << "Middle Click\n";
            }
        }
    }
}

void Game::update() {
    // Game logic update here
}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 232, 183, 150, 255);
    SDL_RenderClear(renderer);

    // Render gameboard =================================
    SDL_Point center = { 8, 32 };           // Rotation pivot point (center of image)
    SDL_RendererFlip flip = SDL_FLIP_NONE;
    int boardOffsetX = 16, boardOffsetY = 16; // adjust if needed
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            SDL_Rect cellRect = { gameBoard.boardPositions[i][j].x + boardOffsetX, gameBoard.boardPositions[i][j].y + boardOffsetY, 64, 64 };
            if (gameBoard.board[i][j] == 1) {
                SDL_RenderCopy(renderer, cross, nullptr, &cellRect);
            } else if (gameBoard.board[i][j] == 2) {
                SDL_RenderCopy(renderer, circle, nullptr, &cellRect);
            }
            if (j < 2) {
                SDL_Rect lineRect = { gameBoard.boardPositions[i][j].x + 64 + boardOffsetX, gameBoard.boardPositions[i][j].y + boardOffsetY, 16, 64 };
                SDL_RenderCopy(renderer, line, nullptr, &lineRect);
            }
            if (i < 2) {
                SDL_Rect lineRect = { gameBoard.boardPositions[i][j].x + 24 + boardOffsetX, gameBoard.boardPositions[i][j].y + 40 + boardOffsetY, 16, 64 };
                SDL_RenderCopyEx(renderer, line, nullptr, &lineRect, 90.0, &center, flip);
            }
        }
    }

    // mause  ----------------------
    int x, y;
    int mouseOffsetX = -16, mouseOffsetY = -1; // adjust if needed
    SDL_GetMouseState(&x, &y);
    SDL_Rect cursorRect = { x + mouseOffsetX, y + mouseOffsetY, 32, 32 }; // adjust size
    SDL_RenderCopy(renderer, mause, nullptr, &cursorRect);
    // -----------------------------

    SDL_RenderPresent(renderer);
}

void Game::cleanup() {
    // clean up resources
    // SDL_DestroyTexture(texture);

    SDL_DestroyTexture(mause);
    SDL_DestroyTexture(cross);
    SDL_DestroyTexture(circle);
    SDL_DestroyTexture(line);

    IMG_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}