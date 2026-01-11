#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    const int width = 900;
    const int height = 600;
    const char *ptitle = "Image Viewer";

    // SDL_Window *pwindow =  SDL_CreateWindow(ptitle, width, height, 0);

    SDL_Window  *pwindow = SDL_CreateWindow(ptitle, SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED, width, height, 0) ;
    SDL_Renderer *prenderer =  SDL_CreateRenderer(pwindow, -1, 0);
    if(!pwindow || !prenderer)
    {
        SDL_Log("Failed to create window or renderer");
        return EXIT_FAILURE;
    }

    // SDL_Rect *fillRect = { 50, 50, 100, 100 };
    // const SDL_Rect *prect = {0, 0 , width, height}
    // int SDL_RenderDrawRect(prenderer, fillRect);

    // SDL_SetRenderDrawColor(prenderer, 255, 255, 255, 255);
    // SDL_RenderClear(prenderer); // Fills the entire rendering target with white
    
    // 2. Set the color for the rectangle (e.g., to blue)
    SDL_SetRenderDrawColor(prenderer, 255, 0, 255, 255);
    
    // 3. Define the rectangle's position and size
    SDL_Rect blueRect = { 150, 150, 10, 10 };
    
    // 4. Draw the filled blue rectangle
    SDL_RenderFillRect(prenderer, &blueRect);
    
    // 5. Present the final rendered image to the window
    SDL_RenderPresent(prenderer);
    

    SDL_Event event;
    while(SDL_WaitEvent(&event))
        if(event.type == SDL_QUIT) break;

    SDL_DestroyWindow(pwindow);
    SDL_Quit();
    
    return EXIT_SUCCESS;
}
