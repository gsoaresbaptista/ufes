#include "svgInput.hpp"
#include "mathAux.hpp"

#include <cstdlib>
#include <iostream>
#include <cmath>
#include <ctime> 

#define GOLDEN_RATIO 0.618033988749895

using namespace std;
using namespace tinyxml2;

SVGInput::SVGInput() {
    this->world_width = 0;
    this->character_x = 0;
    this->character_height = 0;
    this->obstacles = new vector<Rect*>();
    this->enemies = new vector<Enemy*>();
}

SVGInput::~SVGInput() {
}

static inline void set_color(float& r, float& g, float& b) {
    double h = fmod(GOLDEN_RATIO + ((double)rand()/RAND_MAX), 1.0);
    hsv_to_rgb(h, 0.75, 0.95, r, g, b);
}

void SVGInput::read_svg(string svg_path) {
    // Set random seed to color generation
    srand(time(NULL));

    // Read SVG
    XMLDocument svg;
	XMLError error = svg.LoadFile(svg_path.c_str());

    // Verify svg file is correct
    if (error != XML_SUCCESS) {
        cout << "\033[0;31m" << "ERROR: " << "\033[0m" 
            << "Incorrect svg file informed!" << endl;
        exit(0);
    }

    XMLElement* element = svg.FirstChildElement("svg")->FirstChildElement();
    
    while (element != NULL) {
        string element_type(element->Name());

        if (element_type == "rect")
            this->add_rect(element);
        else if (element_type == "circle")
            this->add_enemy(element);
        
        element = element->NextSiblingElement();
    }

    this->normalize_data();

    // Success message
    cout << "\033[0;32m" << "SUCCESS: " << "\033[0m" 
            << "Map loaded!" << endl;
}

void SVGInput::add_rect(XMLElement* element) {
    float x, y, width, height;
    element->QueryFloatAttribute("x", &x);
    element->QueryFloatAttribute("y", &y);
    element->QueryFloatAttribute("width", &width);
    element->QueryFloatAttribute("height", &height);
    
    //
    if (!strcmp(element->Attribute("fill"), "blue")) {
        this->world_width = width;
        this->world_height = height;
        this->camera_size = min(width, height);
        this->world_x = x;
        this->world_y = y;
        return;
    }

    this->obstacles->push_back(
        new Rect(x, y, 0, width, height, 0, 0, 0, 1)
    );
}

void SVGInput::add_enemy(XMLElement* element) {
    float cx, cy, radius, r, g, b;
    element->QueryFloatAttribute("cx", &cx);
    element->QueryFloatAttribute("cy", &cy);
    element->QueryFloatAttribute("r", &radius);
    
    //
    if (!strcmp(element->Attribute("fill"), "green")) {
        this->character_height = 2*radius;
        this->character_x = cx;
        this->character_y = cy - radius;
        return;
    }

    set_color(r, g, b);
    this->enemies->push_back(
        new Enemy(cx, cy - radius, 2*radius, r, g, b) 
    );
}

void SVGInput::normalize_data() {
    for (Rect* rect: *this->obstacles) {
        rect->set_x(rect->get_x() - world_x + rect->get_width()/2);
        rect->set_y(world_height - rect->get_y() + world_y - rect->get_height());
    }

    for (Enemy* enemy: *this->enemies) {
        enemy->set_x(enemy->get_x() - world_x);
        enemy->set_y(world_height - enemy->get_y() + world_y - enemy->get_height());
    }

    this->character_x = this->character_x - world_x;
    this->character_y = world_height - this->character_y + world_y - this->character_height;
}
