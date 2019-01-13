#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "Str_Const.h"
#include "View.h"
#include "Physics.h"


SDL_Event event; // key events.
int keys[401]; // key flags.
int shot_index = 0; // index of shot.
Sint16 n_shots; // number of shots in each level.
bool shooting_flag;
Sint8 state; //game state; 0(menu), 1(game), 2(game over)
bool flag = 1; // if the value of this flag is 0, it means close button is pressed.
Sint16 x2, y2; // coordinates of endpoint of help line
GUN gun; // gun struct
SHOT shot[1000]; // shot structs
bool save_mode = 0;

double cot(double a) {
    return 1 / tan(a);
}

double absolute(double x) {
    return x > 0 ? x : -x;
}

void help_line_coordinates() { // to find the endpoint of help line
    // Mathematical calculations:
    if (cos(gun.angle * PI / 180) > 0) {
        if (-tan(gun.angle * PI / 180) < (double) (FINISH_Y - START_Y) / (FINISH_X - gun.x)) {
            x2 = FINISH_X;
            y2 = (Sint16) (FINISH_Y + (FINISH_X - gun.x) * tan(gun.angle * PI / 180));
        } else {
            x2 = (Sint16) (gun.x - (FINISH_Y - START_Y) * cot(gun.angle * PI / 180));
            y2 = START_Y;
        }
    } else if (cos(gun.angle * PI / 180) < 0) {
        if (tan(gun.angle * PI / 180) < (double) (FINISH_Y - START_Y) / (gun.x - START_X)) {
            x2 = START_X;
            y2 = (Sint16) (FINISH_Y - (gun.x - START_X) * tan(gun.angle * PI / 180));
        } else {
            x2 = (Sint16) (gun.x - (FINISH_Y - START_Y) * cot(gun.angle * PI / 180));
            y2 = START_Y;
        }
    } else {
        x2 = gun.x;
        y2 = START_Y;
    }
}

void make_shots() {
    if (shooting_flag && shot_index < n_shots) { //initializing shots
        shot[shot_index].life = 1;
        shot[shot_index].x = gun.x;
        shot[shot_index].y = gun.y;
        shot[shot_index].angle = gun.angle;
        shot_index++;
        gun.shots_in_screen++;
    }
    if (shot_index == n_shots) { //shooting finished!
        shooting_flag = 0;
        shot_index = 0;
    }
}

void bomb(int x, int y) {
    int counter = 0;
    for (int i = x - 1; i <= x + 1; i++) {
        for (int j = y - 1; j <= y + 1; j++) {
            if (i >= 0 && j >= 0 && i < 2 * n && j < 3 * n && bricks[i + (2 * n) * j] > 0 && bricks[i + (2 * n) * j] != BOMB) {
                bricks[i + (2 * n) * j]--; // Decrease resistance of neighbour bricks if exist.
                if (bricks[i + (2 * n) * j] == 0) {
                    gun.score++; // Increase score if a brick destroyed.
                    counter++;
                }
            } else {
                counter++;
            }
        }
    }
    if (counter == 9 && y != 0) { // it means there is no brick around the bomb!
        bricks[x + (2 * n) * y] = 0; // Destroy the bomb.
    }
}

