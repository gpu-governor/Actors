// Global SDL state
SDL_Window* main_window = NULL;
SDL_Renderer* main_renderer = NULL;
bool main_window_should_close = false;
Uint32 main_target_frame_time = 0;

// COLOR
typedef struct {
    uint8_t r, g, b, a;
} Color;
const Color COLOR_GRAY = {128, 128, 128, 255};
const Color COLOR_SKY_BLUE = {135, 206, 235, 255};
const Color COLOR_GRASS ={94,199,78,255};
const Color COLOR_SAND ={168,110,52,255};


// Global font for text rendering
TTF_Font* main_font = NULL;
const char* font_file = "fonts/FreeMono.ttf";

//--------------------------UTILITY FUNCTIONS-----------------------------------------

// Initialize the window, renderer, and font
void init_window(int width, int height, const char* title, int target_fps) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Failed to initialize SDL: %s", SDL_GetError());
        return;
    }

    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf: %s", TTF_GetError());
        SDL_Quit();
        return;
    }

    main_window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
    if (!main_window) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        TTF_Quit();
        SDL_Quit();
        return;
    }

    main_renderer = SDL_CreateRenderer(main_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!main_renderer) {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(main_window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // Load the default font
    main_font = TTF_OpenFont(font_file, 24);
    if (!main_font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        SDL_DestroyRenderer(main_renderer);
        SDL_DestroyWindow(main_window);
        TTF_Quit();
        SDL_Quit();
        return;
    }

    // Set target frame time
    if (target_fps > 0) {
        main_target_frame_time = 1000 / target_fps;
    }
}

// Check if the window should close
bool running() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            main_window_should_close = true;
        }
    }
    return !main_window_should_close; // Return true if the window should keep running
}
// Set the background color
void clear_screen(Color color) {
    SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, color.a);
    SDL_RenderClear(main_renderer);
}

// Draw a rectangle
void draw_rect(int x, int y, int width, int height, Color color) {
    SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, color.a);
    SDL_Rect rect = {x, y, width, height};
    SDL_RenderFillRect(main_renderer, &rect);
}

// Draw a circle
void draw_circle(int x, int y, int radius, Color color) {
    SDL_SetRenderDrawColor(main_renderer, color.r, color.g, color.b, color.a);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w; // Horizontal offset
            int dy = radius - h; // Vertical offset
            if ((dx * dx + dy * dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(main_renderer, x + dx, y + dy);
            }
        }
    }
}

// Draw text using a dynamic font size
void draw_text(const char* text, int x, int y, int font_size, Color color) {
    TTF_Font* temp_font = TTF_OpenFont(font_file, font_size);
    if (!temp_font) {
        SDL_Log("Failed to load font with size %d: %s", font_size, TTF_GetError());
        return;
    }

    SDL_Color sdl_color = {color.r, color.g, color.b, color.a};
    SDL_Surface* surface = TTF_RenderText_Solid(temp_font, text, sdl_color);
    if (!surface) {
        TTF_CloseFont(temp_font);
        return;
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(main_renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_FreeSurface(surface);
    if (texture) {
        SDL_RenderCopy(main_renderer, texture, NULL, &dest);
        SDL_DestroyTexture(texture);
    }

    TTF_CloseFont(temp_font);
}

// End drawing and present to the screen (with FPS delay)
void present() {
    SDL_RenderPresent(main_renderer);

    // Delay to maintain FPS
    if (main_target_frame_time > 0) {
        static Uint32 last_tick = 0;
        Uint32 elapsed = SDL_GetTicks() - last_tick;
        if (elapsed < main_target_frame_time) {
            SDL_Delay(main_target_frame_time - elapsed);
        }
        last_tick = SDL_GetTicks();
    }
}

// Close and clean up SDL and font
void quit_window() {
    if (main_font) TTF_CloseFont(main_font);
    if (main_renderer) SDL_DestroyRenderer(main_renderer);
    if (main_window) SDL_DestroyWindow(main_window);
    TTF_Quit();
    SDL_Quit();
}