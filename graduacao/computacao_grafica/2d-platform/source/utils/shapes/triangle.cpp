#include "triangle.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

Triangle::Triangle(
        float x, float y,
        float p0_x, float p1_x, float p2_x,
        float p0_y, float p1_y, float p2_y,
        float r, float g, float b
    ) : Shape(x, y, r, g, b) {
    //
    this->x = x;
    this->y = y;
    // 
    this->p0_x = p0_x;
    this->p1_x = p1_x;
    this->p2_x = p2_x;
    //
    this->p0_y = p0_y;
    this->p1_y = p1_y;
    this->p2_y = p2_y;
}

Triangle::~Triangle() {
}

void Triangle::draw() {
    glColor3f(this->color[0], this->color[1], this->color[2]);
    glTranslatef(this->x, this->y, 0);

    glBegin(GL_TRIANGLE_STRIP);
        glVertex3f(p0_x, p0_y, 0.0);
        glVertex3f(p1_x, p1_y, 0.0);
        glVertex3f(p2_x, p2_y, 0.0);
    glEnd();
}
