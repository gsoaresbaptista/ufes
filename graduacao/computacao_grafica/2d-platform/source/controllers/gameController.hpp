#ifndef GAME_CONTROLLER_HPP
#define GAME_CONTROLLER_HPP

#include "../utils/svgInput.hpp"
#include "../views/gameMenu.hpp"

class GameController {
    private:
    public:
        GameController();
        void init(SVGInput* svg_data, GameMenu* menu);
        static void key_down(unsigned char key, int x, int y);
        static void key_up(unsigned char key, int x, int y);
        static void mouse_movement(int x, int y);
        static void mouse_click(int button, int state, int x, int y);
        static void process_controllers(int value);
};

#endif
