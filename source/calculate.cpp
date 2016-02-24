#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <string.h>
#include <sys/time.h>
#include "calculate.h"
#define UNCOLORABLE -2
using namespace std;

extern int windowsize;

void Calculate::changeGraphColor(vector<Window> &window, Graph &g) {
    g.color_type *= (-1);
    for (int i = 0; i < g.win_id.size(); i++){
        window[g.win_id[i]].color_diff += 2 * g.color_type * g.win_color_diff[i];
	}
}

void Calculate::initial_color(vector<Window> &window, Graph &g) {
    int score1 = 0, score2 = 0;
	for (int i = 0; i < g.win_id.size(); i++){
		score1 += abs(window[g.win_id[i]].color_diff + g.color_type * g.win_color_diff[i]);
		score2 += abs(window[g.win_id[i]].color_diff - g.color_type * g.win_color_diff[i]);
	}
	if (score1 > score2) {
        g.color_type *= (-1);
    }
    for (int i = 0; i < g.win_id.size(); i++){
        window[g.win_id[i]].color_diff += g.color_type * g.win_color_diff[i];
    }
}

void Calculate::initial_total_color_diff(Graphs *g, vector<Graph> &graph, vector<Window> &window) {
    int g_id;
    std::vector<int> visited(graph.size(), 0); // 0 -> not visited, 1 -> visited
    std::vector<int> group;
    std::vector<int> win_visited_id(window.size(), 0);
    std::vector<int> win_group;

    for (int i = 0; i < graph.size(); i++){
		graph_order.push_back(i);
	}
	Functor_of_Compare_graphs compare_graphs(g);
	sort(graph_order.begin(), graph_order.end(), compare_graphs);
    //for (auto ii : graph_order) std::cout << ii << ' ' << graph[ii].total_color_diff << endl;

    make_heap(graph_heap.begin(), graph_heap.end(), compare_graphs);

    for (int i = graph.size() - 1; i >= 0; i--) {
        //printf("i = %d, g_id = %d\n", i, graph_order[i]);

        if (visited[graph_order[i]] == 0) {
            visited[graph_order[i]] = 1;
            graph_heap.push_back(graph_order[i]);
            push_heap(graph_heap.begin(), graph_heap.end(), compare_graphs);
        }

        while (!graph_heap.empty()) {
            pop_heap(graph_heap.begin(), graph_heap.end(), compare_graphs);
            g_id = graph_heap.back();
            graph_heap.pop_back();
            group.push_back(g_id);

            initial_color(window, graph[g_id]);
            //printf("g_id : %d\n", g_id);
            //std::cout << "after pop_heap: ";
            //for (auto ii : graph_heap) std::cout << ii << ' ';
            //std::cout << '\n';

            for (int j = 0; j < graph[g_id].win_id.size(); j++) {
                if (win_visited_id[graph[g_id].win_id[j]] == 0) {
                    win_visited_id[graph[g_id].win_id[j]] = 1;
                    win_group.push_back(graph[g_id].win_id[j]);
                }
                for (int k = 0; k < window[graph[g_id].win_id[j]].graph_id.size(); k++) {
                    if (visited[window[graph[g_id].win_id[j]].graph_id[k]] == 0) {
                        visited[window[graph[g_id].win_id[j]].graph_id[k]] = 1;
                        graph_heap.push_back(window[graph[g_id].win_id[j]].graph_id[k]);
                        push_heap(graph_heap.begin(), graph_heap.end(), compare_graphs);
                    }
                }
            }
        }

        if (!group.empty()) {
            std::vector<int> temp = group;
            graph_group_id.push_back(temp);
            group.clear();

            std::vector<int> temp2 = win_group;
            win_group_id.push_back(temp2);
            win_group.clear();
        }
    }

    /*for (std::vector<int> gg : graph_group_id) {
        printf("size : %d\n\t", gg.size());
        for (int id : gg) {printf("\t%d", id);}
        printf("\n\n");
    }*/
    group_size = graph_group_id.size();

}

