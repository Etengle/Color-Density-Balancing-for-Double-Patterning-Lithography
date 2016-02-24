#ifndef __SHAPES_H__
#define __SHAPES_H__
#include "shape.h"
#include <vector>
#include <iostream>

class Shapes {

    friend std::ostream & operator << (std::ostream &, const Shapes &);

    struct Functor_of_Compare_rx {
        Shapes* sp;
        Functor_of_Compare_rx(Shapes* sp) : sp(sp) {};
        bool operator () ( const int & i, const int & j ){return sp->shape[i].right_x < sp->shape[j].right_x;}
    };
    struct Functor_of_Compare_uy {
        Shapes* sp;
        Functor_of_Compare_uy(Shapes* sp) : sp(sp) {};
        bool operator () ( const int & i, const int & j ){return sp->shape[i].up_y < sp->shape[j].up_y;}
    };

    public:
        Shapes (){}
        ~Shapes (){}
        void readFile (int, char**);
        void errorMsg (int, char*);
        void buildConflictArray();
        void findBoundingBox();
        std::vector<Shape>& getShapes();

    private:
        std::vector<Shape> shape;
        std::vector<int> rx, uy;
        void testPrint();
};

#endif
