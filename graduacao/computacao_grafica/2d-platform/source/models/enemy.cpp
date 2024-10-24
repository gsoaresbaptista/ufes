#include "enemy.hpp"
#include "../utils/mathAux.hpp"
#include <ctime>
#include <cmath>
#include <cstdlib>

#include <iostream>
using namespace std;

Enemy::Enemy(float x, float y, float diameter, float r, float g, float b)
    : Character(x, y, diameter, r, g, b) {
    //
    this->last_collided_rect = NULL;
    this->updated_rect = false;
    this->without_shooting = 0;
}

Enemy::~Enemy(){
}

void Enemy::choose_direction() {
    if (((double) rand() / (RAND_MAX)) >= 0.5) {
        this->set_to_right(false);
    }
}

void Enemy::set_collided_rect(Rect* rect) {
    this->last_collided_rect = rect;
    this->updated_rect = true;
}


void Enemy::process(double dt) {
}

bool Enemy::get_updated_rect() {
    return this->updated_rect;
}

void Enemy::set_falling_down(bool value) {
    Character::set_falling_down(value);
    this->updated_rect = false;
    this->walking = false;
}

bool Enemy::at_the_end() {
    float x = this->last_collided_rect->get_x();
    float width = this->last_collided_rect->get_width();

    if (this->get_x() - this->get_width()/2.0 < x - width/2.0 ||
        this->get_x() + this->get_width()/2.0 > x + width/2.0) {
        //

        if (this->get_to_right()) this->set_x(x + width/2  - this->get_width()/2);
        else this->set_x(x - width/2 + this->get_width()/2);

        return true;
    }

    return false;
}

void Enemy::set_walking_flag(bool value) {
    this->walking = value;
}

bool Enemy::get_walking_flag() {
    return this->walking;
}

void Enemy::set_to_right(bool value) {
    Character::set_to_right(value);
}

Enemy* Enemy::clone(Enemy* p) {
    double golden_ratio = 0.618033988749895;
    
    float r, g, b;
    double h = fmod(golden_ratio + ((double)rand()/((double)RAND_MAX+1)), 1.0);
    hsv_to_rgb(h, 0.75, 0.95, r, g, b);

    Enemy* enemy = new Enemy(
        p->get_x(), p->get_y(),
        p->get_height(),
        r, g, b
    );

    enemy->choose_direction();

    return enemy;
}

double Enemy::get_without_shooting() {
    return this->without_shooting;
}

void Enemy::increase_without_shooting(double dt) {
    this->without_shooting += dt;
}

void Enemy::clear_without_shooting() {
    this->without_shooting = 0;
}
