#include<SDL2/SDL.h>
#include<SDL2/SDL_ttf.h>
#include<stdbool.h>
#include"graphics.h"
bool enable_grid= false;
void button() {
    // Define the button rectangle
    SDL_Rect button_rect = {0, 0, 50, 20}; // x, y, width, height

    // Draw the button background
    draw_rect(button_rect.x, button_rect.y, button_rect.w, button_rect.h, COLOR_GRAY);

    // Draw the button label
    draw_text("Grid", button_rect.x + 1, button_rect.y + 1, 12, COLOR_GRASS);

    // Get the mouse position
    int mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    SDL_Point mouse_point = {mouse_x, mouse_y};

    // Check if the mouse is within the button
    if (SDL_PointInRect(&mouse_point, &button_rect)) {
        enable_grid = true; 
    }
}
int main(){
	init_window(640, 480, "Level Editor", 60);
	
	while(running()){
		clear_screen(COLOR_SKY_BLUE);
		button();
		present();
	}
	
	quit_window();
	return 0;
}