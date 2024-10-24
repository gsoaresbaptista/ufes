#include "views/window.hpp"
#include "utils/svgInput.hpp"
#include <iostream>

//
#define DEFAULT_PATH "./inputs/original_phase.svg"

using namespace std;

int main(int argc, char **argv) {
    string svg_path;

    if (argc == 1)
        svg_path = DEFAULT_PATH;
    else
        svg_path = argv[1];

    SVGInput svg_data;
    svg_data.read_svg(svg_path);
    Window *window = new Window("Trabalho CG", 500, 500);
    window->init(&svg_data);

    return 0;
}
