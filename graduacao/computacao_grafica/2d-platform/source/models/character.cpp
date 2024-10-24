#include "character.hpp"
#include "../utils/mathAux.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include<cmath>

#define WHITE 1.0, 1.0, 1.0
#define GRAY 0.31, 0.31, 0.31

// Animations angles
static int id_frame = 0;
static float (*animation)[5];

static float idle_animation[6][5] = {
    {0.00, 30, -30, -10, -10},
    {0.03, 30, -30, -10, -10},
    {0.06, 30, -30, -10, -10},
    {0.09, 30, -30, -10, -10},
    {0.06, 30, -30, -10, -10},
    {0.03, 30, -30, -10, -10},
};

static float walking_animation[10][5] = {
    {0.00, -20, -20,  30,   0},
    {0.03, -10, -30,  30, -20},
    {0.06, -10, -40,  20, -20},
    {0.09,   0, -60,  10, -25},
    {0.12,  15, -70,   5, -15},
    {0.15,  30, -80,   0, -10},
    {0.12,  40, -60,  -5, -10},
    {0.09,  45, -50, -10, -10},
    {0.06,  40, -25, -15, -15},
    {0.03,  35, -10, -20, -20},
};

// Static functions
inline static void draw_leg(Rect* thigh, Rect* calf);

Character::Character(float x, float y, float diameter, float r, float g, float b) {
    this->x = x;
    this->y = y;
    this->current_frame = 0;
    this->arm_angle = 0;
    this->to_right = true;
    this->idle_state = true;
    this->height = diameter;
    this->collision_box = false;
    this->collision_flag = true;

    // Create character proportionally
    this->create_character(
        0.6 * diameter,
        0.4 * diameter,
        r, g, b
    );

    //
    this->collision_rect = new Rect(
        x, y, 0,
        height/3.1, height,
        1.0, 0.0, 0.0
    );
}

inline void Character::create_character(float head_body, float legs, float r, float g, float b) {
    this->body = new Rect(0, -0.85*head_body, 0.0, 0.1*head_body, 0.9*head_body, r, g, b);
    this->head = new Circle(0, 0, 0.15*head_body, r, g, b);
    // Create legs
    this->left_thigh = new Rect(0, -0.50*legs, 30.0, 0.16*legs, 0.53*legs, r, g, b);
    this->left_calf = new Rect(0, -0.60*legs, -30.0,  0.15*legs, 0.622*legs, r, g, b);
    this->right_thigh = new Rect(0, -0.50*legs, -10.0, 0.16*legs, 0.53*legs, r, g, b);
    this->right_calf = new Rect(0, -0.60*legs, -10.0, 0.15*legs, 0.622*legs, r, g, b);
    // Create arm
    this->arm = new Rect(0, head_body/2, -10, 0.5*legs, 0.13*legs, r, g, b);
    this->forearm = new Rect(0.48*legs, 0, 10, 0.4*legs, 0.13*legs, r, g, b);
    // Create gun
    this->gun = new Rect(0.6*legs, 0.065*legs, 0, 0.4*legs, 0.13*legs, GRAY);
    this->gun_support = new Rect(-0.08*legs, 0.065*legs, 90, 0.16*legs, 0.13*legs, GRAY);
}

Rect* Character::get_gun_rect() {
    return this->gun;
}

float Character::get_height() {
    return this->height;
}

float Character::get_x() {
    return this->x;
}

float Character::get_y() {
    return this->y;
}

void Character::next_frame(double dt) {
    if (((this->idle_state && (this->jumping || this->falling_down)) &&
          this->current_frame + 60.0*3*dt/1000 > 100.0) ||
        ((this->idle_state && !this->jumping && !this->falling_down) &&
          this->current_frame + 60.0*3*dt/1000 > 60.0) ||
        (!this->idle_state && this->current_frame + 60.0*3*dt/1000 > 100.0)) {
        //
        this->current_frame = 0;
    }
    else {
        this->current_frame += 60.0*2*dt/1000;
    }
    update_angles();
}

void Character::set_idle(bool state, double dt) {
    this->idle_state = state;
    this->next_frame(dt);
}

inline void Character::set_angles(
        float head_offset,
        float left_thigh,
        float left_calf,
        float right_thigh,
        float right_calf) {
    this->head_offset = head_offset;
    this->left_thigh->set_angle(left_thigh);
    this->left_calf->set_angle(left_calf);
    this->right_thigh->set_angle(right_thigh);
    this->right_calf->set_angle(right_calf);
}

bool Character::get_idle_state() {
    return this->idle_state;
}

void Character::set_direction(int quadrant, bool falling) {
    // Update character direction
    if (quadrant == 1 && (this->idle_state || falling)) {
        this->to_right = false;
    }
    else if (quadrant == 2 && (this->idle_state || falling)) {
        this->to_right = true;
    }
}

float Character::get_arm_angle() {
    if (this->to_right)
        return this->arm_angle+10;

    return 180-this->arm_angle-10;
}

void Character::set_arm_angle(float angle) {
    // Update arm angle
    if (angle >= 45) {
        this->arm_angle = 45;
    }
    else if (angle <= -45) {
        this->arm_angle = -45;
    }
    else {
        this->arm_angle = angle;
    }
}

