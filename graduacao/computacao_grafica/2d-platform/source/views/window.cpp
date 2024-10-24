#include "window.hpp"
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <time.h>

#include "../controllers/gameController.hpp"
#include "../models/game.hpp"
#include "gameMenu.hpp"

GameController* controller = new GameController();
Game* game = Game::get_instance();
GameMenu* menu = nullptr;

Window::Window(const char* title, int width, int height) {
    this->width = width;
    this->height = height;
    this->title = new string(title);
}

Window::~Window() {
}

void Window::set_projection(float camera_size, float x) {
    // Set world's coordinate system in bottom left corner
    glMatrixMode(GL_PROJECTION);
        glOrtho(0, camera_size, 0, camera_size, 0, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(camera_size/2-x, 0, 0);
}

void Window::draw_display() {
     // Clear the screen.
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0f, 0.0f, 1.0f, 1.0);
    // Draw game
    game->draw();
    menu->draw();
    glutSwapBuffers();
}

void Window::init(SVGInput* svg_data){
    int argc = 1;
    glutInit(&argc, NULL);

    glutSetOption(GLUT_MULTISAMPLE, 16);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    glHint(GL_MULTISAMPLE_FILTER_HINT_NV, GL_NICEST);
    glEnable(GL_MULTISAMPLE);

    // Create the window.
    glutInitWindowSize(this->width, this->height);
    // Open on second monitor
    glutCreateWindow(this->title->c_str());

    this->set_projection(
        svg_data->camera_size,
        svg_data->character_x
    );

    //
    svg_data->window_height = height;
    svg_data->window_width = width;
    menu = new GameMenu(
        "Settings",
        svg_data->camera_size/(float)this->width
    );

    controller->init(svg_data, menu);
    glutDisplayFunc(this->draw_display);
    glutMainLoop();
}
