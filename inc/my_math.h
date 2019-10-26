#ifndef MATH
#define MATH

#include <vector>

int addr(const int row, const int col, const int ncol);
int max(const int a1, const int a2);
int min(const int a1, const int a2);
double euclidean(const std::vector<double>::const_iterator x1_begin,
                 const std::vector<double>::const_iterator x1_end,
                 const std::vector<double>::const_iterator x2_begin,
                 const std::vector<double>::const_iterator x2_end);
double sqrt(const double m); 

#endif
