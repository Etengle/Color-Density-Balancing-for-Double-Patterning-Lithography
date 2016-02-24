#ifndef __CALCULATE_H__
#define __CALCULATE_H__
#include "graphs.h"
#include "shapes.h"
#include "window.h"
#include <vector>
#include <iostream>
#include <sys/time.h>

class Calculate {

    struct Functor_of_Compare_graphs {
        Graphs* gs;
        Functor_of_Compare_graphs(Graphs* gs) : gs(gs) {};
        bool operator () ( const int & i, const int & j ){ return gs->getGraphs()[i].total_color_diff < gs->getGraphs()[j].total_color_diff; }
    };

    struct Distance_Pair{
        int distance;
        int id;
        bool operator () (const Distance_Pair & i, const Distance_Pair & j) { return i.distance < j.distance; }
    };

    private:
        std::vector<int> graph_order;
        std::vector<int> graph_heap;
        std::vector<std::vector<int>> graph_group_id;
        std::vector<std::vector<int>> win_group_id;
        time_t t = time(NULL);
        std::vector<double> temperature;


    public:
        Calculate (){

        }
        ~Calculate (){}
        void changeGraphColor(std::vector<Window> &window, Graph &);
        void initial_color(std::vector<Window> &, Graph &);
        void initial_total_color_diff(Graphs *, std::vector<Graph> &, std::vector<Window> &);
        int choose_color(std::vector<Window> &, Graph &);
        int choose_color2(std::vector<Window> &, Graph &, Graph &);
        void getNowState(std::vector<Graph> &, std::vector<int> &);
        void setNowState(std::vector<Graph> &, std::vector<Window> &, std::vector<int> &);
        void getNowState_for_group(std::vector<Graph> &, std::vector<int> &, int);
        void setNowState_for_group(std::vector<Graph> &, std::vector<Window> &, std::vector<int> &, int);
        void iterative_sol(std::vector<Graph> &, std::vector<Window> &, int);
        double acceptanceProbability(int, double);
        void calcTemperature(std::vector<Graph> &, std::vector<Window> &);
        void calcTemperature_for_group(std::vector<Graph> &, std::vector<Window> &, int);
        void changeTimeSeed(int , char *);
        void simulatedAnnealing(std::vector<Graph> &, std::vector<Window> &, struct timeval, char*);
        int cost_function(std::vector<Window> &);
        int cost_function_for_group(std::vector<Window> &, int);
        double real_score(std::vector<Window> &);
        double real_score_for_group(std::vector<Window> &, int);
        double real_score_from_cost(int);
        void printResult(std::vector<Shape> &, std::vector<Graph> &, std::vector<Window> &, int, char**);
        int group_size = 0;
};

#endif
