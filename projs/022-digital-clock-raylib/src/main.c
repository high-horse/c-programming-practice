#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "raylib.h"

#define W_WIDTH 800
#define W_HEIGHT 400
#define FPS 20

#define THICKNESS 20
#define LENGTH 60
#define TIP 15
#define OFFSET 35

#define DARKESTGRAY CLITERAL(Color){ 20, 20, 20, 255 }  // Dark gray
#define ON_COLOR GREEN
#define OFF_COLOR DARKESTGRAY

// top_left, top, top_right, bottom_right, bottom, bottom_left, middle
int numbers[10][7] = {
  {true, true, true, true, true, true, false}, // 0
  {false, false, true, true, false, false, false}, // 1
  {false, true, true, false, true, true, true}, // 2
  {false, true, true, true, true, false, true}, // 3
  {true, false, true, true, false, false, false}, // 4
  {true, true, false, true, true, false, true}, // 5
  {true, true, false, true, true, true, true}, // 6
  {false, true, true, false, false, false, false}, // 7
  {true, true, true, true, true, true, true}, // 8
  {true, true, true, true, true, false, true}, // 9
};

void paint_triangle_strip(Vector2 center, bool vertical, Color color) {
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
    DrawTriangleStrip(points, count, color);
}


void draw_digit_(int digit) {
    Vector2 center = {W_WIDTH / 2, W_HEIGHT / 2};
    int *color_flags = numbers[digit];
    float h = LENGTH;
    float off = OFFSET;

    // Horizontal segments
    Vector2 top    = {center.x, center.y - h - off};
    Vector2 middle = {center.x, center.y};
    Vector2 bottom = {center.x, center.y + h + off};

    // Vertical segments (perfectly aligned grid)
    Vector2 top_left     = {center.x - h/2 - off, center.y - h/2};
    Vector2 top_right    = {center.x + h/2 + off, center.y - h/2};
    Vector2 bottom_left  = {center.x - h/2 - off, center.y + h/2};
    Vector2 bottom_right = {center.x + h/2 + off, center.y + h/2};

    paint_triangle_strip(top_left, true, color_flags[0] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(top, false, color_flags[1] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(top_right, true, color_flags[2] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(bottom_right, true, color_flags[3] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(bottom, false, color_flags[4] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(bottom_left, true, color_flags[5] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(middle, false, color_flags[6] ? ON_COLOR : OFF_COLOR);
}

void draw_digit(int digit) {
    int *color_flags = numbers[digit];
    
    Vector2 center = (Vector2){W_WIDTH / 2, W_HEIGHT / 2};
    
    Vector2 top = {center.x , center.y - LENGTH - OFFSET};
    Vector2 middle = (Vector2){center.x , center.y};
    Vector2 bottom = (Vector2){center.x , center.y + LENGTH + OFFSET};
    
    Vector2 top_left = {center.x - LENGTH / 2  - OFFSET/2, center.y - LENGTH/2 - OFFSET /2}; 
    Vector2 top_right = (Vector2){center.x + LENGTH / 2 + OFFSET/2, center.y - LENGTH/2 - OFFSET /2};
    Vector2 bottom_right = (Vector2){center.x + LENGTH / 2 + OFFSET/2, center.y + LENGTH /2 + OFFSET /2};
    Vector2 bottom_left = (Vector2){center.x - LENGTH / 2 - OFFSET/2, center.y + LENGTH / 2 + OFFSET / 2};
   
    
    paint_triangle_strip(top_left, true, color_flags[0] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(top, false, color_flags[1] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(top_right, true, color_flags[2] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(bottom_right, true, color_flags[3] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(bottom, false, color_flags[4] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(bottom_left, true, color_flags[5] ? ON_COLOR : OFF_COLOR);
    paint_triangle_strip(middle, false, color_flags[6] ? ON_COLOR : OFF_COLOR);
    
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
        draw_digit(2);
        EndDrawing();
    }
    
    CloseWindow();
    return EXIT_SUCCESS;
}
