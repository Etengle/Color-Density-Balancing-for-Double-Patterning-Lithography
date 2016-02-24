#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/time.h>
#include "shapes.h"
#include "graphs.h"
#include "windows.h"
#include "calculate.h"
using namespace std;

int x_spacing, y_spacing, windowsize;
int box_lx, box_rx, box_uy, box_dy;	// bounding box

int main(int argc, char **argv) {
    int diff, old_score, new_score;
    struct timeval start;
    gettimeofday(&start,NULL);
    Shapes s;
    s.readFile(argc, argv);
    s.buildConflictArray();
    Graphs g;
    g.buildConflictGraph(s.getShapes());
    s.findBoundingBox();
    Windows w;
    w.createWindow();
    w.countArea(s.getShapes(), g.getGraphs());
    g.countTotalColorDiff();

    Calculate c;
    //c.printResult(s.getShapes(), g.getGraphs(), w.getWindows(), argc, argv);
    //printf("real score : %.2f\n\n\n", c.real_score(w.getWindows()));

    //std::vector<Graph> &gg = g.getGraphs();
    //c.changeGraphColor(w.getWindows(), g.getGraphs()[0]);
    //c.changeGraphColor(w.getWindows(), g.getGraphs()[0]);
    //c.changeGraphColor(w.getWindows(), g.getGraphs()[2]);
    c.initial_total_color_diff(&g, g.getGraphs(), w.getWindows());
    //c.printResult(s.getShapes(), g.getGraphs(), w.getWindows(), argc, argv);
    //printf("real score : %.2f\n\n\n", c.real_score(w.getWindows()));

    for (int group_id = 0; group_id < c.group_size; group_id++) {
        c.iterative_sol(g.getGraphs(), w.getWindows(), group_id);
    }
    //printf("real score : %.2f\n\n\n", c.real_score(w.getWindows()));
    c.simulatedAnnealing(g.getGraphs(), w.getWindows(), start, argv[1]);

    for (int group_id = 0; group_id < c.group_size; group_id++) {
        c.iterative_sol(g.getGraphs(), w.getWindows(), group_id);
    }

    c.printResult(s.getShapes(), g.getGraphs(), w.getWindows(), argc, argv);
    printf("real score : %.2f\n\n\n", c.real_score(w.getWindows()));
    return 0;
}
