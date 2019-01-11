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

int n; // the dimensions of game will be 2n * 3n
Sint16 bricks[10000]; //array of bricks


void play_game() {
    while (state == 1 && flag) {
        while (state == 1) {
            int start_ticks = SDL_GetTicks(); // stores the starting time of each frame.
            if (events() == -1) {
                flag = 0; //close the window if closing button pressed
                break;
            }
            make_shots(); // it makes shots if necessary.
            if (!gun.shots_in_screen && !save_mode){ // motion of gun if there's not any shots in the screen.
                gun_motion();
                gun_rotation();
            }
            shot_motion(); // motion of shots if they are in the screen.
            drawing(); //draw everything at every frame.
            while (SDL_GetTicks() - start_ticks < 1000 / FPS); //making a delay every frame.
        }
    }
}

int main(int argc, char *argv[]) {
    state = 0; // this value is to show the starting menu at first
    show_window(); // appearing the game window
    show_starting_menu(); // showing the starting menu
    play_game(); // all happenings of the game are here
    game_over(); // shows the game over window at the end of the game
    Quit(); // destroying the window
    return 0;
}