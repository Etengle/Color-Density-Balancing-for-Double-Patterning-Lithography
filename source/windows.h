#ifndef __WINDOWS_H__
#define __WINDOWS_H__
#include "graphs.h"
#include "shapes.h"
#include "window.h"
#include <vector>
#include <iostream>

class Windows {

    friend std::ostream & operator << (std::ostream &, const Windows &);
    private:
        std::vector<Window> window;
        void testPrint(std::vector<Window> &);
    public:
        Windows (){}
        ~Windows (){}
        void createWindow();
        void countArea(std::vector<Shape> &, std::vector<Graph> &);
        void printResult(std::vector<Shape> &, std::vector<Graph> &);
        std::vector<Window>& getWindows();
};

#endif
