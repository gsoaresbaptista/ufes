#include "gunshot.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

Gunshot::Gunshot(float x, float y, float width, float height, float angle, float velocity) {
    this->width = width;
    this->height = height;
    this->x = x;
    this->y = y;
    //
    this->angle = angle;
    this->velocity = velocity;
    //
    this->bullet_body = new Rect(x, y, angle, width, height, 0.8, 0.8, 0);
    this->delta_x = this->velocity * cos(this->angle * M_PI/180);
    this->delta_y = this->velocity * sin(this->angle * M_PI/180);
}

Gunshot::~Gunshot() {
}

void Gunshot::move(float dt) {
    //
    this->y += dt * delta_y;
    this->x += dt * delta_x;
    //
    this->bullet_body->set_x(this->x);
    this->bullet_body->set_y(this->y);
}

bool Gunshot::collision(Rect* rect, float dt) {
    return this->bullet_body->collision_from_left(
        rect,
        this->delta_x * dt,
        this->delta_y * dt
    );
}

void Gunshot::draw() {
    glPushMatrix();
        this->bullet_body->draw_from_left();
    glPopMatrix();
}
