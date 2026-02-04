#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "./types.h"

int game_is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int last_frame_time = 0;
struct Ball{
    float x;
    float y;
    float width;
    float height;
};

struct Ball ball;

int initialize_window(void)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr, "ERROR initializing SDL.\n");
        return false;
    }

    window = SDL_CreateWindow(
        NULL,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        // SDL_WINDOW_BORDERLESS
        SDL_WINDOW_RESIZABLE
    );

    if (!window)
    {
        fprintf(stderr, "ERROR creating SDL window.\n");
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (!renderer)
    {
        fprintf(stderr, "Error creating SDL renderer.\n");
        return false;
    }

    return true;
}

void process_input(void)
{
    SDL_Event event;
    SDL_PollEvent(&event);

    switch (event.type)
    {
    case SDL_QUIT:
        game_is_running = false;
        break;

    case SDL_KEYDOWN:
        if (event.key.keysym.sym == SDLK_ESCAPE)
            game_is_running = false;
        break;

    default:
        break;
    }
}


void setup(void) {
    ball.x = 20;
    ball.y = 20;
    ball.width = 15;
    ball.height = 15;
}

void update(void)
{
    while(! SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME));

    last_frame_time = SDL_GetTicks();

    ball.x += 2;
    ball.y += 2;
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0 ,255);
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect ball_rect = {
        (int) ball.x, 
        (int) ball.y, 
        (int) ball.width, 
        (int) ball.height
    };

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_RenderFillRect(renderer, &ball_rect);

    SDL_RenderPresent(renderer);

    // SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
    // SDL_RenderClear(renderer);
    // SDL_RenderPresent(renderer);
}


void destroy_window(void)
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main()
{
    game_is_running = initialize_window();

    setup();
    while (game_is_running)
    {
        process_input();
        update();
        render();
    }

    destroy_window();
    return true;
}
