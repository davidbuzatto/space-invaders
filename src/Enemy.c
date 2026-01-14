#include <math.h>

#include "raylib/raylib.h"
#include "Types.h"
#include "Enemy.h"

void updateEnemy( Enemy *e, float delta ) {

    static float a = 0; // each enemy must have its own "a" (angle).
                        // this was just a test!

    if ( e->hp != 0 ) {
        e->pos.x += e->vel.x * sinf( DEG2RAD * a ) * delta * 1.5f;
        e->pos.y += e->vel.y * delta;
    }

    a += 1 * delta;

}

void drawEnemy( Enemy *e ) {
    if ( e->hp != 0 ) {
        DrawRectangleV( e->pos, e->dim, e->color );
        //DrawText( TextFormat( "%d", e->hp ), e->pos.x + 10, e->pos.y + 10, 20, BLACK );
    }
}