#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>


#include "raylib.h"

#define W_WIDTH 800
#define W_HEIGHT 400
#define FPS 20

#define THICKNESS 25
#define LENGTH 80
#define TIP 15

void paint_triangle_strip(Vector2 center, bool vertical) {
    Vector2 a, b ,c ,d, e, f;
    int count = 6;
    
    if(vertical) {
        a = (Vector2){center.x, center.y  - LENGTH / 2 - TIP};
        b = (Vector2){center.x + THICKNESS / 2 , center.y - LENGTH / 2};
        c = (Vector2){center.x - THICKNESS / 2 , center.y - LENGTH / 2};
        d = (Vector2){center.x + THICKNESS / 2, center.y + LENGTH / 2};
        e = (Vector2){center.x, center.y  + LENGTH / 2 + TIP };
        f = (Vector2){center.x - THICKNESS / 2, center.y + LENGTH / 2};
    } else {
        a = (Vector2){center.x - LENGTH / 2 - TIP, center.y};
        b = (Vector2){center.x - LENGTH / 2 , center.y - THICKNESS / 2};
        c = (Vector2){center.x - LENGTH / 2 , center.y + THICKNESS / 2};
        d = (Vector2){center.x + LENGTH / 2, center.y - THICKNESS / 2};
        e = (Vector2){center.x + LENGTH / 2 + TIP, center.y };
        f = (Vector2){center.x + LENGTH / 2, center.y + THICKNESS / 2};
    }
   
    Vector2 points[]  = {a, c, b, f, d, e};
    DrawTriangleStrip(points, count, GREEN);
}


void draw_digit_(int digit) {
    Vector2 center = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};

    // Horizontal segments
    Vector2 top    = {center.x, center.y - LENGTH};
    Vector2 middle = {center.x, center.y};
    Vector2 bottom = {center.x, center.y + LENGTH};

    // Vertical segments
    Vector2 topLeft     = {center.x - LENGTH / 2, center.y - LENGTH / 2};
    Vector2 topRight    = {center.x + LENGTH / 2, center.y - LENGTH / 2};
    Vector2 bottomLeft  = {center.x - LENGTH / 2, center.y + LENGTH / 2};
    Vector2 bottomRight = {center.x + LENGTH / 2, center.y + LENGTH / 2};

    // Draw a "7" shape (your target)
    paint_triangle_strip(top, false);        // top horizontal
    paint_triangle_strip(topLeft, true);    // right vertical
}

void draw_digit(int digit) {
    Vector2 center = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};
    
    Vector2 top_left = {center.x - LENGTH / 2, center.y - LENGTH/2}; 
    
    Vector2 top = {center.x , center.y - LENGTH};
    
    Vector2 top_right = (Vector2){center.x + LENGTH / 2 , center.y - LENGTH /2};
    Vector2 bottom_right = (Vector2){center.x + LENGTH / 2 , center.y + LENGTH /2};
    Vector2 bottom = (Vector2){center.x , center.y + LENGTH };
    Vector2 bottom_left = (Vector2){center.x - LENGTH / 2 , center.y + LENGTH / 2};
    Vector2 middle = (Vector2){center.x , center.y};
    
    paint_triangle_strip(top_left, true);
    paint_triangle_strip(top, false);
    paint_triangle_strip(top_right, true);
    paint_triangle_strip(bottom_right, true);
    paint_triangle_strip(bottom, false);
    paint_triangle_strip(bottom_left, true);
    paint_triangle_strip(middle, false);
    
    
    
    
    
}

int main() {
    time_t current =  time(NULL);
    struct tm *current_localtime = localtime(&current);
    printf("time %d:%d:%d\n", current_localtime->tm_hour, current_localtime->tm_min, current_localtime->tm_sec);
    InitWindow(W_WIDTH, W_HEIGHT, "raylib example - basic window");
    SetTargetFPS(FPS);
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        // draw_rectangle();
        draw_digit(1);
        EndDrawing();
    }
    
    CloseWindow();
    return EXIT_SUCCESS;
}
