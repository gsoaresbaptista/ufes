#include "game.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <cmath>
#include "../utils/mathAux.hpp"
#include "../utils/shapes/rect.hpp"

// Pointer to game singleton
static Game* game_ = nullptr;

Game* Game::get_instance() {
    if (game_ == nullptr) {
        game_ = new Game();
    }
    return game_;
}

Game::Game() {
    this->character = NULL;
    this->shot_trajectory_on = false;
    this->arena = NULL;
    this->game_lost = false;
    this->game_won = false;
}

void Game::set_settings(SVGInput* svg_data) {
    this->default_game_settings = svg_data;
    this->window_width = svg_data->window_width;
    this->window_height = svg_data->window_height;
    this->velocity = 0.002 * svg_data->character_height;
    this->camera_size = svg_data->camera_size;

    // Create character
    if (this->character == NULL) {
        this->character = new Character(
            svg_data->character_x,
            svg_data->character_y,
            svg_data->character_height
        );
    }

    //
    if (this->arena == NULL) {
        this->arena = new Arena(
            this->character->get_collision_rect(),
            this->velocity);
        this->arena->set_settings(svg_data);
    }
}

void Game::draw() {
    this->character->draw();
    this->arena->draw();

    if (this->shot_trajectory_on)
        this->draw_shot_trajectory();

    //
    if (this->game_lost || this->game_won) {
        this->draw_end_game();
    }
}

void Game::draw_end_game() {
    glPushMatrix();
        glLoadIdentity();

        glPushMatrix();
            Rect(camera_size/2, 0, 0,
                camera_size, camera_size,
                0, 0, 0, 0.75
            ).draw_from_middle();
        glPopMatrix();

        glPushMatrix();
            if (this->game_lost) {
                glColor4f(1, 0.25, 0.25, 1);
                glTranslatef((250-100)*(camera_size/500), (250)*(camera_size/500), 0);
                glScalef(0.3*(camera_size/500),0.3*(camera_size/500), 1);
                glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"YOU LOST!");
            } else {
                glColor4f(0.46667, 0.86667, 0.46667, 1);
                glTranslatef((250-100)*(camera_size/500), (250)*(camera_size/500), 0);
                glScalef(0.3*(camera_size/500),0.3*(camera_size/500), 1);
                glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"YOU WON!");
            }
        glPopMatrix();

        glPushMatrix();
            glColor4f(1, 1, 1, 0.8);
            glTranslatef((250-70)*(camera_size/500), (220)*(camera_size/500), 0);
            glScalef(0.1*(camera_size/500),0.1*(camera_size/500), 1);
            glutStrokeString(GLUT_STROKE_ROMAN, (unsigned char*)"PRESS R TO RESTART");
        glPopMatrix();
    glPopMatrix();
}

inline void Game::draw_shot_trajectory() {
    float x_out, y_out;
    this->character->get_gun_position(x_out, y_out);

    glPushMatrix();
        glLineWidth(0.8);
        glColor3f(1,0,0);
        glBegin(GL_LINES);
            glVertex2f(x_out, y_out);
            glVertex2f(gun_x - camera_size/2 + character->get_x(), gun_y);
        glEnd();
    glPopMatrix();
}

void Game::move_right(double dt) {
    float px, py;

    int collided_type = arena->collision(
        character, velocity*dt, 0,
        px, py
    );

    if (!collided_type) {
        this->character->move(this->velocity*dt, 0, dt);
        // Update Camera view
        glTranslatef(-this->velocity*dt, 0, 0);
        this->move_arm(this->gun_x, this->gun_y);
    }
    else {
        this->set_idle(true, dt);
        if (collided_type != 3)
            this->character->set_x(px - this->character->get_width()/1.99);
    }
}

void Game::move_left(double dt) {
    float px, py;

    int collided_type = arena->collision(
        character, -velocity*dt, 0,
        px, py
    );

    if (!collided_type) {
        this->character->move(-this->velocity*dt, 0, dt);
        // Update Camera view
        glTranslatef(this->velocity*dt, 0, 0);
        this->move_arm(this->gun_x, this->gun_y);
    }
    else {
        this->set_idle(true, dt);
        if (collided_type != 3)
            this->character->set_x(px + this->character->get_width()/1.99);
    }
}

