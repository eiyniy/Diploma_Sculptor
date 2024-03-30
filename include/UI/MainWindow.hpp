#pragma once

#include <Point.hpp>

class MainWindow
{
public:
    MainWindow(Point &_resolution);

    void clear();

private:
    Point &resolution;
};
