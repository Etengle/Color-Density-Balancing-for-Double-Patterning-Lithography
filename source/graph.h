#ifndef __GRAPH_H__
#define __GRAPH_H__
#include <vector>
#include <iostream>

class Graph {

    friend class Graphs;
    friend class Windows;
    friend class Calculate;

    private:
        int total_area;
    	int color_type; // 1 if color_0 - color_1, -1 if color_1 - color_0
        std::vector<int> win_id;
        std::vector<int> win_color_diff;
        int total_color_diff;
        std::vector<int> shapeid;
    public:
        Graph ()
        : total_area(0), color_type(1), total_color_diff(0){}
        ~Graph (){}

        int contains_win(int window_id) {
            for (int i = 0; i < win_id.size(); i++) {
                if (win_id[i] == window_id) return i;
            }
            return -1;
        }

};

#endif
