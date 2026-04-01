#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "raylib.h"

#define W_WIDTH 970
#define W_HEIGHT 400
#define FPS 10

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
  {true, false, true, true, false, false, true}, // 4
  {true, true, false, true, true, false, true}, // 5
  {true, true, false, true, true, true, true}, // 6
  {false, true, true, true, false, false, false}, // 7
  {true, true, true, true, true, true, true}, // 8
  {true, true, true, true, true, false, true}, // 9
};

Vector2 start_pos = {LENGTH+ OFFSET, W_HEIGHT / 2};

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



void draw_digit(int digit, Vector2 center) {
    int *color_flags = numbers[digit];
    
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

void draw_colon(Vector2 center) {
    double_t t = GetTime();
    
    bool visible = ((int)(t * 4) % 2) == 0;
    
    Vector2 top = (Vector2){center.x, center.y - LENGTH / 2 - OFFSET / 2};
    Vector2 bottom = (Vector2){center.x, center.y + LENGTH / 2 + OFFSET / 2};
    DrawCircle(top.x, top.y, THICKNESS * 2/3, visible ? ON_COLOR : OFF_COLOR);
    DrawCircle(bottom.x, bottom.y, THICKNESS * 2/3, visible ? ON_COLOR : OFF_COLOR);
}

void draw_clock(int hour, int minute, int second) {
    int x = start_pos.x;
    draw_digit(hour / 10, start_pos);
    // start_pos.x += LENGTH + OFFSET;
    x += LENGTH + OFFSET * 2;
    draw_digit(hour % 10, (Vector2) {x, start_pos.y});
    x += LENGTH + OFFSET;
    draw_colon((Vector2) {x , start_pos.y});
    x += LENGTH + OFFSET;
    draw_digit(minute / 10, (Vector2){x, start_pos.y});
     x += LENGTH + OFFSET * 2;
    draw_digit(minute % 10,  (Vector2){x, start_pos.y});
    x += LENGTH + OFFSET;
    draw_colon((Vector2) {x , start_pos.y});
    x += LENGTH + OFFSET;
    draw_digit(second / 10, (Vector2) {x , start_pos.y});
    x += LENGTH + OFFSET * 2;
    draw_digit(second % 10, (Vector2) {x , start_pos.y});
}

int main() {
    time_t current;
    struct tm *current_localtime;
    
    InitWindow(W_WIDTH, W_HEIGHT, "DIGITAL CLOCK");
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
