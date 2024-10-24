#include "mathAux.hpp"
#include <cmath>

void RotatePoint(float x, float y, float angle, float &xOut, float &yOut) {
    angle = M_PI/180 * angle;
    xOut += cos(angle) * x - sin(angle) * y;
    yOut += sin(angle) * x + cos(angle) * y;
}

void normalize_angle(float& angle, int& quadrant) {
    if (angle > 90 || angle < -90) {
        if (angle > 0) {
            angle = 180 - angle;
        }
        else {
            angle =  -180 - angle;
        }
        quadrant = 1;
    }
    else {
        quadrant = 2;
    }
    angle -=10;
}

void hsv_to_rgb(double h, float s, float v, float &r, float& g, float& b) {
    int h_i = (int)(h*6);
    float f = h*6 - h_i;
    float p = v * (1 - s);
    float q = v * (1 - f*s);
    float t = v * (1 - (1 - f) * s);

    if (h_i == 0) {
        r = v; g = t; b = p;
    } else if (h_i == 1) {
        r = p; g = v; b = t;
    } else if (h_i == 2) {
        r = v; g = t; b = p;
    } else if (h_i == 3) {
        r = p; g = q; b = v;
    } else if (h_i == 4) {
        r = t; g = p; b = v;
    } else if (h_i == 5) {
        r = v; g = p; b = q;
    }
}

float normalize_window_position(
    int position,
    float camera_size,
    float dimension
) {
    return position * camera_size/dimension;
}
