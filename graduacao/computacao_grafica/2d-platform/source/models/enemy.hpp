#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "character.hpp"

#include "../utils/shapes/rect.hpp"
    
class Enemy: public Character {
    private:
        Rect* last_collided_rect;
        bool updated_rect;
        bool walking;
        double without_shooting;

    public:
        Enemy(float x, float y, float diameter,
              float r, float g, float b);

        ~Enemy();

        static Enemy* clone(Enemy* p);

        void choose_direction();
        void set_collided_rect(Rect* rect);
        void process(double dt);
        void set_falling_down(bool value);
        void set_to_right(bool value);

        bool at_the_end();

        bool get_updated_rect();
        void set_walking_flag(bool value);
        bool get_walking_flag();
        double get_without_shooting();
        void increase_without_shooting(double dt);
        void clear_without_shooting();
};

#endif
