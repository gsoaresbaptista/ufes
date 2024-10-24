#ifndef MENU_HPP
#define MENU_HPP

// #include <iostream>
#include <string>
#include <vector>

#include "../utils/shapes/triangle.hpp"
#include "../utils/shapes/rect.hpp"

class GameMenu {
    private:
        std::string name;
        float x;
        float y;
        float scale;
        float body_height;
        int fps;
        bool droplist_on;
        bool draw_menu;
        //
        Triangle* toggle_icon_on;
        Triangle* toggle_icon_off;
        Rect* menu_header;
        Rect* menu_body;
        Rect* checked_box;
        Rect* toggle_icon_rect;
        //
        std::vector<std::string> checkbox_labels;
        std::vector<Rect*> checkbox_boxes;
        std::vector<bool> checkbox_states;
        std::vector<bool> checkbox_initial_states;
    public:
        GameMenu(std::string name, float scale);
        ~GameMenu();
        void draw();
        bool check_click(int x, int y);

        void draw_menu_header();
        void draw_menu_body();
        void draw_fps_counter();
        void draw_checkbox(int id);

        bool get_checkbox_state(int index);
        void fps_counter();
        void toggle_draw_menu();
        bool get_draw_menu();
        void create_checkboxes();
        void create_menu_body();
        void create_menu_icon();
        void create_checkbox(std::string value, bool initial_state);
        void reset();

};

#endif
