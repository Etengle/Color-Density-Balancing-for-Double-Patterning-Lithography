#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include "graphs.h"
#include "graph.h"
using namespace std;
#define UNUSE -1
#define UNCOLORABLE -2


void Graphs::testPrint(vector<Shape> &shape){
    cout << endl;
	for (int i = 0; i < shape.size(); i++) printf("id = %d, graph id = %d\n", i+1, shape[i].graphid);
	cout << endl;
	for (int i = 0; i < graph.size(); i++){
		printf("graphid = %d, shape id = {", i);
		for (int j = 0; j < graph[i].shapeid.size(); j++)
			printf("%dc%d, ", graph[i].shapeid[j]+1, shape[graph[i].shapeid[j]].color);
		printf("\b\b}\n");
	}
	cout << endl;
}

/* run recursively,
/* mark graphid and also init coloring to check if bipartite,
/* if not, return 1. */
int Graphs::markGraphIdAndInitColoring(vector<Shape> &shape,int color, int i, int id){
	int res = 0, r = 0;
	shape[i].graphid = id;
	for (int j = 0; j < shape[i].conflictid.size(); j++){
		if (shape[shape[i].conflictid[j]].color == UNUSE) shape[shape[i].conflictid[j]].color = color;
		else if (shape[shape[i].conflictid[j]].color != color) res = 1;
		if (shape[shape[i].conflictid[j]].graphid == UNUSE){
			graph[id].shapeid.push_back(shape[i].conflictid[j]);
			r = markGraphIdAndInitColoring(shape, !color, shape[i].conflictid[j], id);
		}
	}
	return res || r;
}


void Graphs::buildConflictGraph(vector<Shape> &shape){
    for (int i = 0; i < shape.size(); i++){
        if(shape[i].graphid == UNUSE){
            Graph *g = new Graph();
            g->shapeid.push_back(i);
            graph.push_back(*g);
            shape[i].color = 0;
            /* if retun value is 1, means uncolorable. (not bipartite) */
			if(markGraphIdAndInitColoring(shape, 1, i, graph.size()-1)){
				/* give all shape UNCOLORABLE */
				for (int j = 0; j < graph[graph.size()-1].shapeid.size(); j++)
					shape[graph[graph.size()-1].shapeid[j]].color = UNCOLORABLE;
			}
        }
    }
    // testPrint(shape);
}

void Graphs::countTotalColorDiff() {
    for (int i = 0; i < graph.size(); i++) {
        for (int j = 0; j < graph[i].win_color_diff.size(); j++) {
            graph[i].total_color_diff += abs(graph[i].win_color_diff[j]);
        }
    }
}

std::vector<Graph>& Graphs::getGraphs() {
    return graph;
}
