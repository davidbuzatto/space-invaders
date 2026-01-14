/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld struct and function declarations.
 * 
 * @copyright Copyright (c) 2025
 */
#pragma once

#include "Types.h"

#define ENEMY_COUNT 50
#define OBSTACLE_COUNT 4
#define MAX_PLAYER_PROJECTILE_COUNT 50

typedef struct GameWorld {
    Player player;
    Enemy enemies[ENEMY_COUNT];
    Obstacle obstacles[OBSTACLE_COUNT];
    Projectile playerProjectiles[MAX_PLAYER_PROJECTILE_COUNT];
    GameState state;
    int currentProjectileCount;
} GameWorld;

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void );

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw );

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta );

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw );