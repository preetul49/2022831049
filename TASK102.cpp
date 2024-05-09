#include<SDL2/SDL.h>
#include<iostream>
#include<cmath>

using namespace std;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window; window = SDL_CreateWindow("Expand Circle",SDL_WINDOWPOS_UNDEFINED,SDL_WINDOWPOS_UNDEFINED,500,500,SDL_WINDOW_RESIZABLE);
    if(window==NULL) {
         cout<<"Error:"<<endl<<SDL_GetError()<<endl; }
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
         cout << "Error: " << SDL_GetError() << endl; 
         SDL_DestroyWindow(window); 
         SDL_Quit(); }
    int radius = 100;
    SDL_Event event;
    bool game = true;
    while(game) {
        while(SDL_PollEvent(&event)) {
            if(event.type== SDL_QUIT) { 
                game=false; break; }
        }
        SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255); // Light gray color for background
        SDL_RenderClear(renderer);
        radius+=2; if(radius>=250) {
             radius=100; }
        for (int y = -radius; y <= radius; y++) { for (int x = -radius; x <= radius; x++) {
             if (x * x + y * y <= radius * radius) { 
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
                SDL_RenderDrawPoint(renderer, 250 + x, 250 + y); } } }
        SDL_RenderPresent(renderer);
    }
    SDL_DestroyRenderer(renderer);
     SDL_DestroyWindow(window);
      SDL_Quit();
       return 0;
}
