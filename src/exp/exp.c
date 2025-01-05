#include <stdio.h>
#include <stdlib.h>

int** grid;
int gridWidth = 20;
int gridHeight = 15;

void initializeGrid(int width, int height) {
    gridWidth = width;
    gridHeight = height;
    grid = (int**)malloc(height * sizeof(int*));
    for (int i = 0; i < height; i++) {
        grid[i] = (int*)malloc(width * sizeof(int));
        for (int j = 0; j < width; j++) {
            grid[i][j] = 0; // Initialize all tiles as empty
        }
    }
}

void freeGrid() {
    for (int i = 0; i < gridHeight; i++) {
        free(grid[i]);
    }
    free(grid);
}

// save grid to 
void saveGridToCSV(const char* filename) {
    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Error opening file for saving");
        return;
    }

    for (int i = 0; i < gridHeight; i++) {
        for (int j = 0; j < gridWidth; j++) {
            fprintf(file, "%d", grid[i][j]);
            if (j < gridWidth - 1) {
                fprintf(file, ",");
            }
        }
        fprintf(file, "\n");
    }

    fclose(file);
    printf("Grid saved to %s\n", filename);
}

//read
void loadGridFromCSV(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file for loading");
        return;
    }

    int newHeight = 0;
    int newWidth = 0;
    char line[1024];

    // Calculate the new grid dimensions
    while (fgets(line, sizeof(line), file)) {
        newHeight++;
        if (newHeight == 1) {
            char* token = strtok(line, ",");
            while (token) {
                newWidth++;
                token = strtok(NULL, ",");
            }
        }
    }
    rewind(file);

    // Free the old grid and initialize a new one
    freeGrid();
    initializeGrid(newWidth, newHeight);

    // Read the CSV values into the grid
    int y = 0;
    while (fgets(line, sizeof(line), file)) {
        int x = 0;
        char* token = strtok(line, ",");
        while (token) {
            grid[y][x++] = atoi(token);
            token = strtok(NULL, ",");
        }
        y++;
    }

    fclose(file);
    printf("Grid loaded from %s\n", filename);
}

void renderGrid(SDL_Renderer* renderer, int tileSize) {
    for (int y = 0; y < gridHeight; y++) {
        for (int x = 0; x < gridWidth; x++) {
            SDL_Rect tileRect = {x * tileSize, y * tileSize, tileSize, tileSize};
            switch (grid[y][x]) {
                case 1: // Grass
                    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
                    break;
                case 2: // Water
                    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
                    break;
                default: // Empty
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
                    break;
            }
            SDL_RenderFillRect(renderer, &tileRect);
        }
    }
}

void handleMouseClick(int mouseX, int mouseY, int tileSize, int selectedTileID) {
    int gridX = mouseX / tileSize;
    int gridY = mouseY / tileSize;

    if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) {
        grid[gridY][gridX] = selectedTileID;
    }
}

int main(int argc, char* argv[]) {
    // Initialization
    InitWindow("Level Editor", 800, 600); // Your custom window function
    initializeGrid(20, 15);

    int running = 1;
    int selectedTileID = 1;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                if (event.button.button == SDL_BUTTON_LEFT) {
                    handleMouseClick(event.button.x, event.button.y, 32, selectedTileID);
                }
            } else if (event.type == SDL_KEYDOWN) {
                if (event.key.keysym.sym == SDLK_1) selectedTileID = 1; // Grass
                if (event.key.keysym.sym == SDLK_2) selectedTileID = 2; // Water
                if (event.key.keysym.sym == SDLK_s) saveGridToCSV("level.csv");
                if (event.key.keysym.sym == SDLK_l) loadGridFromCSV("level.csv");
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        renderGrid(renderer, 32);
        SDL_RenderPresent(renderer);
    }

    // Cleanup
    freeGrid();
    quitWindow(); // Your custom cleanup function
    return 0;
}