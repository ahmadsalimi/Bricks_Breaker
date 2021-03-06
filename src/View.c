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

FILE *storage;
int last_number;
int load_number = 1;
bool file_checked = 0;
Sint8 menu_state = 0; // 0(starting), 1(new game), 2(load a game)

void show_window() { // The following functions make the window of the game
    SDL_Init(SDL_INIT_VIDEO);
    window = SDL_CreateWindow("Brick Breaker", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
}

void check_storage() {
    storage = fopen("../storage.save", "r"); // open the saving file.
    char line[10000];
    last_number = 1;
    fgets(line, 10000, storage);
    while (1) {
        int num = 0;
        fscanf(storage, "%d", &num);
        if (num){
            fgets(line, 10000, storage);
        } else {
            break;
        }
        last_number++;
    }
    file_checked = 1;
    fclose(storage); //close the file.
}

void load(){
    srand((unsigned int) time(NULL));
    storage = fopen("../storage.save", "r"); // open the saving file.
    char line[10000];
    fgets(line, 10000, storage); // get the first line of file: "Storage File"
    int num = 0;
    while (num != load_number){
        fscanf(storage, "%d", &num); // get the number of save.
        if (num == load_number) {
            fscanf(storage, "%d %hi %hi %hi %hi ", &n, &(gun.score), &(gun.x), &(gun.y), &(gun.angle)); // scan the information of saved game
            for (int i = 0; i < 6 * n * n; i++) {
                fscanf(storage, "%hi ", &bricks[i]); // scan the information of bricks
            }
            break;
        }
        if (num) {
            fgets(line, 10000, storage);
        } else {
            break;
        }
    }
    fclose(storage); // close the file.
    file_checked = 0;
}

void show_starting_menu() { // This function shows starting menu of the game. menu_state 0 means it's in the first state, menu_state 1 means player wants to start a new game, menu_state 2 means player wants to load a saved game.
    n = 1;
    while (state == 0) {
        if (menu_events() == -1) { // this line calls menu_events function in physics.c file. if returned value is -1, it means close button pressed.
            flag = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // background color of window
        SDL_RenderClear(renderer); // In each frame, the contents of the window should be cleared and re-drawn in the following.
        if (menu_state == 0) {
            stringRGBA(renderer,/*x*/ 50 ,/*y*/ SCREEN_HEIGHT / 2, "If you want to start a new game press \"ENTER\" and if you want to load a game press \"L\".",/*RGBA color*/ 0, 0, 0, 255); // this function shows a string in the window.
        } else if (menu_state == 1) { // New game
            char string[50];
            sprintf(string, "Please set the n value by pressing the keys up or down. n = %d", n); // the value of n will be changed by menu_events function that is called in the beginning of this function.
            stringRGBA(renderer, 150, SCREEN_HEIGHT / 2, string, 0, 0, 0, 255);
        } else if (menu_state == 2) { // Load a game
            check_storage(); // this function checks the saving storage to know how many games are saved in the file.
            if (!(last_number - 1)){ // there is no saved game!
                stringRGBA(renderer, 170, SCREEN_HEIGHT / 2, "There is no saved game. Press \"ESC\" to start a new game!", 0, 0, 0, 255);
            } else {
                char string[50];
                sprintf(string, "Please Tell us which saved game do you want to play: %d", load_number); // load_number will be changed by menu_events function that is called in the beginning of this function.
                stringRGBA(renderer, 45, SCREEN_HEIGHT / 2, string, 0, 0, 0, 255);
                stringRGBA(renderer, 45, SCREEN_HEIGHT / 2 + 15, "change the number using the keys up or down. Press \"ENTER\" to load.", 0, 0, 0, 255);
            }
        }
        SDL_RenderPresent(renderer); // this function presents the new drawn items.
    }
}

void draw_borders() {
    thickLineRGBA(renderer, START_X, START_Y - 1, START_X, FINISH_Y + 1, 2, 0, 0, 0, 255);
    thickLineRGBA(renderer, FINISH_X, START_Y - 1, FINISH_X, FINISH_Y + 1, 2, 0, 0, 0, 255);
    thickLineRGBA(renderer, START_X - 1, START_Y, FINISH_X + 1, START_Y, 2, 0, 0, 0, 255);
    thickLineRGBA(renderer, START_X - 1, FINISH_Y, FINISH_X + 1, FINISH_Y, 2, 0, 0, 0, 255);
}

void make_bricks() {
    for (int i = 0; i < 2 * n; i++) { // checking if game is over or not
        if (bricks[i + (2 * n) * (3 * n - 2)] > 0 && bricks[i + (2 * n) * (3 * n - 2)] != BOMB) { // checking the finish of the game
            state = 2; // game over
            break;
        }
    }
    int num = 1 + rand() % (2 * n - 1), counter = 0; //the number of bricks in each new line
    double g = (double) (rand() % 1000) / 1000; // a random for making bomb
    for (int i = 3 * n - 1; i > 0; i--) {
        for (int j = 0; j < 2 * n; j++) {
            bricks[j + (2 * n) * i] = bricks[j + (2 * n) * (i - 1)]; //moving the bricks down a row
        }
    }
    for (int i = 0; i < 2 * n; i++) {
        bricks[i] = 0; //empty first row
    }
    while (counter < num) {
        int i = rand() % (2 * n);
        if (!bricks[i]) {
            bricks[i] = (Sint16) (1 + rand() % (n_shots)); //the random resistance of each brick
            counter++;
        }
    }
    if (g + (double) num / (2 * n) > 1) {
        int r = rand() % (2 * n - num); // num is the number of bricks.
        for (int i = 0, j = 0;; i++) {
            if (j == r && bricks[i] == 0) {
                bricks[i] = BOMB; // it is a bomb
                break;
            }
            if (bricks[i] == 0) {
                j++;
            }
        }
    }
}

void draw_bricks() {
    for (int i = 0; i < 3 * n; i++) {
        for (int j = 0; j < 2 * n; j++) {
            int a = i, b = j;
            if (bricks[j + (2 * n) * i] > 0 && bricks[j + (2 * n) * i] != BOMB) { // if it's a brick and it's not a bomb!
                //draw the Brick
                thickLineRGBA(renderer, (Sint16) (START_X + j * BRICK_SIZE + 1), (Sint16) (START_Y + i * BRICK_SIZE + BRICK_SIZE / 2), (Sint16) (START_X + j * BRICK_SIZE + BRICK_SIZE - 1), (Sint16) (START_Y + i * BRICK_SIZE + BRICK_SIZE / 2), (Uint8) (BRICK_SIZE - 2), 255, 0, 0, (Uint8) (255 * ((double) bricks[j + (2 * n) * i] / (gun.score ? gun.score : 1)))); // transparency depends on the ratio of resistance to the score. syntax: renderer, start x, start y, finish x, finish y, thickness, RGBA.
                char resistance[3];
                sprintf(resistance, "%d", bricks[b + (2 * n) * a]);
                // show the number of resistance in the brick
                stringRGBA(renderer, (Sint16) (START_X + BRICK_SIZE * j + BRICK_SIZE / 2 - 2), (Sint16) (START_Y + BRICK_SIZE * i + BRICK_SIZE / 2 - 2), resistance, (Uint8) (bricks[j + (2 * n) * i] * 2 > gun.score ? 255 : 0), (Uint8) (bricks[j + (2 * n) * i] * 2 > gun.score ? 255 : 0) /*color depends on ratio of resistance to the score.*/, (Uint8) (bricks[j + (2 * n) * i] * 2 > gun.score ? 255 : 0), 255);
            } else if (bricks[j + (2 * n) * i] == BOMB) { // it's a bomb!
                // draw the bomb
                thickLineRGBA(renderer, (Sint16) (START_X + j * BRICK_SIZE + 1), (Sint16) (START_Y + i * BRICK_SIZE + BRICK_SIZE / 2), (Sint16) (START_X + j * BRICK_SIZE + BRICK_SIZE - 1), (Sint16) (START_Y + i * BRICK_SIZE + BRICK_SIZE / 2), (Uint8) (BRICK_SIZE - 2), 0, 176, 188, 255);
                // write "BOMB" label
                stringRGBA(renderer, (Sint16) (START_X + BRICK_SIZE * j + BRICK_SIZE / 5), (Sint16) (START_Y + BRICK_SIZE * i + BRICK_SIZE / 2), "BOMB!!", 255, 255, 255, 255);
            }
        }
    }
}

void draw_gun() {
    if (!gun.shots_in_screen) {
        lineRGBA(renderer, gun.x, gun.y, x2, y2, 0, 0, 0, 120); // draw the help line if there's not any shots in the screen.
    }
    filledCircleRGBA(renderer, gun.x, gun.y, GUN_RADIUS, 0, 171, 239, 255); // draw a circle as a gun
}

void draw_shots() {
    for (int i = 0; i < n_shots; i++) {
        if (shot[i].life) {
            filledCircleRGBA(renderer, (Sint16) shot[i].x, (Sint16) shot[i].y, SHOT_RADIUS - 4, 0, 0, 0, 255); // draw shots if exist in the screen.
        }
    }
}

void view_scores() {
    char scoreboard[20];
    sprintf(scoreboard, "Score: %d", gun.score);
    stringRGBA(renderer, (Sint16) (FINISH_X + 10), START_Y + 10, scoreboard, 0, 0, 0, 255); // write the score at the right side of the game box.
}

void save() {
    storage = fopen("../storage.save", "a"); // open the save file.
    fprintf(storage, "\n%d %d %d %d %d %d ", last_number, n, gun.score, gun.x, gun.y, gun.angle); // print the information of the game in the file.
    for (int i = 0; i < 6 * n * n; i++){
        fprintf(storage, "%d ", bricks[i]); // print the information of bricks in the file.
    }
    save_mode = 0; // close the save mode.
    last_number++;
    fclose(storage); // close the file.
}

void view_save_tool() {
    if (!save_mode) { // playing the game, so save mode is off.
        stringRGBA(renderer, (Sint16) (FINISH_X + 10), START_Y + 70, "If you want to save this game,", 0, 0, 0, 255);
        stringRGBA(renderer, (Sint16) (FINISH_X + 10), START_Y + 85, "press \"S\".", 0, 0, 0, 255);
    } else if (save_mode) { //save mode
        if (!file_checked) {
            check_storage(); // check the save storage to know the number of new save.
        }
        stringRGBA(renderer, (Sint16) (FINISH_X + 10), START_Y + 70, "Press \"ESC\" to back to the game.", 0, 0, 0, 255);
        stringRGBA(renderer, (Sint16) (FINISH_X + 10), START_Y + 85, "Press \"ENTER\" to save this game.", 0, 0, 0, 255);
        char number[20];
        sprintf(number, "The number of this saving will be %d", last_number);
        stringRGBA(renderer, (Sint16) (FINISH_X + 10), START_Y + 100, number, 0, 0, 0, 255);
    }
}

void drawing() {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Background color of window.
    SDL_RenderClear(renderer); // In each frame, the contents of the window should be cleared and re-drawn in the following.
    draw_bricks();
    draw_borders(); //borders of the game.
    draw_gun();
    draw_shots();
    view_scores();
    view_save_tool();
    SDL_RenderPresent(renderer); // this function presents the new drawn items.
}

void game_over() {
    while (state == 2) {
        if (events() == -1) {
            flag = 0;
            break;
        }
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Background color of window.
        SDL_RenderClear(renderer); // In each frame, the contents of the window should be cleared and re-drawn in the following.
        char Score[20];
        sprintf(Score, "Game Over. Your score: %d", gun.score); //show the score.
        stringRGBA(renderer, SCREEN_WIDTH / 2 - 100, SCREEN_HEIGHT / 2, Score, 0, 0, 0, 255);
        SDL_RenderPresent(renderer); // this function presents the new drawn items.
    }
}


void Quit() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}
