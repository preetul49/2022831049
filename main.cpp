#define SDL_MAIN_HANDLED  // Prevent SDL from redefining main
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define BLOCK_SIZE 20

struct Point {
    int x, y;
};

struct Snake {
    Point body[100];
    int length;
    Point direction;
};

void init_snake(Snake& snake) {
    snake.length = 5;
    for (int i = 0; i < snake.length; ++i) {
        snake.body[i].x = SCREEN_WIDTH / 2 - i * BLOCK_SIZE;
        snake.body[i].y = SCREEN_HEIGHT / 2;
    }
    snake.direction = {BLOCK_SIZE, 0};  // moves right at first
}

void move_snake(Snake& snake) {
    for (int i = snake.length - 1; i > 0; --i) {
        snake.body[i] = snake.body[i - 1];
    }
    snake.body[0].x += snake.direction.x;
    snake.body[0].y += snake.direction.y;
}

bool check_collision(const Snake& snake) {
    Point head = snake.body[0];
    // wall
    if (head.x < 0 || head.x >= SCREEN_WIDTH || head.y < 0 || head.y >= SCREEN_HEIGHT) {
        return true;
    }
    // self
    for (int i = 1; i < snake.length; ++i) {
        if (head.x == snake.body[i].x && head.y == snake.body[i].y) {
            return true;
        }
    }
    return false;
}

bool check_food_collision(const Snake& snake, const Point& food) {
    return (snake.body[0].x == food.x && snake.body[0].y == food.y);
}

void spawn_food(Point& food) {
    food.x = (rand() % (SCREEN_WIDTH / BLOCK_SIZE)) * BLOCK_SIZE;
    food.y = (rand() % (SCREEN_HEIGHT / BLOCK_SIZE)) * BLOCK_SIZE;
}

void grow_snake(Snake& snake) {
    snake.body[snake.length] = snake.body[snake.length - 1];
    ++snake.length;
}

void render_text(SDL_Renderer* renderer, TTF_Font* font, const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect dest = {x, y, surface->w, surface->h};
    SDL_RenderCopy(renderer, texture, nullptr, &dest);
    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void render_game_over(SDL_Renderer* renderer, TTF_Font* font, int score) {
    SDL_Color white = {255, 255, 255, 255};
    SDL_Color red = {255, 0, 0, 255};

    // "Game Over" dekhano
    render_text(renderer, font, "GAME OVER", SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2 - 50, red);

    // Last er score
    render_text(renderer, font, "Score: " + std::to_string(score), SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, white);

    SDL_RenderPresent(renderer);
}

int main() {
    // SDL2 and TTF
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }
    if (TTF_Init() == -1) {
        std::cerr << "TTF could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Window open
    SDL_Window* window = SDL_CreateWindow(
        "Snake Game with Scoring",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH, SCREEN_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // creating renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // font load
    TTF_Font* font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        TTF_Quit();
        SDL_Quit();
        return 1;
    }

    // game elements initialize
    Snake snake;
    Point food;
    int score = 0;
    SDL_Color white = {255, 255, 255, 255};
    bool running = true;
    SDL_Event event;
    srand(static_cast<unsigned int>(time(nullptr)));

    init_snake(snake);
    spawn_food(food);

    // main game loop
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_UP:
                        if (snake.direction.y == 0) {
                            snake.direction = {0, -BLOCK_SIZE};
                        }
                        break;
                    case SDLK_DOWN:
                        if (snake.direction.y == 0) {
                            snake.direction = {0, BLOCK_SIZE};
                        }
                        break;
                    case SDLK_LEFT:
                        if (snake.direction.x == 0) {
                            snake.direction = {-BLOCK_SIZE, 0};
                        }
                        break;
                    case SDLK_RIGHT:
                        if (snake.direction.x == 0) {
                            snake.direction = {BLOCK_SIZE, 0};
                        }
                        break;
                }
            }
        }

        move_snake(snake);

        // collision checking
        if (check_collision(snake)) {
            std::cout << "Game Over! Final Score: " << score << std::endl;
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderClear(renderer); 
            render_game_over(renderer, font, score);
            SDL_Delay(3000);  // "Game Over" pop up for 3 seconds
            break; 
        }

        // food collision checking
        if (check_food_collision(snake, food)) {
            grow_snake(snake);
            score += 10;
            spawn_food(food);
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);  // background colour (black)
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);  // snake colour (green)
        for (int i = 0; i < snake.length; ++i) {
            SDL_Rect block = {snake.body[i].x, snake.body[i].y, BLOCK_SIZE, BLOCK_SIZE};
            SDL_RenderFillRect(renderer, &block);
        }

        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);  // food colour (red)
        SDL_Rect food_rect = {food.x, food.y, BLOCK_SIZE, BLOCK_SIZE};
        SDL_RenderFillRect(renderer, &food_rect);

        render_text(renderer, font, "Score: " + std::to_string(score), 10, 10, white);

        SDL_RenderPresent(renderer);

        SDL_Delay(100);
    }

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
