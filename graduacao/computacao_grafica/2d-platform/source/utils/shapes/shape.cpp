#include "shape.hpp"

Shape::Shape(float x, float y, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->color[0] = r;
    this->color[1] = g;
    this->color[2] = b;
}

Shape::~Shape() {
}

float Shape::get_x() {
    return this->x;
}

float Shape::get_y() {
    return this->y;
}

void Shape::set_x(float x) {
    this->x = x;
}

void Shape::set_y(float y) {
    this->y = y;
}