void Character::update_angles() {
    int ln, y0, y1;
    id_frame = (int)(this->current_frame);

    if (this->idle_state && !this->jumping && !this->falling_down) {
        animation = idle_animation;
        ln = id_frame % 6;
        y0 = id_frame - ln;
        y1 = id_frame + 6 - ln;
    }
    else {
        animation = walking_animation;
        ln = id_frame % 10;
        y0 = id_frame - ln;
        y1 = id_frame + 10 - ln;
    }

    this->set_angles(
        ((id_frame-y0)/(y1-y0))*(animation[y1/10][0]-animation[y0/10][0]) + animation[y0/10][0],
        ((id_frame-y0)/(y1-y0))*(animation[y1/10][1]-animation[y0/10][1]) + animation[y0/10][1],
        ((id_frame-y0)/(y1-y0))*(animation[y1/10][2]-animation[y0/10][2]) + animation[y0/10][2],
        ((id_frame-y0)/(y1-y0))*(animation[y1/10][3]-animation[y0/10][3]) + animation[y0/10][3],
        ((id_frame-y0)/(y1-y0))*(animation[y1/10][4]-animation[y0/10][4]) + animation[y0/10][4]
    );
    // Update arm angle
    this->arm->set_angle(this->arm_angle);
}

inline static void draw_leg(Rect* thigh, Rect* calf) {
    glPushMatrix();
        thigh->draw_from_middle();
        calf->draw_from_middle();
    glPopMatrix();
}

void Character::draw_collision_box() {
    glPushMatrix();
        glTranslatef(-this->x, -this->y-this->height, 0);
        this->get_collision_rect()->draw_lines();
    glPopMatrix();
}

void Character::set_y(float y) {
    this->y = y;
    this->collision_rect->set_y(y);
}

void Character::set_x(float x) {
    this->x = x;
    this->collision_rect->set_x(x);
}

void Character::draw_arm() {
    glPushMatrix();
        this->arm->draw_from_left();
        this->forearm->draw_from_left();
        this->gun->draw_from_middle();
        this->gun_support->draw_from_middle();
    glPopMatrix();
}

void Character::get_gun_position(float& x, float& y) {
    x = this->x;
    int side = (this->to_right) ? 1:-1;
    y = this->y + this->head_offset + this->height - this->arm->get_y();

    float angle = side*this->arm->get_angle();
    RotatePoint(side*this->arm->get_width() , 0, angle, x, y);
    angle += side*this->forearm->get_angle();
    RotatePoint(side*this->forearm->get_width(), 0, angle, x, y);
    angle += side*this->gun->get_angle();
    RotatePoint(side*this->gun->get_width(), this->gun->get_height(), angle, x, y);
}

void Character::draw() {
    glPushMatrix();
        // Set position
        glTranslatef(this->x, this->height + this->y + this->head_offset, 0);
        if (!this->to_right) glScalef(-1, 1, 1);
        if (this->collision_box) this->draw_collision_box();

        // Start character draw
        this->head->draw();

        glTranslatef(0, -this->head_offset, 0);
            this->body->draw_from_middle();
        glTranslatef(0, this->head_offset, 0);

        this->draw_arm();

        // Idle animation
        glTranslatef(0, -this->head_offset, 0);

        // Draw legs
        draw_leg(this->left_thigh, this->left_calf);
        draw_leg(this->right_thigh, this->right_calf);
    glPopMatrix();
}

void Character::move(float x, float y, double dt) {
    //
    this->idle_state = false;
    this->next_frame(dt);
    this->x += x;
    this->y += y;
    if (x > 0) this->to_right = true;
    else if (x < 0) this->to_right = false;
}

bool Character::collision(Rect* rect, float delta_x, float delta_y, float& px, float& py) {
    return this->get_collision_rect()->collision(rect, delta_x, delta_y, px, py);
}

bool Character::collision(Rect* rect, float delta_x, float delta_y) {
    return this->get_collision_rect()->collision(rect, delta_x, delta_y);
}

Rect* Character::get_collision_rect() {
    this->update_collision_rect();
    return this->collision_rect;
}

void Character::set_collision_box(bool value) {
    this->collision_box = value;
}

float Character::get_width() {
    return this->get_collision_rect()->get_width();
}

bool Character::get_jumping() {
    return this->jumping;
}

bool Character::get_falling_down() {
    return this->falling_down;
}

double Character::get_falling_time() {
    return this->falling_time;
}

double Character::get_rising_time() {
    return this->rising_time;
}

void Character::set_jumping(bool value) {
    this->jumping = value;
}

void Character::set_falling_down(bool value) {
    this->falling_down = value;
}

void Character::increase_falling_time(float dt) {
    this->falling_time += dt;
}

void Character::clear_falling_time() {
    this->falling_time = 0;
}

void Character::increase_rising_time(float dt) {
    this->rising_time += dt;
}

void Character::clear_rising_time() {
    this->rising_time = 0;
}

void Character::set_collision_flag(bool value) {
    this->collision_flag = value;
}

bool Character::get_collision_flag() {
    return this->collision_flag;
}

bool Character::get_to_right() {
    return this->to_right;
}

void Character::set_to_right(bool value) {
    this->to_right = value;
}

void Character::update_collision_rect() {
    this->collision_rect->set_x(this->x);
    this->collision_rect->set_y(this->y);
}