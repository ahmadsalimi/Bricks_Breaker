#include <SDL.h>
#include <SDL2_gfxPrimitives.h>
#include <stdbool.h>
#include <time.h>
#include <math.h>
#include <stdio.h>
#include "Str_Const.h"
#include "View.h"
#include "Physics.h"


SDL_Event event;
int keys[401];
int shot_index = 0;
Sint16 n_shots;
bool shooting_flag;
Sint8 state;
bool flag = 1;
Sint16 x2, y2;
GUN gun;
SHOT shot[1000];
bool save_mode = 0;

double cot(double a) {
    return 1 / tan(a);
}

double absolute(double x) {
    return x > 0 ? x : -x;
}

void help_line_coordinates() { // to find the last point of help line
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
                bricks[i + (2 * n) * j]--;
                if (bricks[i + (2 * n) * j] == 0) {
                    gun.score++;
                    counter++;
                }
            } else {
                counter++;
            }
        }
    }
    if (counter == 9 && y != 0) { // it means there is no brick around the bomb!
        bricks[x + (2 * n) * y] = 0;
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
            make_bricks();
        }
        return;
    }
    //confluence between shot and bricks
    POINT current_position = {(Sint16) ((shot[i].x - START_X) / BRICK_SIZE), (Sint16) ((shot[i].y - START_Y) / BRICK_SIZE)};
    if (sin(shot[i].angle * PI / 180) < 0 && cos(shot[i].angle * PI / 180) > 0) {
        if (absolute((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X)) < SHOT_RADIUS && bricks[current_position.x + 1 + (2 * n) * current_position.y] > 0) { //brick is on the right
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360);
            if (bricks[current_position.x + 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x + 1, current_position.y);
            }
            if (!bricks[current_position.x + 1 + (2 * n) * current_position.y]) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - current_position.y * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            if (bricks[current_position.x + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y - 1)]--;
            } else {
                bomb(current_position.x, current_position.y - 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - current_position.y * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top-right corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            if (bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)]--;
            } else {
                bomb(current_position.x + 1, current_position.y - 1);
            }
            if (!bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) < 0 && cos(shot[i].angle * PI / 180) < 0) {
        if (absolute((shot[i].x - START_X) - (current_position.x) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x - 1 + (2 * n) * current_position.y] > 0) { //brick is on the left
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360);
            if (bricks[current_position.x - 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x - 1, current_position.y);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * current_position.y]) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - current_position.y * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            if (bricks[current_position.x + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y - 1)]--;
            } else {
                bomb(current_position.x, current_position.y - 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - current_position.y * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)] > 0) { //brick is on the top-left corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            if (bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)]--;
            } else {
                bomb(current_position.x - 1, current_position.y - 1);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) > 0 && cos(shot[i].angle * PI / 180) > 0) {
        if (absolute((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X)) < SHOT_RADIUS && bricks[current_position.x + 1 + (2 * n) * current_position.y] > 0) { //brick is on the right
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360);
            if (bricks[current_position.x + 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x + 1, current_position.y);
            }
            if (!bricks[current_position.x + 1 + (2 * n) * current_position.y]) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            if (bricks[current_position.x + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y + 1)]--;
            } else {
                bomb(current_position.x, current_position.y + 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom-right corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            if (bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)]--;
            } else {
                bomb(current_position.x + 1, current_position.y + 1);
            }
            if (!bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) > 0 && cos(shot[i].angle * PI / 180) < 0) {
        if (absolute((shot[i].x - START_X) - (current_position.x) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x - 1 + (2 * n) * current_position.y] > 0) { //brick is on the left
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360);
            if (bricks[current_position.x - 1 + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x - 1, current_position.y);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * current_position.y]) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            if (bricks[current_position.x + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x + (2 * n) * (current_position.y + 1)]--;
            } else {
                bomb(current_position.x, current_position.y + 1);
            }
            if (!bricks[current_position.x + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (pow((current_position.x) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)] > 0) { //brick is on the bottom-left corner
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            if (bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)] != BOMB) {
                bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)]--;
            } else {
                bomb(current_position.x - 1, current_position.y + 1);
            }
            if (!bricks[current_position.x - 1 + (2 * n) * (current_position.y + 1)]) {
                gun.score++;
            }
        } else if (bricks[current_position.x + (2 * n) * current_position.y] > 0) {
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            if (bricks[current_position.x + (2 * n) * current_position.y] != BOMB) {
                bricks[current_position.x + (2 * n) * current_position.y]--;
            } else {
                bomb(current_position.x, current_position.y);
            }
            if (!bricks[current_position.x + (2 * n) * current_position.y]) {
                gun.score++;
            }
        }
    }
}

