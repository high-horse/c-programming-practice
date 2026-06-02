#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

#define WIDTH 800
#define HEIGHT 600
#define FPS 20

int main() {

    InitWindow(WIDTH, HEIGHT, "Pie Chart");

    SetTargetFPS(FPS);
    while (!WindowShouldClose()) {
        
    }

    CloseWindow();
    
    return EXIT_SUCCESS;
}