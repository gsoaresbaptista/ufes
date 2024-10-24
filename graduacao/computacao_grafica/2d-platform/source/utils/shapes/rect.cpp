#include "rect.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Rect::Rect(float x, float y, float angle,
        float width, float height,
        float r, float g, float b, float alpha) : Shape(x, y, r, g, b) {
    // 
    this->angle = angle;
    this->width = width;
    this->height = height;
    this->alpha = alpha;
}

Rect::~Rect() {
}

void Rect::set_angle(float angle) {
    this->angle = angle;
}

float Rect::get_height() {
    return this->height;
};

float Rect::get_angle() {
    return this->angle;
};

float Rect::get_width() {
    return this->width;
};

void Rect::draw_from_middle() {
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //
    glColor4f(this->color[0], this->color[1], this->color[2], this->alpha);
    glRotatef(this->angle, 0, 0, 1);
    glTranslatef(this->x, this->y, 0);

    glBegin(GL_QUADS);
        glVertex2f( -width/2, 0);
        glVertex2f(  width/2, 0);
        glVertex2f(  width/2, height);
        glVertex2f( -width/2, height);
    glEnd();
}

void Rect::draw_from_left() {
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //
    glColor4f(this->color[0], this->color[1], this->color[2], this->alpha);
    glTranslatef(this->x, this->y, 0);
    glRotatef(this->angle, 0, 0, 1);

    glBegin(GL_QUADS);
        glVertex2f( 0, height/2);
        glVertex2f( width, height/2);
        glVertex2f( width, -height/2);
        glVertex2f( 0, -height/2);
    glEnd();
}

void Rect::draw_lines() {
    //
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //
    glColor4f(this->color[0], this->color[1], this->color[2], this->alpha);
    glTranslatef(this->x, this->y, 0);
    glRotatef(this->angle, 0, 0, 1);

    glBegin(GL_LINE_LOOP);
        glVertex2f( -width/2, 0);
        glVertex2f(  width/2, 0);
        glVertex2f(  width/2, height);
        glVertex2f( -width/2, height);
    glEnd();
}

bool Rect::collision(
    Rect* other,
    float delta_x, float delta_y,
    float& px, float& py
) {
    // Check horizontal collision
    if ((x + width/2 + delta_x > other->x - other->width/2.0) &&
        (x - width/2 + delta_x < other->x + other->width/2.0)) {
        // Check vertical collision
        if ((y + delta_y < other->y + other->height) &&
            (y + height + delta_y > other->y)) {
                
                if (y + delta_y <= other->y + other->height)
                    py = other->y + other->height;
                else py = y;

                if (x < other->x) px = other->x - other->width/2.0;
                else px = other->x + other->width/2.0;

                return true;
        }
    }
    return false;
}

bool Rect::collision( Rect* other, float delta_x, float delta_y) {
    // Check horizontal collision
    if ((x + width/2 + delta_x > other->x - other->width/2.0) &&
        (x - width/2 + delta_x < other->x + other->width/2.0)) {
        // Check vertical collision
        if ((y + delta_y < other->y + other->height) &&
            (y + height + delta_y > other->y)) {
                return true;
        }
    }
    return false;
}

bool Rect::collision_from_left(Rect* other, float delta_x, float delta_y) {
    // Check horizontal collision
    if ((x + width + delta_x > other->x - other->width/2.0) &&
        (x - width + delta_x < other->x + other->width/2.0)) {
        // Check vertical collision
        if ((y - height + delta_y < other->y + other->height) &&
            (y + height + delta_y > other->y)) {
                return true;
        }
    }
    return false;
}