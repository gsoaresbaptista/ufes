#include "gameMenu.hpp"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <cmath>

static char fps_text[10];
void * font = GLUT_BITMAP_8_BY_13;
void * helvetica = GLUT_BITMAP_HELVETICA_12;

int initial_time= time(NULL), final_time, frame_count = 0;
float body_height = 0;

GameMenu::GameMenu(std::string name, float scale) {
    this->fps = 0;
    this->name = name;
    this->x = 10;
    this->y = 500 - 10 - 23 - 15;
    this->scale = scale;
    this->droplist_on = false;
    this->draw_menu = true;
    
    //
    this->create_checkboxes();
    this->create_menu_body();
    this->create_menu_icon();
}

GameMenu::~GameMenu() {
}

void GameMenu::create_menu_body() {
    this->menu_header = new Rect(
        (this->x+85)*scale, (this->y)*scale,
        0, 170*scale, 23*scale, 0.71,0.71,0.71
    );
    this->menu_body = new Rect(
        (this->x+85)*scale, (this->y-body_height)*scale,
        0, 170*scale, body_height*scale, 0.63,0.63,0.63
    );
}

void GameMenu::create_menu_icon() {
    this->toggle_icon_off = new Triangle(
        75*scale, 0,
        (-10)*scale, (6.5-10)*scale, (13-10)*scale,
        (13+3)*scale, (3+3)*scale, (13+3)*scale,
        0.31, 0.31, 0.31
    );
    this->toggle_icon_on = new Triangle(
        75*scale, 0,
        (-10)*scale, (6.5-10)*scale, (13-10)*scale,
        (3+3)*scale, (13+3)*scale, (3+3)*scale,
        0.31, 0.31, 0.31
    );
    this->checked_box = new Rect(
        0, 0, 0, 9*scale, 9*scale, 0.50, 0.80, 0.57
    );
    //
    this->toggle_icon_rect = new Rect(
        toggle_icon_on->get_x() + this->menu_header->get_width()/2,
        toggle_icon_on->get_y() + this->menu_header->get_y(),
        0, 13*scale, 13*scale
    );
}

void GameMenu::create_checkbox(std::string value, bool initial_state) {
    this->checkbox_labels.push_back(value);
    this->checkbox_initial_states.push_back(initial_state);
    this->checkbox_states.push_back(initial_state);
    //
    int qtd = this->checkbox_boxes.size() + 1;
    this->body_height += (22);
    //
    this->checkbox_boxes.push_back(
        new Rect(25*scale, (450 - qtd*20)*scale,
        0, 15*scale, 15*scale, 0.31, 0.31, 0.31)
    );
}

void GameMenu::create_checkboxes() {
    this->create_checkbox("Player box", false);
    this->create_checkbox("Enemy box", false);
    this->create_checkbox("Gun aim", false);
    this->create_checkbox("Enemy Walk", true);
    this->create_checkbox("Enemy gunshot", true);
    this->create_checkbox("Player collision", true);
}

void GameMenu::draw_fps_counter() {
    glPushMatrix();
        glColor3f(1, 1, 1);
        glRasterPos2f(13*scale, (500-13-5)*scale);
        sprintf(fps_text, "FPS: %d", this->fps);
        char *tmpStr = fps_text;
        while(*tmpStr) glutBitmapCharacter(helvetica, *tmpStr++);
    glPopMatrix();
}

void GameMenu::draw_menu_header() {
    glPushMatrix();
        this->menu_header->draw_from_middle();
        glColor3f(0.08, 0.08, 0.08);
        glRasterPos2f((-75)*scale, (7)*scale);
        const char *tmpStr = this->name.c_str();
        while(*tmpStr) glutBitmapCharacter(font, *tmpStr++);
        //
        if (this->droplist_on) this->toggle_icon_on->draw();
        else this->toggle_icon_off->draw();
    glPopMatrix();
}

void GameMenu::draw_menu_body() {
    glPushMatrix();
        if (this->droplist_on) {
            this->menu_body->draw_from_middle();

            for (int i = 0; i < this->checkbox_boxes.size(); i++)
                this->draw_checkbox(i);
        }
    glPopMatrix();
}

void GameMenu::draw_checkbox(int id) {
    glPushMatrix();
        glLoadIdentity();
        this->checkbox_boxes[id]->draw_from_middle();
        // Draw checked icon
        if (this->checkbox_states[id]) {
            glTranslatef(0*scale, 3*scale, 0);
            this->checked_box->draw_from_middle();
            glTranslatef(0*scale, -3*scale, 0);
        }
        // Draw label
        glColor3f(0.08, 0.08, 0.08);
        glRasterPos2f(12*scale, 2.5*scale);
        const char *tmpStr = this->checkbox_labels[id].c_str();
        while(*tmpStr) glutBitmapCharacter(font, *tmpStr++);
    glPopMatrix();
}

void GameMenu::draw() {
    if (this->draw_menu) {
        glPushMatrix();
            glLoadIdentity();
            draw_fps_counter();
            this->draw_menu_header();
            this->draw_menu_body();
        glPopMatrix();
    }
}

bool GameMenu::check_click(int x, int y) {
    bool state = false;
    x *= this->scale;
    y = 500*scale - y*this->scale;
    Rect* click_rect = new Rect(x, y, 0, 1*scale, 1*scale);


    if (this->menu_header->collision(click_rect, 0, 0)) {
        state = true;
        if (this->toggle_icon_rect->collision(click_rect, 0, 0))
            this->droplist_on = (droplist_on) ? false:true;
    }

    if (this->droplist_on) {
        click_rect->set_y(y + 5*scale);
        for (int i = 0; i < this->checkbox_boxes.size(); i++) {

            if (this->checkbox_boxes[i]->collision(click_rect, 0, 0)) {
                this->checkbox_states[i] = (this->checkbox_states[i]) ? false:true;
                state = true;
            }
        }
    }
    return state;
}

bool GameMenu::get_checkbox_state(int index) {
    return this->checkbox_states[index];
}

void GameMenu::fps_counter() {
    frame_count++;
    final_time = time(NULL);
    
    if (final_time - initial_time > 0) {
        this->fps = frame_count/(final_time - initial_time);
        frame_count = 0;
        initial_time = final_time;
    }
}

void GameMenu::toggle_draw_menu() {
    this->draw_menu = (draw_menu) ? false:true;
}

bool GameMenu::get_draw_menu() {
    return this->draw_menu;
}

void GameMenu::reset() {
    for (int i = 0; i < this->checkbox_initial_states.size(); i++)
        this->checkbox_states[i] = this->checkbox_initial_states[i];
    this->draw_menu = true;
    this->droplist_on = false;
}