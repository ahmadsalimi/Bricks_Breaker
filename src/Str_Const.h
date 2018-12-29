#include <SDL.h>
#ifndef PROJECT_STR_CONST_H
#define PROJECT_STR_CONST_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define FPS 120
#define START_Y 100
#define START_X 100
#define FINISH_Y (SCREEN_HEIGHT - START_Y)
#define FINISH_X (START_X + (double) (FINISH_Y - START_Y) * 2 / 3)
#define BRICK_SIZE ((double) (FINISH_X - START_X) / (2 * n))
#define STEP 5
#define SHOT_STEP STEP
#define OMEGA 1
#define GUN_RADIUS 8
#define SHOT_RADIUS 8
#define PI 3.141592653589
#define FIRE_DELAY 10

typedef struct {
    bool life;
    double x;
    double y;
    Sint16 angle;
} SHOT;

typedef struct {
    Sint16 x;
    Sint16 y;
    Sint16 angle;
    Sint16 score;
    Sint16 shots_in_screen;
} GUN;

typedef struct {
    Sint16 x;
    Sint16 y;
    Sint16 r;
} BRICK;

typedef struct {
    Sint16 x;
    Sint16 y;
} POINT;

extern int n;
extern Sint16 n_shots;
extern bool state;
extern bool flag;
extern bool make;
extern bool shooting_flag;
extern GUN gun;
extern Sint16 x2, y2;
extern BRICK bricks[1000];
extern SHOT shot[1000];

#endif //PROJECT_STR_CONST_H
