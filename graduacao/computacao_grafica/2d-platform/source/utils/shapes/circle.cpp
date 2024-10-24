#include "circle.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>

#define INCREMENT 2.0 * M_PI / 50

Circle::Circle(float x, float y, float radius,
        float r, float g, float b) : Shape(x, y, r, g, b) {
    this->radius = radius;
}

float Circle::get_radius() {
    return this->radius;
}

void Circle::draw() {
    glColor3f(this->color[0], this->color[1], this->color[2]);
    glTranslatef(this->x, this->y - this->radius, 0);
    
    glBegin(GL_POLYGON);
         for (float angle = 0; angle < 360; angle += INCREMENT) {
            glVertex2f(radius * cos(angle), radius * sin(angle));
        }
    glEnd();
}