void Game::move_arm(int x, int y) {
    // Normalize mouse position
    this->gun_x = normalize_window_position(x, camera_size, window_width);
    this->gun_y = normalize_window_position((500 - y), camera_size, window_height);
    this->move_arm(this->gun_x, this->gun_y);
}

void Game::move_arm(float x, float y) {
    // Get character position
    float cy = this->character->get_y() + this->character->get_height()/1.35;
    int quadrant;

    // Find angle and quadrant
    float angle = atan2(this->gun_y - cy, this->gun_x - this->camera_size/2.0) * 180/M_PI;
    normalize_angle(angle, quadrant);

    // Update arm angle and character direction
    this->character->set_arm_angle(angle);

    // Defines the character's direction when inactive
    this->character->set_direction(quadrant, this->character->get_idle_state());
}

void Game::jump(bool active, double dt) {
    if (active && !character->get_falling_down()) {
        float delta_y = velocity*(32.5 - character->get_rising_time()/50);

        if (!arena->collision(character, 0, delta_y) && (delta_y > 0)) {
            //
            character->move(0, delta_y, 0);
            character->set_jumping(true);
            character->increase_rising_time(dt);
        }
        else {
            character->set_jumping(false);
        }
    }
    else {
        character->clear_rising_time();
        character->set_jumping(false);
    }
}

void Game::shoot() {
    if (this->game_won || this->game_lost)
        return;

    Rect* gun = this->character->get_gun_rect();
    float arm_x, arm_y;
    float width  = gun->get_width();
    float height = gun->get_height();
    this->character->get_gun_position(arm_x, arm_y);

    this->arena->add_gunshoot(
        new Gunshot(
            arm_x, arm_y,
            width/1.5, height/1.2,
            this->character->get_arm_angle(),
            this->velocity*2
        )
    );
}

void Game::set_idle(bool state, double dt) {
    this->character->set_idle(state, dt);
}

void Game::set_player_collision(bool value) {
    this->character->set_collision_flag(value);
    this->arena->set_collision(value);
}

void Game::set_player_collision_box(bool value) {
    this->character->set_collision_box(value);
}

void Game::set_enemies_collision_box(bool value) {
    this->arena->set_enemies_collision_box(value);
}

void Game::set_shot_tracing(bool value) {
    this->shot_trajectory_on = value;
}

void Game::process(double dt) {
    if (!this->game_lost && !this->game_won) {
        //
        this->arena->gravity(this->character, dt);
        this->move_arm(this->gun_x, this->gun_y);

        //
        this->character->update_collision_rect();
        this->arena->process(dt);

        //
        if (this->arena->get_player_got_shot()) {
            if (!this->character->get_collision_flag()) {
                this->arena->set_player_got_shot(false);
                this->game_lost = false;
            }
            else {
                this->game_lost = true;
            }
        }
        else if (this->arena->get_game_won()) {
            this->game_won = true;
        }
    }
}

bool Game::get_game_lost() {
    return this->game_lost;
}

bool Game::get_game_won() {
    return this->game_won;
}

void Game::restart_game() {
    this->game_lost = false;
    this->game_won = false;
    
    //
    delete this->character;
    this->character = new Character(
            default_game_settings->character_x,
            default_game_settings->character_y,
            default_game_settings->character_height
        );

    //
    delete this->arena;
    this->arena = new Arena(
        this->character->get_collision_rect(),
        this->velocity);

    this->arena->set_settings(this->default_game_settings);

    glLoadIdentity();
    glTranslatef(camera_size/2-this->character->get_x(), 0, 0);
}

void Game::set_enemy_walk(bool value) {
    this->arena->set_enemy_walk(value);
}

void Game::set_enemy_gunshot(bool value) {
    this->arena->set_enemy_gunshot(value);
}