#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>
#include <unistd.h>
#include "libs/linked_list.h"

#define WIN_H 400
#define WIN_W 600
#define FPS 20
#define TITLE "My text editor."


char *filename;


bool parse_file(const char *filename, DoublyLinkedList *buffer) {
    if (!buffer) return false;

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Failed to open file");
        return false;
    }

    char line[1024]; // temporary buffer for each line
    while (fgets(line, sizeof(line), fp)) {
        // Remove newline character
        line[strcspn(line, "\n")] = 0;
        append_line(buffer, line);
    }

    fclose(fp);
    return true;
}

int check_file(const char *filename, DoublyLinkedList *buffer) {
    if (access(filename, F_OK) == 0) {
        return parse_file(filename, buffer) ? EXIT_SUCCESS : EXIT_FAILURE;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("Failed to create file");
        return EXIT_FAILURE;
    }
    fclose(fp); // close the newly created file

    return parse_file(filename, buffer) ? EXIT_SUCCESS : EXIT_FAILURE;
}


void print_buffer(DoublyLinkedList *buffer) {
    printf("\n\n\n========\n\n");
    for (Line *cur = buffer->head; cur; cur = cur->next) {
        printf("%s\n", cur->text);
    }
    
    printf("\n\n\n========\n\n");
    
}

int main(int argc, char *argv[]){
    if(argc != 2) {
        printf("USAGE: %s <file-to-edit>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
    
    char *filename = argv[1];
    DoublyLinkedList *buffer = create_list();
    if (!buffer) {
        fprintf(stderr, "Failed to create text buffer.\n");
        exit(EXIT_FAILURE);
    }

    if (check_file(filename, buffer) != EXIT_SUCCESS) {
        fprintf(stderr, "Error loading file: %s\n", filename);
        destroy_list(buffer);
        exit(EXIT_FAILURE);
    }
    
    print_buffer(buffer);
    
    return 0;
    
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WIN_W, WIN_H, TITLE);
    SetTargetFPS(FPS);
    
    while(!WindowShouldClose()) {
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);
            DrawText("Hello Workd!", 20, 20, 20, BLACK);
        }
        EndDrawing();
    }
    
    CloseWindow();
    destroy_list(buffer);
    return EXIT_SUCCESS;
    
}
