#include <iostream>
#include <vector>
#include <cstdio>
#include "windows.h"
#include "window.h"
#define UNCOLORABLE -2
using namespace std;

extern int windowsize, box_lx, box_rx, box_uy, box_dy;

void Windows::createWindow() {
    int win_xlen = (box_rx - box_lx + windowsize - 1) / windowsize;
	int win_ylen = (box_uy - box_dy + windowsize - 1) / windowsize;

    int lx, dy, rx, uy, diff;

    for (int j = 0; j < win_ylen; j++) {
		for (int i = 0; i < win_xlen; i++) {

            lx = box_lx + windowsize * i;     //lx
            dy = box_dy + windowsize * j;     //dy
            rx = box_lx + windowsize * (i+1); //rx
            uy = box_dy + windowsize * (j+1); //uy

            //printf("lx : %d, dy : %d, rx : %d, uy : %d\n", lx, dy, rx, uy);

            if (rx > box_rx) {
                rx = box_rx;
                lx = rx - windowsize;
            }

            if (uy > box_uy) {
                uy = box_uy;
                dy = uy - windowsize;
            }

            Window w = Window(lx, dy, rx, uy);
            //printf("WIN=%d,%d,%d,%d\n",
                //w.left_x, w.down_y, w.right_x, w.up_y);
            window.push_back(w);
        }
    }
}


void Windows::countArea(vector<Shape> &shape, vector<Graph> &graph) {

    int windowid, graphid, i_w, i_g;
    int win_left, win_right, win_top, win_down, area;
    int win_xlen = (box_rx - box_lx + windowsize - 1) / windowsize;
	int win_ylen = (box_uy - box_dy + windowsize - 1) / windowsize;

    for (int k = 0; k < shape.size(); k++) {
        if(shape[k].color == UNCOLORABLE) continue;
        win_left  = (shape[k].left_x  - box_lx)    / windowsize;
		win_right = (shape[k].right_x - box_lx - 1)/ windowsize; // if windowsize = 10 and right_x = 10, win_right should be 0
		win_down  = (shape[k].down_y  - box_dy)    / windowsize;
		win_top   = (shape[k].up_y    - box_dy - 1)/ windowsize;

        if ((box_rx - windowsize) < shape[k].right_x) {win_right = win_xlen - 1;}
        if ((box_uy - windowsize) < shape[k].up_y)    {win_top   = win_ylen - 1;}

        for (int j = win_down; j <= win_top; j++){
			for (int i = win_left; i <= win_right; i++){
                windowid = j*win_xlen + i;
                graphid  = shape[k].graphid;
				area = window[windowid].intersect_Area(shape[k]);
                //printf("shape_id : %d, win %d, g_id %d, area %d\n", k, windowid, graphid, area);

                window[windowid].total_area += area;
				graph[graphid].total_area += area;

                i_w = graph[graphid].contains_win(windowid);
                if (i_w == -1) {
                    graph[graphid].win_id.push_back(windowid);
                    graph[graphid].win_color_diff.push_back(0);
                    i_w = graph[graphid].win_id.size() - 1;
                }

                i_g = window[windowid].contains_graph(graphid);
                if (i_g == -1) {
                    window[windowid].graph_id.push_back(graphid);
                }

                if(shape[k].color == 0) {
                    //window[windowid].color_diff += area;
                    graph[graphid].win_color_diff[i_w] += area;
				} else {
					//window[windowid].color_diff -= area;
                    graph[graphid].win_color_diff[i_w] -= area;
				}
            }
        }
    }
}

std::vector<Window>& Windows::getWindows() {
    return window;
}
