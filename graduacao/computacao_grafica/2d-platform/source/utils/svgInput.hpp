#ifndef SVG_READER_HPP
#define SVG_READER_HPP

#include "libs/tinyxml2.h"
#include "../utils/shapes/rect.hpp"
#include "../utils/shapes/circle.hpp"
#include "../models/enemy.hpp"

#include <vector>
#include <string>

class SVGInput{
    private:
        float world_x;
        float world_y;

        void normalize_data();
        void add_rect(tinyxml2::XMLElement* element);
        void add_enemy(tinyxml2::XMLElement* element);
    public:
        float world_width;
        float world_height;
        float camera_size;
        float window_height;
        float window_width;
        float character_x;
        float character_y;
        float character_height;
        std::vector<Rect*>* obstacles;
        std::vector<Enemy*>* enemies;

        SVGInput();
        ~SVGInput();
        void read_svg(std::string svg_path);

        //
        void set_window_width(float width);
        void set_window_height(float height);

        // 
        float get_world_width();
        float get_world_height();
        float get_camera_size();
        float get_character_x();
        float get_character_y();
        float get_character_height();
        float get_window_height();
        float get_window_width();
        std::vector<Rect*>* get_obstacles();
        std::vector<Enemy*>* get_enemies();
};

#endif