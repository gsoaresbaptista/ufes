#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "gunshot.hpp"
#include "../utils/shapes/rect.hpp"
#include "../utils/shapes/circle.hpp"

class Character {
    private:
        //
        float x;
        float y;
        float height;

        //
        bool collision_flag;
        bool to_right;

        //
        bool jumping;
        bool falling_down;
        double falling_time;
        double rising_time;

        //
        bool idle_state;
        double current_frame;

        // 
        float head_offset;

        // 
        Rect* left_thigh;
        Rect* left_calf;
        Rect* right_thigh;
        Rect* right_calf;
        Rect* body;
        Circle* head;

        Rect* arm;
        Rect* forearm;
        float arm_angle;

        Rect* gun;
        Rect* gun_support;

        //
        bool collision_box;
        Rect* collision_rect;

        // 
        void update_angles();

        inline void set_angles(float head_offset, float left_thigh,
            float left_calf, float right_thigh, float right_calf);

        inline void create_character(float head_body, float legs, float r, float g, float b);
        void draw_collision_box();
        void draw_arm();
    public:
        Character(float x, float y, float diameter, float r=1, float g=01, float b=1);
        void move(float x, float y, double dt);
        void draw();
        void next_frame(double dt);
        void set_idle(bool state, double dt);
        float get_x();
        float get_y();
        void set_y(float y);
        void set_x(float x);
        float get_height();
        void set_arm_angle(float angle);
        float get_arm_angle();
        Rect* get_collision_rect();
        bool collision(Rect* rect, float delta_x, float delta_y);
        bool collision(Rect* rect, float delta_x, float delta_y, float& px, float& py);
        void get_gun_position(float& x_out, float& y_out);
        void set_direction(int quadrant, bool falling);
        bool get_idle_state();
        void set_collision_box(bool value);
        float get_width();

        //
        bool get_to_right();
        bool get_collision_flag();
        bool get_jumping();
        bool get_falling_down();
        double get_falling_time();
        double get_rising_time();
        Rect* get_gun_rect();

        double get_current_frame() { return this->current_frame;};

        //
        void set_to_right(bool value);
        void set_collision_flag(bool value);
        void set_jumping(bool value);
        void set_falling_down(bool value);
        void increase_falling_time(float dt);
        void clear_falling_time();
        void increase_rising_time(float dt);
        void clear_rising_time();
        void update_collision_rect();
};

#endif
