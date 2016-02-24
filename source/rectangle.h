#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__
#include <cmath>

class Rectangle {

    public:
        Rectangle (int &lx, int &dy, int &rx, int &uy)
        : left_x(lx), down_y(dy), right_x(rx), up_y(uy){

        }
        ~Rectangle (){}
        bool intersect(Rectangle &rec){

            int t_r = this->right_x;
            int t_l = this->left_x;
            int t_u = this->up_y;
            int t_d = this->down_y;

            int r_r = rec.right_x;
            int r_l = rec.left_x;
            int r_u = rec.up_y;
            int r_d = rec.down_y;

            return (r_r > t_l) && (t_r > r_l) && (r_u > t_d) && (t_u > r_d);
        }

        int intersect_Area(Rectangle &rec){

            int t_r = this->right_x;
            int t_l = this->left_x;
            int t_u = this->up_y;
            int t_d = this->down_y;

            int r_r = rec.right_x;
            int r_l = rec.left_x;
            int r_u = rec.up_y;
            int r_d = rec.down_y;

            //printf("t_r : %d, t_l : %d, t_u : %d, t_d : %d\n", t_r, t_l, t_u, t_d);
            //printf("r_r : %d, r_l : %d, r_u : %d, r_d : %d\n", r_r, r_l, r_u, r_d);

            if (t_r > r_r) t_r = r_r;
            if (t_l < r_l) t_l = r_l;
            if (t_u > r_u) t_u = r_u;
            if (t_d < r_d) t_d = r_d;
            //printf("t_r : %d, t_l : %d, t_u : %d, t_d : %d\n", t_r, t_l, t_u, t_d);

            t_r = t_r - t_l;
            t_u = t_u - t_d;
            //printf("t_r : %d, t_u : %d, t_r*t_u : %d\n", t_r, t_u, t_r * t_u);
            if (t_r <= 0 || t_u <= 0) return 0;
            return t_r * t_u;
        }

        int left_x, down_y, right_x, up_y;

};

#endif
