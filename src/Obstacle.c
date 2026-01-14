#include "raylib/raylib.h"
#include "Types.h"
#include "Obstacle.h"

void updateObstacle( Obstacle *o, float delta ) {

}

void drawObstacle( Obstacle *o ) {
    for ( int i = 0; i < OBSTACLE_ROWS; i++ ) {
        for ( int j = 0; j < OBSTACLE_COLS; j++ ) {
            if ( o->structure[i][j] == 1 ) {
                DrawRectangle( 
                    o->pos.x + o->pieceDim.x * j,
                    o->pos.y + o->pieceDim.y * i,
                    o->pieceDim.x,
                    o->pieceDim.y,
                    o->color
                    //Fade( o->color, 0.5f )
                );
            }
        }
    }
}