#ifndef __WINDOW_H__
#define __WINDOW_H__
#include <vector>
#include <iostream>
#include "rectangle.h"

class Window : public Rectangle{

    friend class Windows;
    friend class Graphs;
    friend class Calculate;

    private:
    	int color_diff;   // color_0 = (total_area + color_diff) / 2
    	int total_area;   // color_1 = (total_area - color_diff) / 2
        std::vector<int> graph_id;

    public:
        Window (int &lx, int &dy, int &rx, int &uy)
        : Rectangle(lx, dy, rx, uy), color_diff(0), total_area(0) {

        }
        ~Window (){}

        int contains_graph(int g_id) {
            for (int i = 0 ; i < graph_id.size(); i++) {
                if (graph_id[i] == g_id) return i;
            }
            return -1;
        }

};

#endif
