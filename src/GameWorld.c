/**
 * @file GameWorld.h
 * @author Prof. Dr. David Buzatto
 * @brief GameWorld implementation.
 * 
 * @copyright Copyright (c) 2025
 */
#include <stdio.h>
#include <stdlib.h>

#include "GameWorld.h"
#include "ResourceManager.h"

#include "Player.h"
#include "Enemy.h"
#include "Obstacle.h"
#include "Projectile.h"

#include "raylib/raylib.h"
//#include "raylib/raymath.h"
//#define RAYGUI_IMPLEMENTATION    // to use raygui, comment these three lines.
//#include "raylib/raygui.h"       // other compilation units must only include
//#undef RAYGUI_IMPLEMENTATION     // raygui.h

static void activateProjectile( Projectile *p, GameWorld *gw );
static void resolveProjectileEnemyCollision( GameWorld *gw );
static void resolveProjectileObstacleCollision( GameWorld *gw );

/**
 * @brief Creates a dinamically allocated GameWorld struct instance.
 */
GameWorld* createGameWorld( void ) {

    GameWorld *gw = (GameWorld*) calloc( 1, sizeof( GameWorld ) );

    Player *p = &gw->player;
    p->dim = (Vector2) { 40, 20 };
    p->color = BLUE;
    p->pos = (Vector2) { GetScreenWidth() / 2 - p->dim.x / 2, GetScreenHeight() - p->dim.y - 20 };
    p->vel = 0;
    p->lives = 3;
    p->score = 0;

    int enemiesPerLine = 10;
    int enemyWidth = 40;
    int enemyHeight = 30;
    int enemyHGap = 15;
    int enemyVGap = 15;
    int totalEnemyWidth = enemiesPerLine * enemyWidth + enemiesPerLine * enemyHGap - enemyHGap;
    int totalEnemyHeight = ( ENEMY_COUNT / enemiesPerLine * enemyHeight ) + ( ENEMY_COUNT / enemiesPerLine * enemyVGap ) - enemyVGap;
    int enemyXStartOffset = -50;
    int enemyYStartOffset = -100;

    for ( int i = 0; i < ENEMY_COUNT; i++ ) {
        Enemy *e = &gw->enemies[i];
        int row = i / enemiesPerLine;
        int col = i % enemiesPerLine;
        e->dim = (Vector2) { enemyWidth, enemyHeight };
        e->pos = (Vector2) { 
            GetScreenWidth() / 2 - totalEnemyWidth / 2 + e->dim.x * col + enemyHGap * col + enemyXStartOffset, 
            GetScreenHeight() / 2 - totalEnemyHeight / 2 + e->dim.y * row  + enemyVGap * row + enemyYStartOffset
        };
        e->vel = (Vector2) { 30, 2 };
        e->color = YELLOW;
        e->hp = 1;
        e->points = 10;
    }

    int obstaclePieceWidth = 8;
    int obstaclePieceHeight = 15;
    int obstacleWidth = OBSTACLE_COLS * obstaclePieceWidth;
    int obstacleHeight = OBSTACLE_ROWS * obstaclePieceHeight;
    int obstacleHGap = 50;
    int totalObstacleWidth = OBSTACLE_COUNT * obstacleWidth + OBSTACLE_COUNT * obstacleHGap - obstacleHGap;
    int obstacleYStartOffset = -50;

    for ( int i = 0; i < OBSTACLE_COUNT; i++ ) {
        Obstacle *o = &gw->obstacles[i];
        o->pieceDim = (Vector2) { obstaclePieceWidth, obstaclePieceHeight };
        o->pos = (Vector2) { 
            GetScreenWidth() / 2 - totalObstacleWidth / 2 + obstacleWidth * i + obstacleHGap * i,
            p->pos.y - obstacleHeight + obstacleYStartOffset
        };
        o->color = GREEN;
        for ( int j = 0; j < OBSTACLE_ROWS; j++ ) {
            for ( int k = 0; k < OBSTACLE_COLS; k++ ) {
                o->structure[j][k] = 1;
            }
        }
    }

    gw->currentProjectileCount = 0;

    return gw;

}

/**
 * @brief Destroys a GameWindow object and its dependecies.
 */
