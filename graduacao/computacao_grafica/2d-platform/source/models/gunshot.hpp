#ifndef GUNSHOT_HPP
#define GUNSHOT_HPP

#include "../utils/shapes/rect.hpp"

class Gunshot {
    private:
        float x;
        float y;
        float angle;
        float velocity;
        float delta_x;
        float delta_y;
        float width;
        float height;
        Rect* bullet_body;
    public:
        Gunshot(float x, float y,
                float width, float height,
                float angle, float velocity);
        ~Gunshot();
        void draw();
        void move(float dt);
        bool collision(Rect* rect, float dt);
};

#endif
