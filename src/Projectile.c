#include "raylib/raylib.h"
#include "Types.h"
#include "Player.h"

void updateProjectile( Projectile *p, float delta ) {

    p->pos.x += p->vel.x * delta;
    p->pos.y += p->vel.y * delta;

    if ( p->pos.y + p->dim.y < 0 ) {
        p->active = false;
    }

}

void drawProjectile( Projectile *p ) {
    if ( p->active ) {
        DrawRectangleV( p->pos, p->dim, p->color );
    }
}