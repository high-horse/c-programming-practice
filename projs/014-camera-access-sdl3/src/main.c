#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "constants.h"

typedef struct
{
    bool running;

    int window_width;
    int window_height;

    SDL_Window *window;
    SDL_Renderer *renderer;
} App;

App app;
bool error;
bool show_red = true;
Uint64 last_toggle;
SDL_FRect rect;

bool initialize(void)
{
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        perror("ERROR ");
        fprintf(stderr, "ERROR INITIALIZING SDL: %s\n", SDL_GetError());
        return false;
    }
    app.window = SDL_CreateWindow(
        WINDOW_TITLE,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_RESIZABLE);

    if (!app.window)
    {
        fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
        return false;
    }

    app.renderer = SDL_CreateRenderer(app.window, NULL);
    if (!app.renderer)
    {
        fprintf(stderr, "error creating renderer: %s \n", SDL_GetError());
        return false;
    }

    app.running = true;
    return true;
}

void setup()
{
    rect.h = 20;
    rect.w = 20;
    rect.x = 120;
    rect.y = 120;
}

void render(void)
{

    if (show_red)
    {
        SDL_SetRenderDrawColor(app.renderer, 255, 0, 0, 255);
    }
    else
    {
        SDL_SetRenderDrawColor(app.renderer, 0, 255, 0, 255);
    }
    SDL_RenderClear(app.renderer);

    SDL_SetRenderDrawColor(app.renderer, 0, 0, 255, 255);
    SDL_RenderFillRect(app.renderer, &rect);
    // SDL_RenderClear(app.renderer);
    SDL_RenderPresent(app.renderer);
}

void update(void)
{
    Uint64 now_time = SDL_GetTicks();
    if (now_time - last_toggle >= 1000)
    {
        show_red = !show_red;
        last_toggle = now_time;
        rect.x += 25;
        rect.y += 25;
    }
}

bool handle_event()
{

    SDL_Event event;
    while (SDL_PollEvent(&event) > 0)
    {
        switch (event.type)
        {
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            printf("Mouse btn down encountered\n");
            break;

        case SDL_EVENT_QUIT:
            printf("closing...\n");
            app.running = false;
            break;

        case SDL_EVENT_KEY_DOWN:
            if(event.key.key == SDLK_ESCAPE) {
                printf("closing...\n");
                app.running = false;
            }

        default:
            break;
        }
    }

    return true;
}

void destroy(void)
{
    app.running = false;
    SDL_DestroyRenderer(app.renderer);
    SDL_DestroyWindow(app.window);
    SDL_Quit();
}

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    printf("Starting ... \n");
    if (!initialize())
    {
        return EXIT_FAILURE;
    }
    setup();
    while (app.running)
    {
        handle_event();

        update();
        render();
    }
    destroy();
    // SDL_Delay(2000);
    return EXIT_SUCCESS;
}