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
#define TIP 10
#define OFFSET 25

#define DARKESTGRAY CLITERAL(Color){ 20, 20, 20, 255 }  // Dark gray
#define ON_COLOR GREEN
#define OFF_COLOR DARKESTGRAY

// top_left, top, top_right, bottom_right, bottom, bottom_left, middle
extern int numbers_lookup_table[10][7] ;


extern const Vector2 start_pos ;

void paint_triangle_strip(Vector2 center, bool vertical, Color color);
void draw_colon(Vector2 center) ;
void draw_digit(int digit, Vector2 center);
void draw_clock(int hour, int minute, int second);


#endif // DRAW_H
