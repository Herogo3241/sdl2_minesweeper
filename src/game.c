#include "game.h"

typedef struct {
    int value;   // -1 for mine, 0-8 for numbers
    int revealed;
    int flagged;
} Cell;

Cell grid[GRID_HEIGHT][GRID_WIDTH];

void initGrid() {
    // Initialize grid with 0s
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            grid[i][j].value = 0;
            grid[i][j].revealed = 0;
            grid[i][j].flagged = 0;
        }
    }

    // Randomly place mines
    int minesPlaced = 0;
    while (minesPlaced < NUM_MINES) {
        int x = rand() % GRID_WIDTH;
        int y = rand() % GRID_HEIGHT;
        if (grid[y][x].value == 0) {
            grid[y][x].value = -1;  // Place mine
            minesPlaced++;
        }
    }

    // Calculate numbers for adjacent cells
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            if (grid[i][j].value == -1) continue;
            int mineCount = 0;
            for (int dx = -1; dx <= 1; dx++) {
                for (int dy = -1; dy <= 1; dy++) {
                    int nx = i + dx, ny = j + dy;
                    if (nx >= 0 && nx < GRID_HEIGHT && ny >= 0 && ny < GRID_WIDTH && grid[nx][ny].value == -1) {
                        mineCount++;
                    }
                }
            }
            grid[i][j].value = mineCount;
        }
    }
}

void reveal(int x, int y) {
    if (x < 0 || x >= GRID_WIDTH || y < 0 || y >= GRID_HEIGHT || grid[y][x].revealed)
        return;
    
    grid[y][x].revealed = 1;

    // If the cell is a number, stop
    if (grid[y][x].value > 0) return;

    // If the cell is empty, reveal neighboring cells recursively
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = x + dx, ny = y + dy;
            if (nx >= 0 && nx < GRID_WIDTH && ny >= 0 && ny < GRID_HEIGHT) {
                reveal(nx, ny);
            }
        }
    }
}

void drawGrid(SDL_Renderer *renderer, TTF_Font *font) {
    for (int i = 0; i < GRID_HEIGHT; i++) {
        for (int j = 0; j < GRID_WIDTH; j++) {
            SDL_Rect rect = { j * TILE_SIZE, i * TILE_SIZE, TILE_SIZE, TILE_SIZE };
            if (grid[i][j].revealed) {
                if (grid[i][j].value == -1) {
                    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red for mine
                } else {
                    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light gray for revealed cells
                }
            } else {
                SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255); // Dark gray for unrevealed cells
            }
            SDL_RenderFillRect(renderer, &rect);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Border color
            SDL_RenderDrawRect(renderer, &rect);

            // Render numbers if the cell is revealed and has a number
            if (grid[i][j].revealed && grid[i][j].value > 0) {
                SDL_Color color = {0, 0, 0, 255}; // Black for numbers
                char text[2];
                sprintf(text, "%d", grid[i][j].value);
                SDL_Surface *textSurface = TTF_RenderText_Solid(font, text, color);
                SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

                int textWidth = textSurface->w;
                int textHeight = textSurface->h;
                SDL_Rect textRect = { j * TILE_SIZE + (TILE_SIZE - textWidth) / 2, i * TILE_SIZE + (TILE_SIZE - textHeight) / 2, textWidth, textHeight };

                SDL_RenderCopy(renderer, textTexture, NULL, &textRect);
                SDL_FreeSurface(textSurface);
                SDL_DestroyTexture(textTexture);
            }
        }
    }
}