int Calculate::choose_color(vector<Window> &window, Graph &g) {
    int score1 = 0, score2 = 0;
	int i;
	for (i = 0; i < g.win_id.size(); i++){
		score1 += abs(window[g.win_id[i]].color_diff);
		score2 += abs(window[g.win_id[i]].color_diff - 2 * g.color_type * g.win_color_diff[i]);
	}
	return score1 - score2;
}

int Calculate::choose_color2(vector<Window> &window, Graph &g1, Graph &g2) {
    int score1 = 0, score2 = 0;
	int i;
	for (i = 0; i < g1.win_id.size(); i++){
		score1 += abs(window[g1.win_id[i]].color_diff);
		score2 += abs(window[g1.win_id[i]].color_diff - 2 * g1.color_type * g1.win_color_diff[i]);
	}
	return score1 - score2;
}

void Calculate::getNowState(vector<Graph> &graph, vector<int> &color_type) {
    for (int i = 0; i < graph.size(); i++) {
        color_type[i] = graph[i].color_type;
    }
}

void Calculate::getNowState_for_group(vector<Graph> &graph, vector<int> &color_type, int group_id) {
    for (int i = 0; i < graph_group_id[group_id].size(); i++) {
        color_type[i] = graph[graph_group_id[group_id][i]].color_type;
    }
}

void Calculate::setNowState(vector<Graph> &graph, vector<Window> &window, vector<int> &color_type) {
    for (int i = 0; i < graph.size(); i++) {
        if (color_type[i] != graph[i].color_type) changeGraphColor(window, graph[i]);
    }
}

void Calculate::setNowState_for_group(vector<Graph> &graph, vector<Window> &window, vector<int> &color_type, int group_id) {
    for (int i = 0; i < graph_group_id[group_id].size(); i++) {
        if (color_type[i] != graph[graph_group_id[group_id][i]].color_type) changeGraphColor(window, graph[graph_group_id[group_id][i]]);
    }
}

void Calculate::iterative_sol(vector<Graph> &graph, vector<Window> &window, int group_id){
    int change = 1;
    int g_id;
    std::vector<int> best, current;
    best.resize(graph.size(), 1);
    getNowState(graph, best);
    current = best;
    int best_cost = cost_function(window);
	int current_cost;
    int graphcount = graph.size();
    int counting = 0;

    std::vector<Distance_Pair> dis;
    std::vector<int> visited(graphcount, 1); // 0 -> not visited, 1 -> visited


    printf("count : %d, score : %d, %.2f\n", 0, best_cost,  real_score(window));
    while (change) {
        /*if (change == 1) {
            change = 2;
        } else if (change == 2) {
            change = 0;
        }*/
        if (change == 1) {
            change = 0;
        }
        for (int i = 0; i < graphcount; i++) {
            visited[i] = 1;
        }
        for (int id : graph_group_id[group_id]) {
            visited[id] = 0;
        }

        for (int i = 0; i < graph_group_id[group_id].size(); i++) {
            for (int j = 0; j < graphcount; j++) {
                if (visited[j] == 0) {
                    Distance_Pair d;
                    d.id = j;
                    d.distance = choose_color(window, graph[j]);
                    dis.push_back(d);
                }
            }

            std::sort(dis.begin(), dis.end(), Distance_Pair());
            //for (auto ii : dis) std::cout << ii.id << ' ' << ii.distance << endl;
            //char c;
            //scanf("%c", &c);
            //printf("id = %d, distance = %d\n", dis.back().id, dis.back().distance);
            g_id = dis.back().id;
            visited[g_id] = 1;

            changeGraphColor(window, graph[g_id]);
            current[g_id] = graph[g_id].color_type;
            current_cost = cost_function(window);
            if (best_cost > current_cost) {
                best = current;
                //setNowState(graph, window, best);
                //printf("@@ %.2f, %.2f, %10d, %10d\n", real_score(window), real_score_from_cost(best_cost), current_cost, best_cost);
                best_cost = current_cost;
                change = 1;
            }
            dis.clear();
        }
        setNowState(graph, window, best);
        current = best;
        printf("count : %d, score : %d\n", ++counting, best_cost);
        printf("%.2f\n", real_score(window));
    }
}

