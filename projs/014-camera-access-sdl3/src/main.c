#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define SDL_MAIN_USE_CALLBACLS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include "constants.h"

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;

    int width;
    int height;

    int camera_count;
    SDL_CameraID *devices;
    SDL_Camera *camera;

    SDL_Texture *texture;

    int take_screenshot;
    int screenshot_no;

} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv)
{
    AppState *app_state = malloc(sizeof(AppState));
    *app_state = (AppState){
        .width = WINDOW_WIDTH,
        .height = WINDOW_HEIGHT,
        .take_screenshot = 0,
        .screenshot_no = 0};
    *appstate = app_state;

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CAMERA))
    {
        SDL_Log("could not initialize sdl: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE, app_state->width, app_state->height, 0, &(app_state->window), &(app_state->renderer)))
    {
        SDL_Log("could not initialize window and renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_CameraID *devices = SDL_GetCameras(&app_state->camera_count);
    if (devices == NULL || app_state->camera_count == 0)
    {
        SDL_Log("Cant enumerate available cameras : %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    app_state->devices = devices;
    SDL_Log("Found Camera count %d \n", app_state->camera_count);

    // open 1st camera
    app_state->camera = SDL_OpenCamera(devices[0], NULL);
    if (app_state->camera == NULL)
    {
        SDL_Log("Could not open camera: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    // get camera format
    SDL_CameraSpec spec;
    SDL_GetCameraFormat(app_state->camera, &spec);
    int FPS = spec.framerate_numerator / spec.framerate_denominator;
    SDL_Log("Got camera of %dx%d and FPS %d.", spec.width, spec.height, FPS);

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *app_state = appstate;

    SDL_Surface *frame = SDL_AcquireCameraFrame(app_state->camera, NULL);
    if (frame != NULL)
    {
        SDL_FlipSurface(frame, SDL_FLIP_HORIZONTAL);
        if (app_state->texture == NULL)
        {
            SDL_SetWindowSize(app_state->window, frame->w, frame->h);
            app_state->width = frame->w;
            app_state->height = frame->h;
            app_state->texture = SDL_CreateTexture(app_state->renderer, frame->format, SDL_TEXTUREACCESS_STREAMING, frame->w, frame->h);
        }
        else
        {
            SDL_UpdateTexture(app_state->texture, NULL, frame->pixels, frame->pitch);
        }

        if (app_state->take_screenshot == 1)
        {
            app_state->take_screenshot = 0;
            char fname[100];
            fname[0] = '\0';

            sprintf(fname, "screenshpt-%d.bmp", app_state->screenshot_no);
            // SDL_SaveBMP(frame, fname);
            // app_state->screenshot_no ++;

            SDL_Surface *copy = SDL_ConvertSurface(frame, SDL_PIXELFORMAT_RGBA32);
            if (copy)
            {
                SDL_BlitSurface(frame, NULL, copy, NULL);
                SDL_FlipSurface(copy, SDL_FLIP_HORIZONTAL);
                SDL_SaveBMP(copy, fname);
                SDL_DestroySurface(copy);
            }
        }

        SDL_ReleaseCameraFrame(app_state->camera, frame);
    }

    SDL_SetRenderDrawColorFloat(app_state->renderer, 0.4f, 0.6f, 1.0f, SDL_ALPHA_OPAQUE_FLOAT);
    SDL_RenderClear(app_state->renderer);

    if (app_state->texture != NULL)
    {
        // SDL_RenderTexture(app_state->renderer, app_state->texture, NULL, NULL);
        SDL_RenderTextureRotated(
            app_state->renderer,
            app_state->texture,
            NULL,
            NULL,
            0.0,
            NULL,
            SDL_FLIP_HORIZONTAL);
    }

    SDL_RenderPresent(app_state->renderer);
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *app_state = appstate;

    if (event->type == SDL_EVENT_QUIT || event->key.scancode == SDL_SCANCODE_ESCAPE)
    {
        return SDL_APP_SUCCESS;
    }
    else if (event->type == SDL_EVENT_KEY_DOWN)
    {
        if (event->key.scancode == SDL_SCANCODE_S)
        {
            app_state->take_screenshot = 1;
        }
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    AppState *app_state = appstate;

    if (app_state->devices != NULL)
    {
        free(app_state->devices);
    }

    if (app_state->camera != NULL)
    {
        SDL_CloseCamera(app_state->camera);
    }

    if (app_state->texture != NULL)
    {
        SDL_DestroyTexture(app_state->texture);
    }
    free(app_state);
}

int main(int argc, char **argv)
{
    void *appstate;
    if (SDL_AppInit(&appstate, argc, argv) != SDL_APP_CONTINUE)
    {
        return 1;
    }

    SDL_Event event;
    SDL_AppResult result = SDL_APP_CONTINUE;

    while (result == SDL_APP_CONTINUE)
    {
        while (SDL_PollEvent(&event))
        {
            result = SDL_AppEvent(appstate, &event);
        }
        if (result == SDL_APP_CONTINUE)
        {
            result = SDL_AppIterate(appstate);
        }
    }

    SDL_AppQuit(appstate, result);
    SDL_Quit();
    return 0;
}
