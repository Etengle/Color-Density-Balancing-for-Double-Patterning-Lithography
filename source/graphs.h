#ifndef __GRAPHS_H__
#define __GRAPHS_H__
#include "graph.h"
#include "shapes.h"
#include <vector>
#include <iostream>

class Graphs {

    friend std::ostream & operator << (std::ostream &, const Graphs &);

    private:
        std::vector<Graph> graph;
        void testPrint(std::vector<Shape> &);
    public:
        Graphs (){}
        ~Graphs (){}
        void buildConflictGraph(std::vector<Shape> &);
        int markGraphIdAndInitColoring(std::vector<Shape> &,int, int, int);
        void countTotalColorDiff();
        std::vector<Graph>& getGraphs();
};

#endif
