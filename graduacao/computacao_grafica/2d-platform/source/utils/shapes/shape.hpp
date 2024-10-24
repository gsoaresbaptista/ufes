#ifndef SHAPE_HPP
#define SHAPE_HPP

class Shape {
    protected:
        float x;
        float y;
        float color[3];
    public:
        Shape(float x, float y, float r, float g, float b);
        ~Shape();
        float get_x();
        float get_y();
        void set_x(float x);
        void set_y(float y);
};

#endif