void destroyGameWorld( GameWorld *gw ) {
    free( gw );
}

/**
 * @brief Reads user input and updates the state of the game.
 */
void updateGameWorld( GameWorld *gw, float delta ) {

    updatePlayer( &gw->player, delta );

    for ( int i = 0; i < ENEMY_COUNT; i++ ) {
        updateEnemy( &gw->enemies[i], delta );
    }

    for ( int i = 0; i < gw->currentProjectileCount; i++ ) {
        updateProjectile( &gw->playerProjectiles[i], delta );
    }

    if ( IsKeyPressed( KEY_SPACE ) ) {
        int pos = gw->currentProjectileCount % MAX_PLAYER_PROJECTILE_COUNT;
        activateProjectile( &gw->playerProjectiles[pos], gw );
    }

    resolveProjectileEnemyCollision( gw );
    resolveProjectileObstacleCollision( gw );

}

/**
 * @brief Draws the state of the game.
 */
void drawGameWorld( GameWorld *gw ) {

    BeginDrawing();
    ClearBackground( BLACK );

    for ( int i = 0; i < MAX_PLAYER_PROJECTILE_COUNT; i++ ) {
        drawProjectile( &gw->playerProjectiles[i] );
    }

    drawPlayer( &gw->player );

    for ( int i = 0; i < ENEMY_COUNT; i++ ) {
        drawEnemy( &gw->enemies[i] );
    }

    for ( int i = 0; i < OBSTACLE_COUNT; i++ ) {
        drawObstacle( &gw->obstacles[i] );
    }

    DrawText( TextFormat( "SCORE: %d", gw->player.score ), 10, 10, 20, gw->player.color );

    EndDrawing();

}

static void activateProjectile( Projectile *p, GameWorld *gw ) {
    p->active = true;
    p->dim = (Vector2) { 5, 20 };
    p->pos.x = gw->player.pos.x + gw->player.dim.x / 2 - p->dim.x / 2;
    p->pos.y = gw->player.pos.y;
    p->vel = (Vector2) { 0, -500 };
    p->color = RED;
    gw->currentProjectileCount++;
}

static void resolveProjectileEnemyCollision( GameWorld *gw ) {

    for ( int i = 0; i < ENEMY_COUNT; i++ ) {
        Enemy *e = &gw->enemies[i];
        for ( int j = 0; j < MAX_PLAYER_PROJECTILE_COUNT; j++ ) {
            Projectile *p = &gw->playerProjectiles[j];
            if ( p->active && e->hp != 0 ) {
                bool collided = CheckCollisionRecs(
                    (Rectangle) { e->pos.x, e->pos.y, e->dim.x, e->dim.y },
                    (Rectangle) { p->pos.x, p->pos.y, p->dim.x, p->dim.y }
                );
                if ( collided ) {
                    (e->hp)--;
                    p->active = false;
                    gw->player.score += e->points;
                    break;
                }
            }
        }
    }

}

static void resolveProjectileObstacleCollision( GameWorld *gw ) {

    for ( int i = 0; i < OBSTACLE_COUNT; i++ ) {

        Obstacle *o = &gw->obstacles[i];

        for ( int j = 0; j < OBSTACLE_ROWS; j++ ) {
            for ( int k = 0; k < OBSTACLE_COLS; k++ ) {

                Rectangle piece = {
                    o->pos.x + o->pieceDim.x * k,
                    o->pos.y + o->pieceDim.y * j,
                    o->pieceDim.x,
                    o->pieceDim.y
                };

                //DrawRectangleLinesEx( piece, 1, WHITE );

                for ( int m = 0; m < MAX_PLAYER_PROJECTILE_COUNT; m++ ) {
                    Projectile *p = &gw->playerProjectiles[m];
                    if ( p->active && o->structure[j][k] != 0 ) {
                        bool collided = CheckCollisionRecs(
                            piece,
                            (Rectangle) { p->pos.x, p->pos.y, p->dim.x, p->dim.y }
                        );
                        if ( collided ) {
                            o->structure[j][k] = 0;
                            p->active = false;
                            break;
                        }
                    }
                }
            }
        }
    }

}