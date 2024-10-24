#include "gameController.hpp"
#include "../models/game.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <iostream>
using namespace std;

//
int frame_rate = 60;
bool pressed_keys[256];

bool mouse_left;
bool mouse_right;
bool gunshot_blocked;

int clicked_x;
int clicked_y;

//
Game* game_model = Game::get_instance();
GameMenu* game_menu = nullptr;

GameController::GameController() {
    frame_rate = 60;
}

static inline void load_menu_settings() {
    game_model->set_player_collision_box(game_menu->get_checkbox_state(0));
    game_model->set_enemies_collision_box(game_menu->get_checkbox_state(1));
    game_model->set_shot_tracing(game_menu->get_checkbox_state(2));
    game_model->set_enemy_walk(game_menu->get_checkbox_state(3));
    game_model->set_enemy_gunshot(game_menu->get_checkbox_state(4));
    game_model->set_player_collision(game_menu->get_checkbox_state(5));
}

void GameController::init(SVGInput* settings, GameMenu* menu) {
    //
    game_model->set_settings(settings);
    game_menu = menu;

    // 
    glutKeyboardFunc(this->key_down);
    glutKeyboardUpFunc(this->key_up);
    glutPassiveMotionFunc(this->mouse_movement);
    glutMotionFunc(this->mouse_movement);
    glutMouseFunc(this->mouse_click);
    glutTimerFunc(1000.0/frame_rate, this->process_controllers, 0);
}

void GameController::mouse_click(int button, int state, int x, int y) {
    clicked_x = x;
    clicked_y = y;

    
    if (button == 0) {
        if (state == 0) {
            if (game_menu->check_click(x, y)) {
                load_menu_settings();
            }
            else {
                mouse_left = true;
            }
        }
        else {
            mouse_left = false;
            gunshot_blocked = false;
        }
    }
    else {
        if (state == 0) {
            mouse_right = true;
        }
        else {
            mouse_right = false;
        }
    }
}

void GameController::process_controllers(int value) {
    //
    game_menu->fps_counter();

    //
    static GLdouble previousTime = glutGet(GLUT_ELAPSED_TIME);
    GLdouble currentTime, timeDiference;
    currentTime = glutGet(GLUT_ELAPSED_TIME);
    timeDiference = currentTime - previousTime;
    previousTime = currentTime;

    //
    if (game_model->get_game_lost() || game_model->get_game_won()) {
        if (game_menu->get_draw_menu()) {
            game_menu->toggle_draw_menu();
        }

        if (pressed_keys['r']) {
            game_menu->reset();
            game_model->restart_game();
            load_menu_settings();
        }

        glutPostRedisplay();
        glutTimerFunc(1000.0/frame_rate, process_controllers, 0);
        
        return;
    }

    //
    if (pressed_keys['d']) {
        game_model->move_right(timeDiference);
    }
    else if (pressed_keys['a']) {
        game_model->move_left(timeDiference);
    } 
    else {
        game_model->set_idle(true, timeDiference);
    }

    
    if (mouse_right) {
        game_model->jump(true, timeDiference);
    }
    else {
        game_model->jump(false, timeDiference);
    }

    if (mouse_left) {
        if (!gunshot_blocked) {
            game_model->shoot();
            gunshot_blocked = true;
        }
    }

    //
    if (pressed_keys['m']) {
        game_menu->toggle_draw_menu();
        pressed_keys['m'] = false;
    }

    //
    game_model->process(timeDiference);

    //
    glutPostRedisplay();
    glutTimerFunc(1000.0/frame_rate, process_controllers, 0);
}

void GameController::key_down(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
        case 'A':
            pressed_keys[(int)('a')] = true;
            break;
        case 'd':
        case 'D':
            pressed_keys[(int)('d')] = true;
            break;
        case 'm':
        case 'M':
            pressed_keys[(int)('m')] = true;
            break;
        // case 'w':
        // case 'W':
        //     pressed_keys[(int)('w')] = true;
        //     break;
        case 'r':
        case 'R':
            pressed_keys[(int)('r')] = true;
            break;
    }
}

void GameController::key_up(unsigned char key, int x, int y) {
    switch (key) {
        case 'a':
        case 'A':
            pressed_keys[(int)('a')] = false;
            break;
        case 'd':
        case 'D':
            pressed_keys[(int)('d')] = false;
            break;
        case 'm':
        case 'M':
            pressed_keys[(int)('m')] = false;
            break;
        // case 'w':
        // case 'W':
        //     pressed_keys[(int)('w')] = false;
        //     break;
        case 'r':
        case 'R':
            pressed_keys[(int)('r')] = false;
            break;
    }
}

void GameController::mouse_movement(int x, int y) {
    //
    if (game_model->get_game_lost() || game_model->get_game_won())
        return;

    game_model->move_arm(x, y);
}
