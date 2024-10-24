#ifndef TRIANGLE_HPP
#define TRIANGLE_HPP

#include "shape.hpp"

class Triangle: public Shape {
    private:
        float x, y;
        float p0_x, p1_x, p2_x;
        float p0_y, p1_y, p2_y;
    public:
        Triangle(
            float x, float y,
            float p0_x, float p1_x, float p2_x,
            float p0_y, float p1_y, float p2_y,
            float r=0, float g=0, float b=0);
        ~Triangle();
        void draw();
};

#endif