double Calculate::acceptanceProbability(int distance, double temperature){
	if (distance > 0) {
		return 1;
	}
    if (temperature - 1E-7 < 0) {
        return 0;
    }

	return 0.8 * exp((double)distance / (double)temperature);
}

void Calculate::changeTimeSeed(int group_id, char *inputfilename){

    //double low_limit = 20000;
    //if (temperature <= low_limit ) {
    //    temperature = low_limit;
    //}

    string filename(inputfilename);
    string str1("case1");
    string str2("case2");
    string str3("case3");
    string str4("case4");
    string str5("case5");
    if (filename.find(str3) != string::npos) {
        t = time(NULL);
    } else if (filename.find(str3) != string::npos) {
        t = time(NULL);
    } else if (filename.find(str3) != string::npos) {
        t = time(NULL);
    } else if (filename.find(str4) != string::npos) {
        t = time(NULL);
    } else if (filename.find(str5) != string::npos) {
        t = time(NULL);
    }
}

void Calculate::calcTemperature(vector<Graph> &graph, vector<Window> &window){
    std::vector<std::vector<int>> orig, current;
    orig.resize(graph_group_id.size());
    current.resize(graph_group_id.size());
    for (int i = 0; i < graph_group_id.size(); i++) {
        orig[i].resize(graph_group_id[i].size());
        getNowState_for_group(graph, orig[i], i);
        current[i] = orig[i];
    }

    //vector<double> temperature;
    temperature.resize(group_size, 0.0);
    printf("rand : %d\n", rand());
    int chosen_id, distance;
    int test_num = 5000;
    //time_t t = time(NULL);
    //printf("Number : %ld\n", t);
    //srand(t);
    double prob = 0.8;

    for (int group_id = 0; group_id < group_size; group_id++) {
        printf("temperature %d : %15.5f\n", group_id, temperature[group_id]);
        for (int i = 0; i < test_num; i++) {

            chosen_id = rand() % graph_group_id[group_id].size();
            //if (i < 10) {
            //    printf("id : %d\n", chosen_id);
            //}
            distance = choose_color(window, graph[graph_group_id[group_id][chosen_id]]);
            if (distance < 0) {
                temperature[group_id] -= (double)distance;
            }
            //if ( i < 10) {
                //printf("t_10 : %d, %d, %f\n", graph[graph_group_id[group_id][chosen_id]].shapeid[0], distance, temperature[group_id]);
            //}
        }
        //printf("temperature %d : %.5f\n", group_id, temperature[group_id]);
        temperature[group_id] = (-1) * temperature[group_id] / log(prob) / (double)test_num;
        printf("temperature %d : %.5f\n", group_id, temperature[group_id]);
    }

    for (int i = 0; i < group_size; i++) {
        setNowState_for_group(graph, window, orig[i], i);
    }

}

void Calculate::calcTemperature_for_group(vector<Graph> &graph, vector<Window> &window, int group_id){

    std::vector<int> orig, current;
    orig.resize(graph_group_id[group_id].size());
    getNowState_for_group(graph, orig, group_id);
    current = orig;

    //vector<double> temperature;
    //temperature.resize(group_size, 0.0);


    int chosen_id, distance;
    int test_num = 5000;
    //time_t t = time(NULL);
    //printf("Number : %ld\n", t);
    //srand(t);
    double prob = 0.8;

    //printf("temperature %d : %15.5f\n", group_id, temperature[group_id]);
    temperature[group_id] = 0.0;
    for (int i = 0; i < test_num; i++) {

        chosen_id = rand() % graph_group_id[group_id].size();
        distance = choose_color(window, graph[graph_group_id[group_id][chosen_id]]);
        if (distance < 0) {
            temperature[group_id] -= distance;
        }
    }
    temperature[group_id] = (-1) * temperature[group_id] / log(prob) / (double)test_num;
    //printf("temperature %d : %.5f\n", group_id, temperature[group_id]);

    setNowState_for_group(graph, window, orig, group_id);

}

