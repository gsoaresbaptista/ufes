#ifndef CIRCLE_HPP
#define CIRCLE_HPP

#include "shape.hpp"

class Circle: public Shape {
    private:
        float radius;
    public:
        Circle(float x, float y, float radius,
            float r, float g, float b);
        ~Circle() {};
        void set_x(float x);
        void set_y(float y);
        float get_radius();
        void draw();
};

#endif