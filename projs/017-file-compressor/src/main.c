#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

char *file_extension = ".compressed";
int main(int argc, char *argv[]){
    if(argc != 2) {
        printf("USAGE: %s <file to compress>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    // input file
    char *original_filename = argv[1];
    FILE *original_fp = fopen(original_filename, "r");
    if(original_fp == NULL) {
        perror("Failed to open file to compress:");
        exit(EXIT_FAILURE);
    }

    // output file
    int len = strlen(original_filename) + strlen(file_extension) + 1;
    char op_filename[len];
    snprintf(op_filename, len, "%s%s", original_filename, file_extension);

    printf("output filename %s\n", op_filename);
    // FILE *compressed_fp = fopen

    // InitWindow(900, 600, "File Compressor");
    // while (!WindowShouldClose())
    // {
    //     BeginDrawing();
    //     ClearBackground(RAYWHITE);
    //     DrawText("Hello Raylib", 190, 200, 19, LIGHTGRAY);
    //     EndDrawing();
    // }
    
    // CloseWindow();

    return EXIT_SUCCESS;
}