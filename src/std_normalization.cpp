#include <vector>
#include <iostream>

#include <std_normalization.h>
#include <my_math.h>

using namespace std;

void normalize_column_std(vector<double>& matrix, 
                         const int nrow,
                         const int ncol,
                         const int col){
    pair<double, double> avg_and_std = find_column_avg_and_std(matrix, nrow, ncol, col);
    double avg = avg_and_std.first;
    double std = avg_and_std.second;
    for(int row = 0; row < nrow; ++row){
        matrix[addr(row, col, ncol)] = (matrix[addr(row, col, ncol)] - avg)/std;
    }
}


pair<double, double> find_column_avg_and_std(const vector<double>& matrix, 
                                             const int nrow,
                                             const int ncol,
                                             const int col){
    double sum = 0;
    double square_sum = 0;
    for(int i = 0; i < nrow; ++i) {
       sum += matrix[addr(i, col, ncol)];
       square_sum += matrix[addr(i, col, ncol)] * matrix[addr(i, col, ncol)];
    }
    double avg = sum / nrow;
    double variance = square_sum / nrow - avg * avg;
    return make_pair(avg, sqrt(variance));
}

