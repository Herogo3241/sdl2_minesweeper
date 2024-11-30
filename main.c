#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "src/constant.h"
#include "src/game.c"



int main() {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    SDL_Window *window = SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, GRID_WIDTH * TILE_SIZE, GRID_HEIGHT * TILE_SIZE, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return 1;
    }

    // Load font
    TTF_Font *font = TTF_OpenFont("assets/arial.ttf", 24);  // Change the path to the font file you want to use
    if (!font) {
        printf("Font could not be loaded! SDL_ttf Error: %s\n", TTF_GetError());
        return 1;
    }

    srand(time(NULL));
    initGrid();

    int quit = 0;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x = e.button.x / TILE_SIZE;
                int y = e.button.y / TILE_SIZE;

                if (e.button.button == SDL_BUTTON_LEFT) {
                    reveal(x, y);
                }
                // Handle right-click to flag (e.button.button == SDL_BUTTON_RIGHT)
            }
        }

        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Clear screen
        SDL_RenderClear(renderer);

        drawGrid(renderer, font);  // Draw the grid

        SDL_RenderPresent(renderer);
    }

    // Clean up
    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
