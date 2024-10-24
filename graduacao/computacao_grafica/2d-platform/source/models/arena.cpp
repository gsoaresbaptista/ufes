#include "arena.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <iostream>
using namespace std;

#include "../utils/mathAux.hpp"

#include <cmath>

Arena::Arena(Rect* character_box, float velocity) {
    this->character_box = character_box;
    this->velocity = velocity;
    this->collision_on = true;
    this->player_got_shot = false;
    this->game_won = false;
    this->enemy_walk = true;
    this->enemy_gunshot = true;
}

Arena::~Arena() {
}

void Arena::set_settings(SVGInput* svg_data) {
    //
    this->enemies = new std::vector<Enemy*>();
    for (Enemy* enemy: *svg_data->enemies)
        this->enemies->push_back(Enemy::clone(enemy));

    //
    this->camera_size = svg_data->camera_size;
    this->world_width = svg_data->world_width;
    this->world_height = svg_data->world_height;

    // Define obstacles and create floor
    this->obstacles = svg_data->obstacles;
    this->obstacles->push_back(
        new Rect(
            svg_data->world_width/2,
            -1, 0,
            svg_data->world_width, 1)
    );
}

void Arena::gravity(Character* character, double dt) {
    if (!character->get_jumping()) {
        float px, py;
        character->increase_falling_time(dt);

        if (!this->collision(
                character, 0,
                -velocity*character->get_falling_time()/50,
                px, py)) {
            //
            character->set_falling_down(true);
            character->move(0,
                -velocity*character->get_falling_time()/50, 0
            );
        }
        else if (character->get_collision_flag()) {
            character->set_y(py);
            character->set_falling_down(false);
            character->clear_falling_time();
        }
    }

    if (!character->get_collision_flag() && character->get_y() < 0) {
        character->set_y(0);
        character->set_falling_down(false);
        character->clear_falling_time();
    }
}

void Arena::set_collision(bool value) {
    this->collision_on = value;
}

int Arena::collision(Character* character, float delta_x, float delta_y) {
    //
    float px, py;
    return collision(character, delta_x, delta_y, px, py);
}

int Arena::collision(Character* character, float delta_x, float delta_y, float& px, float& py) {
    bool first = true;
    float lx = -1, ly = -1;

    //
    if (!character->get_collision_flag()) {
        return 0;
    }

    //
    if ((character->get_collision_rect() != this->character_box) &&
        (character->collision(this->character_box, delta_x, delta_y, px, py)) &&
        (this->collision_on)) {
            return 1;
        }
    //
    for (Rect* obstacle: *this->obstacles) {
        if (character->collision(obstacle, delta_x, delta_y, lx, ly)) {
            if ((delta_x < 0 && lx > px) || (delta_x > 0 && lx < px) ||
                (delta_y < 0 && ly > py) || (delta_y > 0 && ly < py) ||
                first) {
                //
                px = lx; py = ly;
                first = false;
            }
        }
    }

    if (lx != -1 || ly != -1)
        return 2;

    //
    for (Enemy* enemy: *this->enemies) {
        if (character->collision(
                enemy->get_collision_rect(),
                delta_x, delta_y, px, py)
            && enemy != character)
            //
            return 3;
    }

    //
    if (character->get_x() - character->get_width()/2 + delta_x <= 0) {
        px = 0;
        py = character->get_y();
        return 2;
    }

    //
    if (character->get_y() + character->get_height() + delta_y >= world_height) {
        px = character->get_x();
        py = character->get_y();
        return 2;
    }

    return 0;
}

void Arena::move_bullets(float dt) {
    bool move;
    auto it = this->bullets.begin();

    while (it != this->bullets.end()) {
        move = true;

        //
        if ((*it)->collision(this->character_box, dt)) {
            this->player_got_shot = true;
            move = false;
        }

        // Check collision with obstacles
        for (Rect* obstacle: *this->obstacles)
            if ((*it)->collision(obstacle, dt))
                move = false;

        // Check collision with enemies
        auto enemy = this->enemies->begin();
        while (enemy != this->enemies->end()) {
            if ((*it)->collision((*enemy)->get_collision_rect(), dt)) {
                enemy = this->enemies->erase(enemy);
                move = false;
            }
            else {
                enemy++;
            }
        }

        // Remove or go to next gunshot
        if (move) {
            (*it++)->move(dt);
        }
        else {
            delete *it;
            it = this->bullets.erase(it);
        }
    }
}

