#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>


int main() 
{
    printf("hello world \n");
    FILE *in = stdin;
    int width = 900;
    int height = 600;
    char ppmformat[3];
    char line[1000];

    // Read first line (specifier p3 or p6)
    fgets(line, sizeof(line), in);
    sscanf(line, "%2s", ppmformat);


    char pdimensions[1000];
    // read 2nd line  (dimensions)

    if( fgets(pdimensions, 1000, in) == NULL){
        perror("error reading dimension\n");
        return EXIT_FAILURE;
    }

    if(pdimensions[0] == '#') {
        memset(pdimensions, 0, sizeof(pdimensions));
        fgets(pdimensions, 1000, in);
    }

    // Must return 2 since 2 arguments are returned from sscanf.
    if(sscanf(pdimensions, "%d %d", &width, &height) !=2 ) {
        perror("Error reading dimensions\n");
        return EXIT_FAILURE;
    }
  

    char pthrowable[1000] ;
    // read 3nd line  (max color)
    fgets(pthrowable, 1000, in);


    SDL_Window *pwindow = SDL_CreateWindow("Image Viewer",  SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, 0);
    SDL_Surface *psurface =  SDL_GetWindowSurface(pwindow); 
    
    int x, y;
    x = y = 50;
    SDL_Rect pixel = (SDL_Rect) {x, y, 1, 1};
    Uint32 color =  0;

   
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            Uint8 r, g, b;

            if(ppmformat[1] == '6') 
            {
                r = (char) getchar();
                g = (char) getchar();
                b = (char) getchar();
            } else {
                int ir, ig, ib;
                fscanf(in, "%d %d %d", &ir, &ig, &ib);
                r = (Uint8)ir;
                g = (Uint8)ig;
                b = (Uint8)ib;

            }

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