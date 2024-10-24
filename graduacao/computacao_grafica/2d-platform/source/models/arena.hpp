#ifndef ARENA_HPP
#define ARENA_HPP

#include "character.hpp"
#include "character.hpp"
#include "enemy.hpp"
#include "gunshot.hpp"

#include "../utils/svgInput.hpp"
#include "../utils/shapes/rect.hpp"

class Arena {
    private:
        bool game_won;
        bool player_got_shot;
        bool collision_on;
        bool enemy_walk;
        bool enemy_gunshot;

        float velocity;
        float camera_size;

        //
        float world_width;
        float world_height;

        Rect* character_box;

        std::vector<Rect*>* obstacles;
        std::vector<Enemy*>* enemies;
        std::vector<Gunshot*> bullets;

    public:
        Arena(Rect* character_box, float velocity);
        ~Arena();
        void set_settings(SVGInput* settings);
        void gravity(Character* character, double dt);
        void set_collision(bool value);
        void set_enemies_collision_box(bool value);
        void set_enemy_walk(bool value);
        void set_enemy_gunshot(bool value);
        bool get_player_got_shot();
        bool get_game_won();
        void set_player_got_shot(bool value);

        int collision(Character* character, float delta_x, float delta_y);
        int collision(Character* character, float delta_x, float delta_y, float& px, float& py);

        void move_bullets(float dt);
        void add_gunshoot(Gunshot* bullet);

        void draw();
        void process(double dt);

        void move_right(Enemy* enemy, double dt);
        void move_left(Enemy* enemy, double dt);
};

#endif