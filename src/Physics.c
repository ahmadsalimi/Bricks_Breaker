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
bool state;
bool flag = 1;
Sint16 x2, y2;
GUN gun;
SHOT shot[1000];

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
    if (shooting_flag && shot_index < FIRE_DELAY * n_shots && !(shot_index % FIRE_DELAY)) { //initializing shots
        //printf("TODO\n");
        shot[shot_index / FIRE_DELAY].life = 1;
        shot[shot_index / FIRE_DELAY].x = gun.x;
        shot[shot_index / FIRE_DELAY].y = gun.y;
        shot[shot_index / FIRE_DELAY].angle = gun.angle;
        shot_index += FIRE_DELAY;
        gun.shots_in_screen++;
        //printf("Done\n");
    }
    if (shot_index == FIRE_DELAY * n_shots) { //shooting finished!
        shooting_flag = 0;
        shot_index = 0;
        //printf("Checked\n");
    }
}

void shot_reflection(int i) {
    //shot reflection in borders
    if (shot[i].x > FINISH_X - SHOT_RADIUS || shot[i].x < START_X + SHOT_RADIUS) { // left or right border
        shot[i].angle = (Sint16) (180 - shot[i].angle);
    }
    if (shot[i].y < START_Y + SHOT_RADIUS) { // top border
        shot[i].angle = -shot[i].angle;
        shot[i].y += SHOT_RADIUS;
    }
    if (shot[i].y > FINISH_Y) { //bottom border
        shot[i].life = 0;
        shot[i].x = 0;
        shot[i].y = 0;
        gun.shots_in_screen--;
        if (!gun.shots_in_screen) {
            make_bricks();
        }
    }
    //confluence between shot and bricks
    POINT current_position = {(Sint16) ((shot[i].x - START_X) / BRICK_SIZE), (Sint16) ((shot[i].y - START_Y) / BRICK_SIZE)};
    if (sin(shot[i].angle * PI / 180) < 0 && cos(shot[i].angle * PI / 180) > 0) {
        if (absolute((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X)) < SHOT_RADIUS && bricks[current_position.x + 1 + (2 * n) * current_position.y].r) { //brick is on the right
            printf("right: %d -> %d\n", shot[i].angle, (180 - shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360);
            bricks[current_position.x + 1 + (2 * n) * current_position.y].r--;
            if(!bricks[current_position.x + 1 + (2 * n) * current_position.y].r) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - current_position.y * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y - 1)].r) { //brick is on the top
            printf("top: %d -> %d\n", shot[i].angle, -shot[i].angle % 360);
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            bricks[current_position.x + (2 * n) * (current_position.y - 1)].r--;
            if(!bricks[current_position.x + (2 * n) * (current_position.y - 1)].r) {
                gun.score++;
            }
        } else if (pow((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - current_position.y * BRICK_SIZE, 2) < pow(SHOT_RADIUS / 2, 2) && bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)].r) { //brick is on the top-right corner
            printf("top-right: %d -> %d\n", shot[i].angle, (180 + shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)].r--;
            if(!bricks[current_position.x + 1 + (2 * n) * (current_position.y - 1)].r) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) < 0 && cos(shot[i].angle * PI / 180) < 0) {
        if (absolute((shot[i].x - START_X) - (current_position.x) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x - 1 + (2 * n) * current_position.y].r) { //brick is on the left
            printf("left: %d -> %d\n", shot[i].angle, (180 - shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 - shot[i].angle) & 360);
            bricks[current_position.x - 1 + (2 * n) * current_position.y].r--;
            if(!bricks[current_position.x - 1 + (2 * n) * current_position.y].r) {
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - current_position.y * BRICK_SIZE < SHOT_RADIUS) && bricks[current_position.x + (2 * n) * (current_position.y - 1)].r) { //brick is on the top
            printf("top: %d -> %d\n", shot[i].angle, -shot[i].angle % 360);
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            bricks[current_position.x + (2 * n) * (current_position.y - 1)].r--;
            if(!bricks[current_position.x + (2 * n) * (current_position.y - 1)].r) {
                gun.score++;
            }
        } else if (pow((current_position.x) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - current_position.y * BRICK_SIZE, 2) < pow(SHOT_RADIUS, 2) && bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)].r) { //brick is on the top-left corner
            printf("top-left: %d -> %d\n", shot[i].angle, (180 + shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)].r--;
            if(!bricks[current_position.x - 1 + (2 * n) * (current_position.y - 1)].r) {
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) > 0 && cos(shot[i].angle * PI / 180) > 0) {
        if (absolute((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X)) < SHOT_RADIUS && bricks[current_position.x + 1 + (2 * n) * current_position.y].r) { //brick is on the right
            printf("right: %d -> %d\n", shot[i].angle, (180 - shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 - shot[i].angle) % 360);
            bricks[current_position.x + 1 + (2 * n) * current_position.y].r--;
            if(!bricks[current_position.x + 1 + (2 * n) * current_position.y].r){
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y + 1)].r) { //brick is on the bottom
            printf("bottom: %d -> %d\n", shot[i].angle, -shot[i].angle % 360);
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            bricks[current_position.x + (2 * n) * (current_position.y + 1)].r--;
            if(!bricks[current_position.x + (2 * n) * (current_position.y + 1)].r) {
                gun.score++;
            }
        } else if (pow((current_position.x + 1) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE, 2) < pow(SHOT_RADIUS, 2) && bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)].r) { //brick is on the bottom-right corner
            printf("bottom-right: %d -> %d\n", shot[i].angle, (180 + shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)].r--;
            if(!bricks[current_position.x + 1 + (2 * n) * (current_position.y + 1)].r){
                gun.score++;
            }
        }
    } else if (sin(shot[i].angle * PI / 180) > 0 && cos(shot[i].angle * PI / 180) < 0) {
        if (absolute((shot[i].x - START_X) - (current_position.x) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x - 1 + (2 * n) * current_position.y].r) { //brick is on the left
            printf("left: %d -> %d\n", shot[i].angle, (180 - shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 - shot[i].angle) & 360);
            bricks[current_position.x - 1 + (2 * n) * current_position.y].r--;
            if(!bricks[current_position.x - 1 + (2 * n) * current_position.y].r){
                gun.score++;
            }
        } else if (absolute((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE) < SHOT_RADIUS && bricks[current_position.x + (2 * n) * (current_position.y + 1)].r) { //brick is on the bottom
            printf("bottom: %d -> %d\n", shot[i].angle, -shot[i].angle % 360);
            shot[i].angle = (Sint16) (-shot[i].angle % 360);
            bricks[current_position.x + (2 * n) * (current_position.y + 1)].r--;
            if(!bricks[current_position.x + (2 * n) * (current_position.y + 1)].r){
                gun.score++;
            }
        } else if (pow((current_position.x) * BRICK_SIZE - (shot[i].x - START_X), 2) + pow((shot[i].y - START_Y) - (current_position.y + 1) * BRICK_SIZE, 2) < pow(SHOT_RADIUS, 2) && bricks[current_position.x + (2 * n) * (current_position.y + 1)].r) { //brick is on the bottom-left corner
            printf("bottom-left: %d -> %d\n", shot[i].angle, (180 + shot[i].angle) % 360);
            shot[i].angle = (Sint16) ((180 + shot[i].angle) % 360);
            bricks[current_position.x + (2 * n) * (current_position.y + 1)].r--;
            if(!bricks[current_position.x + (2 * n) * (current_position.y + 1)].r){
                gun.score++;
            }
        }
    }
}

