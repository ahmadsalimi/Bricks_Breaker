#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "View.h"
#include "Physics.h"

SDL_Window *window;
SDL_Renderer *renderer;

void show_window() {
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void show_starting_menu() {
    while (state == 0) {
        if (menu_events() == -1) {
            flag = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }
}

void draw_borders() {
    thickLineRGBA(renderer, START_X, START_Y - 1, START_X, FINISH_Y + 1, 2, 0, 0, 0, 255);
    thickLineRGBA(renderer, FINISH_X, START_Y - 1, FINISH_X, FINISH_Y + 1, 2, 0, 0, 0, 255);
    thickLineRGBA(renderer, START_X - 1, START_Y, FINISH_X + 1, START_Y, 2, 0, 0, 0, 255);
    thickLineRGBA(renderer, START_X - 1, FINISH_Y, FINISH_X + 1, FINISH_Y, 2, 0, 0, 0, 255);
}

void make_bricks() {
    int num = 1 + rand() % (2 * n - 1), counter = 0; //the number of bricks in each new line
    for (int i = 3 * n - 1; i > 0; i--) {
        for (int j = 0; j < 2 * n; j++) {
            bricks[j + (2 * n) * i] = bricks[j + (2 * n) * (i - 1)]; //moving the bricks down a row
        }
    }
    for (int i = 0; i < 2 * n; i++){
        bricks[i].r = 0;
    }
    while (counter < num) {
        int i = rand() % (2 * n);
        int max_r = n_shots ? n_shots : 1;
        if (!bricks[i].r){
            bricks[i].r = (Sint16) (1 + rand() % (max_r)); //the random resistance of each brick
            counter++;
        }
    }
}

void draw_bricks() {
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 2 * n; j++) {
            if (bricks[j + (2 * n) * i].r) {
                thickLineRGBA(renderer, (Sint16) (START_X + j * BRICK_SIZE + 1), (Sint16) (START_Y + i * BRICK_SIZE + BRICK_SIZE / 2), (Sint16) (START_X + j * BRICK_SIZE + BRICK_SIZE - 1), (Sint16) (START_Y + i * BRICK_SIZE + BRICK_SIZE / 2), (Uint8) (BRICK_SIZE - 2), 255, 0, 0, (Uint8) (255 * ((double) bricks[j + (2 * n) * i].r / (gun.score + 1))));
                char resistance[3];
                sprintf(resistance, "%d", bricks[j + (2 * n) * i].r);
                stringRGBA(renderer, (Sint16) (START_X + BRICK_SIZE * j + BRICK_SIZE / 2 - 2), (Sint16) (START_Y + BRICK_SIZE * i + BRICK_SIZE / 2 - 2), resistance, (Uint8) (bricks[j + (2 * n) * i].r * 2 > gun.score ? 255 : 0),  (Uint8) (bricks[j + (2 * n) * i].r * 2 > gun.score ? 255 : 0),  (Uint8) (bricks[j + (2 * n) * i].r * 2 > gun.score ? 255 : 0), 255);
            }
        }
    }
}

void draw_gun() {
    if (!gun.shots_in_screen) {
        lineRGBA(renderer, gun.x, gun.y, x2, y2, 0, 0, 0, 120);
    }
    filledCircleRGBA(renderer, gun.x, gun.y, GUN_RADIUS, 0, 171, 239, 255);
}

void draw_shots() {
    for (int i = 0; i < n_shots; i++) {
        if (shot[i].life) {
            filledCircleRGBA(renderer, (Sint16) shot[i].x, (Sint16) shot[i].y, SHOT_RADIUS - 4, 0, 0, 0, 255);
        }
    }
}

void drawing() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderClear(renderer);
    draw_bricks();
    draw_borders();
    draw_gun();
    draw_shots();
    SDL_RenderPresent(renderer);
}

void Quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
