#pragma once

struct Color
{
    Color() = default;
    Color(double r, double g, double b, double a);

    double r;
    double g;
    double b;
    double a;
};

Color::Color(double r, double g, double b, double a) : r{r}, g{g}, b{b}, a{a}
{
}