void Arena::add_gunshoot(Gunshot* bullet) {
    this->bullets.push_back(bullet);
}

void Arena::draw() {
    for (Enemy* enemy: *this->enemies) {
        enemy->draw();
    }

    for (Rect* obstacle: *this->obstacles) {
        glPushMatrix();
            obstacle->draw_from_middle();
        glPopMatrix();
    }

    for (Gunshot* bullet: bullets) {
        glPushMatrix();
            bullet->draw();
        glPopMatrix();
    }
}

void Arena::process(double dt) {
    //
    this->move_bullets(dt);

    //
    if (character_box->get_x() + character_box->get_width()/2 >= this->world_width) {
        this->game_won = true;
    }

    //
    for (auto character: *this->enemies)
        this->gravity(character, dt);

    //
    for (Enemy* enemy: *this->enemies) {
        enemy->increase_without_shooting(dt/1000);

        //
        if (this->enemy_walk) {
            if (!enemy->get_falling_down() && !enemy->get_updated_rect()) {
                for (Rect* obj: *this->obstacles) {
                    if (enemy->get_collision_rect()->collision(obj, 0, -velocity/120)) {
                        enemy->set_collided_rect(obj);
                        break;
                    }
                }
            }
            else if (enemy->get_updated_rect()) {
                if (enemy->at_the_end()) {
                    bool to_right = enemy->get_to_right();
                    enemy->set_to_right(!to_right);
                }

                //
                if (enemy->get_to_right()) {
                    move_right(enemy, dt);
                }
                else {
                    move_left(enemy, dt);
                }

                if (!enemy->get_walking_flag()) {
                    enemy->set_to_right(!enemy->get_to_right());
                }
            }
        }
        else {
            enemy->set_idle(true, dt);
        }

        if (this->enemy_gunshot) {
            // Get character position
            int quadrant;

            // Find angle and quadrant
            float angle = atan2(
                (character_box->get_y() + character_box->get_height()*0.85)
                - (enemy->get_y() + enemy->get_height()/1.35),
                character_box->get_x() - enemy->get_x()) * 180/M_PI;
            normalize_angle(angle, quadrant);

            // Update arm angle and character direction
            enemy->set_arm_angle(angle);

            float arm_x, arm_y;
            enemy->get_gun_position(arm_x, arm_y);

            if (enemy->get_without_shooting() >= ((double)rand()/RAND_MAX)*7.0 + 3.0) {
                enemy->set_idle(true, dt);
                enemy->set_direction(quadrant, enemy->get_idle_state());

                //
                float arm_x, arm_y;
                enemy->get_gun_position(arm_x, arm_y);
                
                //
                Rect* gun = enemy->get_gun_rect();
                float width  = gun->get_width();
                float height = gun->get_height();

                this->add_gunshoot(
                    new Gunshot(
                        arm_x, arm_y,
                        width/1.5, height/1.2,
                        enemy->get_arm_angle(),
                        this->velocity*2
                    )
                );

                enemy->clear_without_shooting();
            }
        }
    }
}

void Arena::set_enemies_collision_box(bool value) {
    for (Enemy* enemy: *this->enemies) {
        enemy->set_collision_box(value);
    }
}

void Arena::move_right(Enemy* enemy, double dt) {
    float px, py;

    if (!collision(enemy, this->velocity*dt, 0, px, py)) {
        enemy->move(this->velocity*dt, 0, dt);
        enemy->set_walking_flag(true);
    }
    else {
        enemy->set_idle(true, dt);
        enemy->set_x(px - enemy->get_width()/1.999);
        enemy->set_walking_flag(false);
    }
}

void Arena::move_left(Enemy* enemy, double dt) {
    float px, py;

    if (!collision(enemy, -this->velocity*dt, 0, px, py)) {
        enemy->move(-this->velocity*dt, 0, dt);
        enemy->set_walking_flag(true);

    }
    else {
        enemy->set_idle(true, dt);
        enemy->set_x(px + enemy->get_width()/1.999);
        enemy->set_walking_flag(false);
    }
}

bool Arena::get_player_got_shot() {
    return this->player_got_shot;
}

void Arena::set_player_got_shot(bool value) {
    this->player_got_shot = value;
}

bool Arena::get_game_won() {
    return this->game_won;
}

void Arena::set_enemy_walk(bool value) {
    this->enemy_walk = value;
}

void Arena::set_enemy_gunshot(bool value) {
    this->enemy_gunshot = value;
}
