#ifndef RECT_HPP
#define RECT_HPP

#include "shape.hpp"

class Rect: public Shape {
    private:
        float width;
        float height;
        float angle;
        float alpha;
    public:
        Rect(float x, float y, float angle,
            float width, float height,
            float r=0, float g=0, float b=0, float alpha = 1.0);
        ~Rect();
        void set_angle(float angle);
        float get_angle();
        float get_height();
        float get_width();
        void draw_from_middle();
        void draw_from_left();
        void draw_lines();

        //
        bool collision(Rect* other,
                       float delta_x, float delta_y,
                       float& px, float& py);
        
        bool collision(Rect* other,
                       float delta_x, float delta_y);

        bool collision_from_left(Rect* other,
                       float delta_x, float delta_y);
};

#endif