#ifndef DRAW_H
#define DRAW_H

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
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
const int numbers_lookup_table[10][7] = {
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


const Vector2 start_pos = {LENGTH+ OFFSET, W_HEIGHT / 2};

void paint_triangle_strip(Vector2 center, bool vertical, Color color);
void draw_colon(Vector2 center) ;
void draw_digit(int digit, Vector2 center);
void draw_clock(int hour, int minute, int second);


#endif // DRAW_H