void shot_motion() {
    for (int i = 0; i < n_shots; i++) {
        if (shot[i].life) {
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
            shot_reflection(i);
        }
    }
}

void gun_motion() {
    gun.x += STEP * (keys[SDLK_RIGHT % 401] - keys[SDLK_LEFT % 401]);
    if (gun.x - GUN_RADIUS < START_X) { gun.x = START_X + GUN_RADIUS; }
    else if (gun.x + GUN_RADIUS > FINISH_X) { gun.x = (Sint16) (FINISH_X - GUN_RADIUS); }
    help_line_coordinates();
}

void gun_rotation() {
    gun.angle += OMEGA * (keys[SDLK_m % 401] - keys[SDLK_n % 401]);
    if (gun.angle >= 0) { gun.angle = -1; }
    else if (gun.angle <= -180) { gun.angle = -179; }
}

int get_keys() {
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                return -1;
            case SDL_KEYDOWN:
                keys[event.key.keysym.sym % 401] = 1;
                break;
            case SDL_KEYUP:
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
    make_bricks();
}

int menu_events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (menu_state == 0) {
        if (keys[SDLK_RETURN % 401]) {
            menu_state = 1;
            keys[SDLK_RETURN % 401] = 0;
        } else if (keys[SDLK_l % 401]) {
            menu_state = 2;
            keys[SDLK_l % 401] = 0;
        }
    } else if (menu_state == 1) {
        if (keys[SDLK_RETURN % 401]) {
            setting(); //settings before starting the game
            state = 1;
            keys[SDLK_RETURN % 401] = 0;
        } else if (keys[SDLK_UP % 401] && n < 40) {
            n++;
            keys[SDLK_UP % 401] = 0;
        } else if (keys[SDLK_DOWN % 401] && n > 1) {
            n--;
            keys[SDLK_DOWN % 401] = 0;
        }
    } else if (menu_state == 2) {
        if (!(last_number - 1)) {
            if (keys[SDLK_ESCAPE % 401]) {
                menu_state = 1;
                keys[SDLK_ESCAPE % 401] = 0;
            }
        } else {
            if (keys[SDLK_RETURN % 401]) {
                load();
                state = 1;
                keys[SDLK_RETURN % 401] = 0;
            } else if (keys[SDLK_UP % 401] && load_number < last_number - 1) {
                load_number++;
                keys[SDLK_UP % 401] = 0;
            } else if (keys[SDLK_DOWN % 401] && load_number > 1) {
                load_number--;
                keys[SDLK_DOWN % 401] = 0;
            }
        }
    }
}


int events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (!save_mode && !gun.shots_in_screen) {
        if (keys[SDLK_SPACE % 401]) {
            if (!shooting_flag) {
                n_shots = (Sint16) (gun.score ? gun.score : 1);
                shot_index = 0;
                gun.shots_in_screen = 0;
            }
            keys[SDLK_SPACE % 401] = 0;
            shooting_flag = 1;
        } else if (keys[SDLK_s % 401]) {
            save_mode = 1;
        }
    } else if (save_mode) {
        if (keys[SDLK_ESCAPE % 401]) {
            save_mode = 0;
        } else if (keys[SDLK_RETURN % 401]) {
            save();
        }
    }
}