void Calculate::simulatedAnnealing(vector<Graph> &graph, vector<Window> &window, struct timeval start, char *inputfilename){
    changeTimeSeed(0, inputfilename);
    printf("Number : %ld\n", t);
    srand(t);
    //printf("rand : %d\n", rand());

    calcTemperature(graph, window);

    std::vector<std::vector<int>> best, current, last_change;
    best.resize(graph_group_id.size());
    current.resize(graph_group_id.size());
    last_change.resize(graph_group_id.size());
    for (int i = 0; i < graph_group_id.size(); i++) {
        best[i].resize(graph_group_id[i].size());
        getNowState_for_group(graph, best[i], i);
        current[i] = best[i];
        last_change[i] = best[i];
    }

    std::vector<int> no_change;
    no_change.resize(graph_group_id.size(), 10000);

    /*int graph_area = 0;
    for (int i = 0; i < graph.size(); i++) {
        graph_area += abs(graph[i].total_color_diff);
    }*/
    //printf("graph area : %d, g_a/%d : %d\n", graph_area, graph.size(), graph_area/graph.size());

    struct timeval end;
    gettimeofday(&end, NULL);
    //double temperature = 1000000000;
	//int temperature = cost_function(window);
    //double high_limit = temperature / 20;
    //double low_limit = 20000;
    //double low_limit = 8000;
    //double high_limit = low_limit * 8;
    //printf("low_limit : %lf\n", low_limit);
    //printf("%d\n", cost_function(window));
	int cooling = 2;
	int chosen_id, distance, group_id, graph_size;
    graph_size = graph.size();
    int group_count = graph_group_id.size();
	std::vector<int> best_cost, current_cost, graphcount;

    best_cost.resize(group_count);
    current_cost.resize(group_count);
    graphcount.resize(group_count);

    for (int i = 0; i < group_count; i++) {
        best_cost[i] = cost_function_for_group(window, i);
        current_cost[i] = best_cost[i];
        graphcount[i] = graph_group_id[i].size();
    }

	double rand_double, score;
    //time_t t = time(NULL);
	//time_t t = 1437963533;
    //changeTimeSeed(group_id, inputfilename);

    int counting = 1000;
    int run_minute = 55;
    long time_val = end.tv_sec - start.tv_sec;
    int time_count = (time_val / 60);
    int rounds_limit, rounds;
    rounds_limit = 6000000;
    rounds = 0;
    //int initial_score = real_score_from_cost(best_cost);

    printf("time : %d min", time_count);
    score = 70.0;
    for (int i = 0; i < group_count; i++) {
        score += (real_score_from_cost(best_cost[i]) - 70.0);
    }
    printf("\ttotal score : %.2f\n", score);
    for (int i = 0; i < group_count; i++) {
        printf("\t\t\tscore : %.2f\n", real_score_for_group(window, i));
    }
    for (int i = 0; i < group_count; i++) {
        temperature[i] = temperature[i] * (rounds_limit - rounds) / rounds_limit;
        printf("\ttemp %d : \t%12.5f\n", i, temperature[i]);
    }
    time_count++;



    //printf("temperature : %f\n", temperature);
	while (counting > 0 && rounds < rounds_limit){
        counting--;
        if (counting == 0) {
            gettimeofday(&end, NULL);
            time_val = end.tv_sec - start.tv_sec;
            if (time_val < run_minute * 60) {
                counting = 1000;
                if (time_val > time_count * 60) {
                    //printf("time : %d min, score : %.2f\n", time_count, real_score_from_cost(best_cost));
                    printf("time : %d min", (int) time_val/60);
                    score = 70.0;
                    for (int i = 0; i < group_count; i++) {
                        score += (real_score_from_cost(best_cost[i]) - 70.0);
                    }
                    printf("\ttotal score : %.2f\n", score);
                    for (int i = 0; i < group_count; i++) {
                        //printf("\t\tscore : %.2f\n", real_score_for_group(window, i));
                        printf("\t\t\tscore : %.2f\n", real_score_from_cost(best_cost[i]) - 70.0);
                    }
                    for (int i = 0; i < group_count; i++) {
                        printf("\ttemp %d : \t%12.5f\n", i, temperature[i]);
                    }
                    time_count++;
                }
                //printf("temperature : %f\n", temperature);
            } else {
                printf("time : %d min", (int) time_val/60);
                score = 70.0;
                for (int i = 0; i < group_count; i++) {
                    score += (real_score_from_cost(best_cost[i]) - 70.0);
                }
                printf("\ttotal score : %.2f\n", score);
                for (int i = 0; i < group_count; i++) {
                    printf("\t\t\tscore : %.2f\n", real_score_from_cost(best_cost[i]) - 70.0);
                }
                for (int i = 0; i < group_count; i++) {
                    printf("\ttemp %d : \t%12.5f\n", i, temperature[i]);
                }
                //printf("cost : %d\n", cost_function(window));
            }
        }

        for (int try_times = 0; try_times < 10000; try_times++) {
            group_id = rand() % graph_size;
            for(int i = 0; i < group_count; i++) {
                group_id = group_id - graphcount[i];
                if (group_id < 0) {
                    group_id = i;
                    break;
                }
            }

            chosen_id = rand() % graphcount[group_id];
    		distance = choose_color(window, graph[graph_group_id[group_id][chosen_id]]);
            //printf("chosen_id : %d, is : %d, acc : %f\n", chosen_id, distance, acceptanceProbability(distance, best_cost));

    		rand_double = (double)rand() / (double)RAND_MAX;
    		//printf("chosen_id : %d, distance : %d, temperature : %d, acc : %f, rand : %f\n", chosen_id, distance, temperature, acceptanceProbability(distance, temperature), rand_double);
    		if (acceptanceProbability(distance, temperature[group_id]) >= rand_double) {
    			changeGraphColor(window, graph[graph_group_id[group_id][chosen_id]]);
    			current[group_id][chosen_id] = graph[graph_group_id[group_id][chosen_id]].color_type;
    			//printf("now_cost : %d, cost_function : %d\n", now_cost, cost_function());
    			current_cost[group_id] = cost_function_for_group(window, group_id);
    			if(best_cost[group_id] > current_cost[group_id]){
    				best[group_id] = current[group_id];
    				best_cost[group_id] = current_cost[group_id];
                    /*printf("change id : %d\n", group_id);
                    for (int i = 0; i < group_count; i++) {
                        printf("\t\t\tscore : %10d\n", best_cost[i]);
                    }*/
                    //printf("best_cost : %d\tdistance : %d\t temperature : %f\tacceptanceProbability : %f\n", best_cost, distance, temperature, acceptanceProbability(distance, temperature));
    			}
    		}
        }

        //printf("drop temperature %d : \n", rounds);
        for (int i = 0; i < group_count; i++) {

            if (best[i] == last_change[i]) {
                no_change[i]--;
            } else {
                last_change[i] = best[i];
                no_change[i] = 10000;
            }

            if (no_change[i] == 0) {
                //temperature[i] = temperature[i] * 10;
                calcTemperature_for_group(graph, window, i);
                no_change[i] = 10000;
            } else {
                //temperature[i] = temperature[i] * (rounds_limit - rounds) / rounds_limit;
                temperature[i] = temperature[i] * 0.9995; // 0.9967 = 0.95 root 16
            }
            //printf("\t%d\n", no_change[i]);
            //printf("\ttemp %d : \t%12.5f\n", i, temperature[i]);
        }
        rounds++;
	}

	//setNowState(graph, window, best);
    for (int i = 0; i < group_count; i++) {
        setNowState_for_group(graph, window, best[i], i);
    }

    for (int i = 0; i < group_count; i++) {
        iterative_sol(graph, window, group_id);
    }
    //printf("time_seed : %ld\n", t);
    //printf("improve : %.2f\n", real_score_from_cost(best_cost) - initial_score);
    printf("Number : %ld\n", t);
}

