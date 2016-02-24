#ifndef __SHAPE_H__
#define __SHAPE_H__
#include <vector>
#include <iostream>
#include "rectangle.h"
#define UNUSE -1

class Shape : public Rectangle{

    friend class Shapes;
    friend class Graphs;
    friend class Windows;
    friend class Calculate;

    public:
        Shape (int &lx, int &dy, int &rx, int &uy)
        : Rectangle(lx, dy, rx, uy),
        graphid(UNUSE), color(UNUSE) {

        }
        ~Shape (){}

    private:
        int graphid;
        int color;
        std::vector <int> conflictid;

};

#endif
