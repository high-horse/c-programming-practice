#include "libs/draw.h"

int main() {    
    time_t current;
    struct tm *current_localtime;
    
    InitWindow(W_WIDTH, W_HEIGHT, "DIGITAL CLOCK");
    SetWindowState(FLAG_WINDOW_UNDECORATED);     
    SetTargetFPS(FPS);
    
    while (!WindowShouldClose()) {
        current = time(NULL);
        current_localtime = localtime(&current);
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        draw_clock(current_localtime->tm_hour, current_localtime->tm_min, current_localtime->tm_sec);
        
        EndDrawing();
    }
    
    CloseWindow();
    return EXIT_SUCCESS;
}
