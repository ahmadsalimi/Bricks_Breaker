#include <SDL.h>

#ifndef PROJECT_STR_CONST_H
#define PROJECT_STR_CONST_H

// all of constants and structs are defined here.

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define FPS 120 // Frame Per Second
#define START_Y 100 // X coordinate of the starting point of the game map
#define START_X 100 // Y coordinate of the starting point of the game map
#define FINISH_Y (SCREEN_HEIGHT - START_Y) // X coordinate of the ending point of the game map
#define FINISH_X (START_X + (double) (FINISH_Y - START_Y) * 2 / 3) // Y coordinate of the ending point of the game map
#define BRICK_SIZE ((double) (FINISH_X - START_X) / (2 * n)) // size of bricks (depends on "n" value)
#define STEP 5 // speed of gun
#define SHOT_STEP STEP // speed of shots
#define OMEGA 1 // rotation rate of gun
#define GUN_RADIUS 8
#define SHOT_RADIUS 8
#define PI 3.141592653589
#define BOMB 10000

typedef struct {
    bool life; // if shot is in the game
    double x;
    double y;
    Sint16 angle;
} SHOT;

typedef struct {
    Sint16 x;
    Sint16 y;
    Sint16 angle;
    Sint16 score;
    Sint16 shots_in_screen; // number of shots in the screen.
} GUN;

typedef struct {
    Sint16 x;
    Sint16 y;
} POINT;

extern int n; // the dimensions of game will be 2n * 3n
extern Sint16 n_shots; // number of shots in each level.
extern Sint8 state; //game state; 0(menu), 1(game), 2(game over)
extern Sint8 menu_state; // 0(starting), 1(new game), 2(load a game)
extern int last_number; // the number of last saved game plus one
extern int load_number;
extern bool flag;
extern bool save_mode;
extern GUN gun;
extern Sint16 x2, y2;
extern Sint16 bricks[10000];
extern SHOT shot[1000];

#endif //PROJECT_STR_CONST_H
