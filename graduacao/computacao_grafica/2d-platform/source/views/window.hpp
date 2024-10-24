#ifndef WINDOW_HPP
#define WINDOW_HPP

#include "../utils/svgInput.hpp"
#include <string>

class Window {
    private:
        //
        int height;
        int width;
        std::string* title;

        //
        static void draw_display();
        static void set_projection(float world_width, float x);
    public:
        Window(const char* title, int width, int height);
        ~Window();
        void init(SVGInput* settings);
};

#endif