int Calculate::cost_function_for_group(vector<Window> &window, int group_id) {
	// count as total difference
	// we wish the score to be as low as possible
	int i, win_id, score = 0;
    for (i = 0; i < win_group_id[group_id].size(); i++) {
        win_id = win_group_id[group_id][i];
        score += (window[win_id].color_diff > 0) ? window[win_id].color_diff : (-1) * window[win_id].color_diff;
    }
	return score;
}

int Calculate::cost_function(vector<Window> &window) {
	// count as total difference
	// we wish the score to be as low as possible
	int i, score = 0;
	for (i = 0; i < window.size(); i++){
		score += (window[i].color_diff > 0) ? window[i].color_diff : (-1) * window[i].color_diff;
	}
	return score;
}

double Calculate::real_score_for_group(vector<Window> &window, int group_id) {
	//we wish the real score to be as high as possible
	int i, win_id;
	double score = 0.0;
	double base = 0.2 * 100.0 / (windowsize * windowsize);
    for (i = 0; i < win_group_id[group_id].size(); i++) {
        win_id = win_group_id[group_id][i];
        score -= (window[win_id].color_diff > 0) ? window[win_id].color_diff * base : window[win_id].color_diff * (-1) * base;
    }
	return score;
}

double Calculate::real_score(vector<Window> &window) {
	//we wish the real score to be as high as possible
	int i;
	double score = 70.0;
	double base = 0.2 * 100.0 / (windowsize * windowsize);
	for (i = 0 ; i < window.size(); i++){
		score -= (window[i].color_diff > 0) ? window[i].color_diff * base : window[i].color_diff * (-1) * base;
	}
	return score;
}

