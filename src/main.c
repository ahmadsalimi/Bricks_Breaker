#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "Str_Const.h"
#include "View.h"
#include "Physics.h"

#ifdef main
#undef main
#endif /*main*/

int n;
BRICK bricks[1000];
bool make = 1;

void setting() {
    srand((unsigned int) time(NULL));
    //random position for gun:
    gun.x = (Sint16) (START_X + rand() % ((int) FINISH_X - START_X));
    gun.y = FINISH_Y;
    gun.angle = -90;
    gun.score = 0;
    for (int i = 0; i < 6 * n * n; i++){
        bricks[i].r = 0;
    }
}


void play_game() {
    while (state && flag) {
        setting(); //settings before starting the game
        make_bricks();
        while (1) {
            int start_ticks = SDL_GetTicks();
            if (events() == -1) {
                flag = 0; //close the window if closing button pressed
                break;
            }
            drawing(); //draw everything at every frame
            while (SDL_GetTicks() - start_ticks < 1000 / FPS); //making a delay every frame
        }
    }
}

int main(int argc, char *argv[]) {
    state = 0; // this value is to show the starting menu at first
    show_window(); // appearing the game window
    show_starting_menu(); // showing the starting menu
    n = 3;
    play_game(); // all happenings of the game are here
    Quit(); // destroying the window
    return 0;
}