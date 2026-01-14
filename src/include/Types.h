#pragma once

#include <stdbool.h>
#include "raylib/raylib.h"

#define OBSTACLE_ROWS 4
#define OBSTACLE_COLS 8

typedef struct Player {
    Vector2 pos;
    float vel;
    Vector2 dim;
    Color color;
    int score;
    int lives;
} Player;

typedef struct Enemy {
    Vector2 pos;
    Vector2 vel;
    Vector2 dim;
    Color color;
    int hp;
    int points;
} Enemy;

typedef struct Obstacle {
    Vector2 pos;
    Vector2 pieceDim;
    Color color;
    bool structure[OBSTACLE_ROWS][OBSTACLE_COLS];
} Obstacle;

typedef struct Projectile {
    Vector2 pos;
    Vector2 vel;
    Vector2 dim;
    Color color;
    bool active;
} Projectile;

typedef enum GameState {
    GAME_STATE_START,
    GAME_STATE_PLAYING,
    GAME_STATE_WON,
    GAME_STATE_LOSE
} GameState;