void shot_motion() {
    for (int i = 0; i < n_shots; i++) {
        if (shot[i].life) {
            shot_reflection(i);
            shot[i].x += 2 * SHOT_STEP * cos(shot[i].angle * PI / 180);
            shot[i].y += 2 * SHOT_STEP * sin(shot[i].angle * PI / 180);
        }
    }
}

void gun_motion() {
    gun.x += STEP * (keys[SDLK_RIGHT % 401] - keys[SDLK_LEFT % 401]);
    if (gun.x - GUN_RADIUS < START_X) { gun.x = START_X + GUN_RADIUS; }
    else if (gun.x + GUN_RADIUS > FINISH_X) { gun.x = FINISH_X - GUN_RADIUS; }
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

int menu_events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (keys[SDLK_RETURN % 401]) {
        state = 1;
        keys[SDLK_RETURN % 401] = 0;
    }
}


int events() {
    if (get_keys() == -1) {
        return -1;
    }
    if (keys[SDLK_SPACE % 401]) {
        if (!shooting_flag) {
            n_shots = (Sint16) (gun.score + 1);
            shot_index = 0;
            gun.shots_in_screen = 0;
        }
        keys[SDLK_SPACE % 401] = 0;
        shooting_flag = 1;
    }
    make_shots();
    if (!gun.shots_in_screen){
        gun_motion();
        gun_rotation();
    }
    shot_motion();
}
