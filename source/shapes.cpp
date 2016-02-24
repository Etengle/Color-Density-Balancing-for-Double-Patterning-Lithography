#include <iostream>
#include <fstream>
#include <algorithm>
#include <cstdlib>
#include <cstdio>
#include "shapes.h"
#include "graphs.h"
#define UNCOLORABLE -2
using namespace std;

extern int x_spacing, y_spacing, windowsize;
extern int box_lx, box_rx, box_uy, box_dy;	// bounding box


/* for debugging purpose */
void Shapes::testPrint(){
	cout << endl;
	cout << "shapedount = " << shape.size() << endl;
	cout << endl;
	for (int i = 0; i < shape.size(); i++)
		cout << "rx[" << i << "]: id = " << rx[i] << ", rx = " << shape[rx[i]].right_x << endl;
	for (int i = 0; i < shape.size(); i++)
		cout << "uy[" << i << "]: id = " << uy[i] << ", uy = " << shape[uy[i]].up_y << endl;
	cout << endl;
	for (int i = 0; i < shape.size(); i++){
		cout << "id = " << i+1 << ", conflict id = {";
		for (int j = 0; j < shape[i].conflictid.size(); j++)
			cout << shape[i].conflictid[j]+1 << ", ";
		cout << "\b\b}\n";
	}
	cout << endl;
	printf("bounding box = (%d, %d)x(%d, %d)\n", box_lx, box_dy, box_rx, box_uy);
}


/* check if one shape is a neighbor to the other */
bool conflict(int &a, int &b, int &c, int &d){
	if (!(b >= d || a >= c)) return true;
	return false;
}

/* error msg */
void Shapes::errorMsg(int code, char* str){
	cout << "Error: ";
	switch(code){
		case 1:
			cout << "Unable to open file " << str << endl;
			break;
		case 2:
			cout << "The format is \"./DPT_balance_color ($input_file_name) ($input_file_name).out\"" << endl;
			break;
	}
}

/* parser */
void Shapes::readFile(int argc, char** argv){
	FILE *fin;
	int lx, rx, uy, dy;
	if(argc >= 2){
		if((fin = fopen(argv[1], "r")) == NULL){
			errorMsg(1, argv[1]);
			exit(1);
		}
	}
    else {errorMsg(2, NULL); exit(2);}
    fscanf(fin, "ALPHA=%d\n", &x_spacing);
	fscanf(fin, "BETA=%d\n", &y_spacing);
	fscanf(fin, "OMEGA=%d\n", &windowsize);
    while(fscanf(fin, "%d,%d,%d,%d", &lx, &dy, &rx, &uy) == 4){
        Shape s = Shape(lx, dy, rx, uy);
        shape.push_back(s);
    }
    fclose(fin);
}

/* return a referrence */
std::vector<Shape>& Shapes::getShapes(){
	return shape;
}

void Shapes::buildConflictArray(){
	for (int i = 0; i < shape.size(); i++){
		rx.push_back(i);
		uy.push_back(i);
	}
	Functor_of_Compare_rx compare_rx(this);
	Functor_of_Compare_uy compare_uy(this);
	sort(rx.begin(), rx.end(), compare_rx);
	sort(uy.begin(), uy.end(), compare_uy);
	/* degugging */
	/*
		for (auto i : rx) cout << i << " ";
		cout << endl;
		for (auto i : uy) cout << i << " ";
		cout << endl;
	*/
	// build conflict id arrays
	for (int i = 0; i < shape.size(); i++){
		// x-spacing
		for (int j = 0; shape.at(rx.at(j)).right_x < shape.at(i).left_x; j++){
			if (shape.at(i).left_x - shape.at(rx.at(j)).right_x <= x_spacing &&
				conflict(shape.at(rx.at(j)).down_y, shape.at(i).down_y, shape.at(i).up_y, shape.at(rx.at(j)).up_y)){
				shape.at(i).conflictid.push_back(rx.at(j));
				shape.at(rx.at(j)).conflictid.push_back(i);
			}
		}
		// y-spacing
		for (int j = 0; shape[uy[j]].up_y < shape[i].down_y; j++){
			if (shape[i].down_y - shape[uy[j]].up_y <= y_spacing &&
				conflict(shape[uy[j]].left_x, shape[i].left_x, shape[i].right_x, shape[uy[j]].right_x)){
				shape.at(i).conflictid.push_back(uy[j]);
				shape.at(uy[j]).conflictid.push_back(i);
			}
		}
	}
}

void Shapes::findBoundingBox(){
	box_lx = shape[shape.size() - 1].right_x;
	box_dy = shape[shape.size() - 1].up_y;
	/* the largest rx and is not UNCOLORABLE is the rx of bounding box. */
	for (int i = shape.size() - 1; i >= 0; i--){
		if (shape[rx[i]].color != UNCOLORABLE){
			box_rx = shape[rx[i]].right_x;
			break;
		}
	}
	/* the largest upy and is not UNCOLORABLE is the upy of bounding box. */
	for (int i = shape.size() - 1; i >= 0; i--){
		if (shape[uy[i]].color != UNCOLORABLE){
			box_uy = shape[uy[i]].up_y;
			break;
		}
	}
	/* general search lx, dy, I still think how to make it faster. */
	for (int i = 0; i < shape.size(); i++){
		if (shape[rx[i]].color != UNCOLORABLE && shape[rx[i]].left_x < box_lx)
			box_lx = shape[rx[i]].left_x;
		if (shape[uy[i]].color != UNCOLORABLE && shape[uy[i]].down_y < box_dy)
			box_dy = shape[uy[i]].down_y;
	}
	// testPrint();
}

/* overriding <<, thus you can do cout << ss; */
ostream & operator << ( ostream &output, const Shapes &ss ){
	output << "ALPHA=" << x_spacing << endl;
	output << "BETA=" << y_spacing << endl;
	output << "OMEGA=" << windowsize << endl;
	for (auto s : ss.shape) output << s.left_x << ',' << s.down_y << ',' << s.right_x << ',' << s.up_y << endl;
    return output;
}
