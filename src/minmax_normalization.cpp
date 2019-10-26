#include <vector>
#include <iostream>

#include <minmax_normalization.h>
#include <my_math.h>

using namespace std;

void normalize_column_minmax(vector<double>& matrix, 
                             const int nrow, 
                             const int ncol, 
                             const int col){
    pair<double, double> min_and_max = find_column_min_and_max(matrix, nrow, ncol, col);
    double min = min_and_max.first;
    double max = min_and_max.second;
    for(int row = 0; row < nrow; ++row){
        matrix[addr(row, col, ncol)] = 
            (matrix[addr(row, col, ncol)] - min)/(max - min);
    }
}

pair<double, double> find_column_min_and_max(const vector<double>& matrix, 
                                             const int nrow,
                                             const int ncol,
                                             const int col){
    int i;
    double mx, mn;
    if(nrow % 2 == 0){
        if(matrix[addr(0, col, ncol)] > matrix[addr(1, col, ncol)]){
            mx = matrix[addr(0, col, ncol)];
            mn = matrix[addr(1, col, ncol)];
        } else{
            mx = matrix[addr(1, col, ncol)];
            mn = matrix[addr(0, col, ncol)];
        }
        i = 2;
    } else{
        mx = matrix[addr(0, col, ncol)];
        mn = mx;
        i = 1;
    }
    while(i < nrow - 1){
        if(matrix[addr(i, col, ncol)] < matrix[addr(i + 1, col, ncol)]){
            mx = max(mx, matrix[addr(i + 1, col, ncol)]);
            mn = min(mn, matrix[addr(i, col, ncol)]);
        } else{
            mx = max(mx, matrix[addr(i, col, ncol)]);
            mn = min(mn, matrix[addr(i + 1, col, ncol)]);
        }
        i += 2;
    }
    return make_pair(mn, mx);
}
