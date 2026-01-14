#include "raylib/raylib.h"
#include "Types.h"
#include "Player.h"

void updatePlayer( Player *p, float delta ) {

    p->vel = 0;

    if ( IsKeyDown( KEY_RIGHT ) ) {
        p->vel = 200;
    }

    if ( IsKeyDown( KEY_LEFT ) ) {
        p->vel = -200;
    }

    p->pos.x += p->vel * delta;

    if ( p->pos.x < 0 ) {
        p->pos.x = 0;
    } else if ( p->pos.x + p->dim.x > GetScreenWidth() ) {
        p->pos.x = GetScreenWidth() - p->dim.x;
    }

}

void drawPlayer( Player *p ) {
    DrawRectangleV( p->pos, p->dim, p->color );
}