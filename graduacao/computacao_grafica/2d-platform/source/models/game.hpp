#ifndef GAME_HPP
#define GAME_HPP

#include "character.hpp"
#include "enemy.hpp"
#include "gunshot.hpp"
#include "arena.hpp"

#include "../utils/shapes/rect.hpp"
#include "../utils/svgInput.hpp"

#include <vector>

using namespace std;

class Game {
    private:
        Character* character;
        Arena* arena;
        
        float velocity;
        float camera_size;
        float window_width;
        float window_height;

        float gun_x;
        float gun_y;
        bool shot_trajectory_on;
        bool game_lost;
        bool game_won;

        SVGInput* default_game_settings;

        //
        inline void draw_shot_trajectory();
        
        //
        Game();
    public:
        static Game* get_instance();
        void set_settings(SVGInput* settings);
        void process(double dt);

        void move_right(double dt);
        void move_left(double dt);
        void jump(bool active, double dt);
        void shoot();

        void move_arm(int x, int y);
        void move_arm(float x, float y);

        void set_idle(bool state, double dt);
        bool get_game_lost();
        bool get_game_won();
        void gravity(double dt);
        
        void draw();
        void draw_end_game();
        void restart_game();

        void set_player_collision_box(bool value);
        void set_enemies_collision_box(bool value);
        void set_player_collision(bool value);
        void set_shot_tracing(bool value);
        void set_enemy_walk(bool value);
        void set_enemy_gunshot(bool value);
};

#endif