void shot_reflection(int i) {
    //shot reflection in borders
    if (shot[i].x > FINISH_X - SHOT_RADIUS) { // right border
        shot[i].angle = (Sint16) (180 - shot[i].angle);
        shot[i].x -= SHOT_RADIUS / 2;
    }
    if (shot[i].x < START_X + SHOT_RADIUS) { // left border
        shot[i].angle = (Sint16) (180 - shot[i].angle);
        shot[i].x += SHOT_RADIUS / 2;
    }
    if (shot[i].y < START_Y + SHOT_RADIUS) { // top border
        shot[i].angle = -shot[i].angle;
        shot[i].y += SHOT_RADIUS / 2;
    }
    if (shot[i].y > FINISH_Y) { //bottom border
        shot[i].life = 0;
        shot[i].x = 0;
        shot[i].y = 0;
        gun.shots_in_screen--;
        if (gun.shots_in_screen <= 0) {
            make_bricks(); // make a new row.
        }
        return;
    }
    //confluence between shot and bricks
    POINT current_position = {(Sint16) ((shot[i].x - START_X) / BRICK_SIZE), (Sint16) ((shot[i].y - START_Y) / BRICK_SIZE)};
    if (sin(shot[i].angle * PI / 180) < 0 && cos(shot[i].angle * PI / 180) > 0) { // The shot is moving up-right
        if (absolute((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X)) < SHOT_RADIUS && bricks[current_position.x + 1 + (2 * n) * current_position.y] > 0) { //brick is on the right
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x + 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x + 1, current_position.y); //function of bomb
            }
            if (!bricks[current_position.x + 1 + (2 * n) * current_position.y]) {
                gun.score++; // Increase score if a brick destroyed
            }
        } else if (absolute((shot[i].y - START_Y) - current_position.y * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top
            shot[i].angle = (Sint16) (-shot[i].angle % 360); // reflect the shot
            if (bricks[current_position.x + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y - 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y - 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - current_position.y * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top-right corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x + 1, current_position.y - 1);
            }
            if (!bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) < 0 && cos(shot[i].angle * PI / 180) < 0) { // The shot is moving up-left
        if (absolute((shot[i].x - START_X) - (current_position.x) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x - 1 + (2 * n) * current_position.y] > 0) { //brick is on the left
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x - 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x - 1, current_position.y);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * current_position.y]) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - current_position.y * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top
            shot[i].angle = (Sint16) (-shot[i].angle % 360); // reflect the shot
            if (bricks[current_position.x + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y - 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y - 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - current_position.y * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top-left corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x - 1, current_position.y - 1);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) > 0 && cos(shot[i].angle * PI / 180) > 0) { // The shot is moving bottom-right
        if (absolute((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X)) < SHOT_RADIUS && bricks[current_position.x + 1 + (2 * n) * current_position.y] > 0) { //brick is on the right
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x + 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x + 1, current_position.y);
            }
            if (!bricks[current_position.x + 1 + (2 * n) * current_position.y]) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom
            shot[i].angle = (Sint16) (-shot[i].angle % 360); // reflect the shot
            if (bricks[current_position.x + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y + 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y + 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom-right corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x + 1, current_position.y + 1);
            }
            if (!bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) > 0 && cos(shot[i].angle * PI / 180) < 0) { // The shot is moving bottom-left
        if (absolute((shot[i].x - START_X) - (current_position.x) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x - 1 + (2 * n) * current_position.y] > 0) { //brick is on the left
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x - 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x - 1, current_position.y);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * current_position.y]) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom
            shot[i].angle = (Sint16) (-shot[i].angle % 360); // reflect the shot
            if (bricks[current_position.x + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y + 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y + 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom-left corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            if (bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)]--; // Decrease the resistance
            } else {
                bomb(current_position.x - 1, current_position.y + 1);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360); // reflect the shot
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--; // Decrease the resistance
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    }
}

void shot_motion() { // motion of each shot.
    for (int i = 0; i < n_shots; i++) {
        if (shot[i].life) {
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            shot_reflection(i); // check the reflections.
        }
    }
}

void gun_motion() {
    gun.x += STEP * (keys[SDLK_RIGHT % 401] - keys[SDLK_LEFT % 401]); // change x.
    //limits for motion of gun :
    if (gun.x - GUN_RADIUS < START_X) { gun.x = START_X + GUN_RADIUS; }
    else if (gun.x + GUN_RADIUS > FINISH_X) { gun.x = (Sint16) (FINISH_X - GUN_RADIUS); }
    help_line_coordinates(); // calculate the endpoint of help line.
}

