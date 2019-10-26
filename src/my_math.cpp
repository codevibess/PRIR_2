#include <vector>
#include <stdexcept>
#include <iostream>

#include <csv_reading.h>
#include <minmax_normalization.h>
#include <std_normalization.h>
#include <my_math.h>

using namespace std;

int addr(const int row, const int col, const int ncol){
    return row * ncol + col;
}

int max(const int a1, const int a2){
    if(a1 > a2) return a1;
    else        return a2;
}

int min(const int a1, const int a2){
    if(a1 > a2) return a2;
    else        return a1;
}
 
double euclidean(const vector<double>::const_iterator x1_begin,
                 const vector<double>::const_iterator x1_end,
                 const vector<double>::const_iterator x2_begin,
                 const vector<double>::const_iterator x2_end){
    double diff_squares_sum = 0;
    vector<double>::const_iterator x1_it = x1_begin;
    vector<double>::const_iterator x2_it = x2_begin;
    while(x1_it != x1_end and x2_it != x2_end){
        diff_squares_sum += (*x1_it - *x2_it) * (*x1_it - *x2_it);
        ++x1_it;
        ++x2_it;
    }
    return sqrt(diff_squares_sum);
}

double sqrt(double number){
    double error = 0.00001; //define the precision of your result
    double s = number;
    while ((s - number / s) > error){
        s = (s + number / s) / 2;
    }
    return s;
}