double Calculate::real_score_from_cost(int cost) {
    double base = 0.2 * 100.0 / (windowsize * windowsize);
    //printf("cost : %d, base : %f, cost/base : %f", cost, base, ((double)cost/base));
    return 70.0 - ((double)cost * base);
}

void Calculate::printResult(vector<Shape> &shape, vector<Graph> &graph, vector<Window> &window, int argc, char** argv) {

    FILE *fout = stdout;
	if(argc == 3){
		if((fout = fopen(argv[2], "w")) == NULL){
			exit(1);
		}
	}

    int NO, A, B;
    double win_squre_2X = 2 * windowsize * windowsize / 100.0;

    for (int i = 0;  i < window.size(); i++) {
        fprintf(fout, "WIN[%d]=%d,%d,%d,%d(%.2f %.2f)\n",
            i+1, window[i].left_x, window[i].down_y, window[i].right_x, window[i].up_y,
            (double)(window[i].total_area + window[i].color_diff) / win_squre_2X , (double)(window[i].total_area - window[i].color_diff) / win_squre_2X);
    }
    for (int i = 0; i < graph.size(); i++) {

        fprintf(fout, "GROUP\n");
        A = 0, B = 0, NO = 0;

        for (int j = 0; j < graph[i].shapeid.size(); j++) {
            int id = graph[i].shapeid[j];
			switch (shape[id].color){
				case UNCOLORABLE:
					fprintf(fout, "NO[%d]=", ++NO);
					break;
				case 0:
					if (graph[i].color_type == 1) {
						fprintf(fout, "CA[%d]=", ++A);
					} else {
						fprintf(fout, "CB[%d]=", ++B);
					}
					break;
				case 1:
					if (graph[i].color_type == 1) {
						fprintf(fout, "CB[%d]=", ++B);
					} else {
						fprintf(fout, "CA[%d]=", ++A);
					}
					break;
				default:
					fprintf(fout, "UNCOLOR=");
					break;
			}
		    fprintf(fout, "%d,%d,%d,%d\n", shape[id].left_x, shape[id].down_y, shape[id].right_x, shape[id].up_y);
        }
    }
}
