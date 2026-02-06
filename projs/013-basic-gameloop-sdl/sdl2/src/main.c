#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "./types.h"

int game_is_running = false;
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

int last_frame_time = 0;
typedef struct
{
    float x;
    float y;
    float width;
    float height;
} Ball;

Ball ball;

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
         SDL_WINDOW_BORDERLESS
        //SDL_WINDOW_RESIZABLE
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

void setup(void)
{

	int x, y;
	SDL_GetWindowPosition(window, &x, &y);
printf("position aquired x=>%d y=>%d\n", x, y);
    // ball.x = 20;
    // ball.y = 20;
ball.x = x;
ball.y = y;
    ball.width = 15;
    ball.height = 15;
}

void old_update(void)
{
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), last_frame_time + FRAME_TARGET_TIME))
        ;

    // get delta time factor conversion to second to be used to update objects later
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();

    ball.x += 60 * delta_time;
    ball.y += 50 * delta_time; 
}

void update(void)
{

    // int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - last_frame_time);

    // if(time_to_wait > 0  && time_to_wait <= FRAME_TARGET_TIME) {
    //     SDL_Delay(time_to_wait);
    // }
    
    // get delta time factor conversion to second to be used to update objects later
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f;
    last_frame_time = SDL_GetTicks();

    // ball.x += 50 * delta_time;
    // ball.y += 60 * delta_time;
}

void render(void)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Draw a rectangle
    SDL_Rect ball_rect = {
        (int)ball.x,
        (int)ball.y,
        (int)ball.width,
        (int)ball.height};

    SDL_SetRenderDrawColor(renderer, 255, 255, 0, 255);

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
