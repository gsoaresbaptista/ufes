#ifndef MATH_AUX_HPP
#define MATH_AUX_HPP

float normalize_window_position(int position, float camera_size, float dimension);
void RotatePoint(float x, float y, float angle, float &xOut, float &yOut);
void normalize_angle(float& angle, int& quadrant);
void hsv_to_rgb(double h, float s, float v, float &r, float& g, float& b);

#endif