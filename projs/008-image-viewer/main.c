#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


int main() 
{
    printf("hello world \n");
    FILE *in = stdin;
    int width = 900;
    int height = 600;
    
    char *pthrowaway = calloc(1000, sizeof(char));
    // Read first line (specifier p3 or p6)
    fgets(pthrowaway, 1000, in);
    free(pthrowaway);

    char *pdimensions = calloc(1000, sizeof(char));
    // read 2nd line  (dimensions)
    fgets(pdimensions, 1000, in);
    sscanf(pdimensions, "%d %d", &width, &height);
    free(pdimensions);

    char *pthrowable = calloc(1000, sizeof(char));
    // read 2nd line  (max color)
    fgets(pthrowable, 1000, in);
    free(pthrowable);


    SDL_Window *pwindow = SDL_CreateWindow("Image Viewer",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Surface *psurface =  SDL_GetWindowSurface(pwindow); 
    
    int x, y;
    x = y = 50;
    SDL_Rect pixel = (SDL_Rect) {x, y, 1, 1};
    Uint32 color =  0;

    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint8 r, g, b;
            r = (char) getchar();
            g = (char) getchar();
            b = (char) getchar();
            color = SDL_MapRGB(psurface->format, r,  g,  b);
            pixel.x = i;
            pixel.y = j;
            SDL_FillRect(psurface, &pixel, color);
        }
        
    }
    
    SDL_FillRect(psurface, &pixel, color);
    SDL_UpdateWindowSurface(pwindow);
    // SDL_Delay(4000);
    int running = 1;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {  // poll until all events are handled!
            if(event.type == SDL_QUIT) {
                running = 0;
            }
        }
        SDL_Delay(100);
        // update game state, draw the current frame
    }
    return EXIT_SUCCESS;
}