#pragma once
#include "GLFW/glfw3.h"
int w = 1280, h = 1024;
GLFWwindow* window;
double epsilon = 0.0001;
double PI = 3.1415;
double scale;
double min(double a, double b) { return a < b ? a : b; }
double max(double a, double b) { return a > b ? a : b; }
double mod(double val, double by)
{
    while (val > by)val -= by;
    while (val < 0)val += by;
    return val;
}