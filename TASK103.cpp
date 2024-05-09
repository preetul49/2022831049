#include <SDL2/SDL.h>
#include <iostream>
#include <cmath>

using namespace std;

int main(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_Window *window = SDL_CreateWindow("Collision", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 500, 500, SDL_WINDOW_RESIZABLE);
    if (window == NULL) { cout << "Error: " << SDL_GetError() << endl; }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) { cout << "Error: " << SDL_GetError() << endl; SDL_DestroyWindow(window); SDL_Quit(); }

    int c1x = 50; int c1y = 250; int c1r = 50;
    int c2x = 250; int c2y = 50; int c2r = 50;
    SDL_Event event;
    bool game = true;

    while (game) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) { game = false; break; }
            else if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP: if (c2y - 10 >= c2r) { c2y -= 10; } break;
                    case SDLK_DOWN: if (c2y + 10 <= 500 - c2r) { c2y += 10; } break;
                    case SDLK_LEFT: if (c2x - 10 >= c2r) { c2x -= 10; } break;
                    case SDLK_RIGHT: if (c2x + 10 <= 500 - c2r) { c2x += 10; } break;
                    default: break;
                }
            }
        }

        c1x += 2;
        if (c1x - c1r > 500) { c1x = c1r; }
        int dx = c1x - c2x;
        int dy = c1y - c2y;
        int distance = sqrt(dx * dx + dy * dy);
        int md = c1r + c2r;
        int r = (distance <= md) ? 0 : 255;
        int k = (distance <= md) ? 0 : 255;

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black color for background
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Red color for circle
        for (int y = -c1r; y <= c1r; y++) {
            for (int x = -c1r; x <= c1r; x++) {
                if (x * x + y * y <= c1r * c1r) { SDL_RenderDrawPoint(renderer, c1x + x, c1y + y); }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, k, 0, 255);
        for (int y = -c2r; y <= c2r; y++) {
            for (int x = -c2r; x <= c2r; x++) {
                if (x * x + y * y <= c2r * c2r) { SDL_RenderDrawPoint(renderer, c2x + x, c2y + y); }
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