void gun_rotation() {
    gun.angle += OMEGA * (keys[SDLK_m % 401] - keys[SDLK_n % 401]); //change angle.
    //limits fot rotation.
    if (gun.angle >= 0) { gun.angle = -1; }
    else if (gun.angle <= -180) { gun.angle = -179; }
}

int get_keys() {
    while (SDL_PollEvent(&event)) { // this function gives the keyboard events.
        switch (event.type) {
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN: //pressing a key
                keys[event.key.keysym.sym % 401] = 1; // save 1 if a key is pressed and %401 is to decrease the amount of a big number.
                break;
            case SDL_KEYUP: //dropping a key
                keys[event.key.keysym.sym % 401] = 0;
                break;
            default:
                break;
        }
    }
}

void setting() {
    srand((unsigned int) time(NULL));
    //random position for gun:
    gun.x = (Sint16) (START_X + rand() % ((int) FINISH_X - START_X));
    gun.y = FINISH_Y;
    gun.angle = -90;
    gun.score = 0;
    for (int i = 0; i < 6 * n * n; i++){
        bricks[i] = 0;
    }
    n_shots = 1;
    make_bricks(); // this function makes a row of bricks.
}

int menu_events() {
    if (get_keys() == -1) { // if close button pressed! get_keys functions will read keyboard events.
        return -1;
    }
    if (menu_state == 0) {
        if (keys[SDLK_RETURN % 401]) { // change the state of menu to start new game.
            menu_state = 1;
            keys[SDLK_RETURN % 401] = 0;
        } else if (keys[SDLK_l % 401]) { // change the state of menu to load a saved game.
            menu_state = 2;
            keys[SDLK_l % 401] = 0;
        }
    } else if (menu_state == 1) { // starting a new game.
        if (keys[SDLK_RETURN % 401]) { // start!
            setting(); //settings before starting the game.
            state = 1; // play the game
            keys[SDLK_RETURN % 401] = 0;
        } else if (keys[SDLK_UP % 401] && n < 40) { // Increase n value
            n++;
            keys[SDLK_UP % 401] = 0;
        } else if (keys[SDLK_DOWN % 401] && n > 1) { // Decrease n value
            n--;
            keys[SDLK_DOWN % 401] = 0;
        }
    } else if (menu_state == 2) { // loading a new game
        if (!(last_number - 1)) { // there is no saved game!
            if (keys[SDLK_ESCAPE % 401]) { // change menu state to new game mode.
                menu_state = 1;
                keys[SDLK_ESCAPE % 401] = 0;
            }
        } else {
            if (keys[SDLK_RETURN % 401]) { //load!
                load(); // this function loads the selected number.
                state = 1; // play the game
                keys[SDLK_RETURN % 401] = 0;
            } else if (keys[SDLK_UP % 401] && load_number < last_number - 1) { // Increase load number
                load_number++;
                keys[SDLK_UP % 401] = 0;
            } else if (keys[SDLK_DOWN % 401] && load_number > 1) { // Increase load number
                load_number--;
                keys[SDLK_DOWN % 401] = 0;
            }
        }
    }
}


int events() {
    if (get_keys() == -1) { // if close button pressed! get_keys functions will read keyboard events.
        return -1;
    }
    if (!save_mode && !gun.shots_in_screen) { // keys will be applied if save_mode is off and there is no shots in the screen.
        if (keys[SDLK_SPACE % 401]) { // shoot!!
            if (!shooting_flag) {
                n_shots = (Sint16) (gun.score ? gun.score : 1); // number of shots will be equal to score! and it will be 1 if score is 0.
                shot_index = 0;
                gun.shots_in_screen = 0;
            }
            keys[SDLK_SPACE % 401] = 0;
            shooting_flag = 1;
        } else if (keys[SDLK_s % 401]) { // saving mode!
            save_mode = 1;
        }
    } else if (save_mode) {
        if (keys[SDLK_ESCAPE % 401]) { //back to the game
            save_mode = 0;
        } else if (keys[SDLK_RETURN % 401]) { //save the game!!
            save();
        }
    }
}
