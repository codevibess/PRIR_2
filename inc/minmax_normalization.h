#ifndef MINMAX
#define MINMAX

#include <vector>

void normalize_column_minmax(std::vector<double>& matrix,
                             const int nrow,
                             const int ncol,
                             const int col);
                             
std::pair<double, double> find_column_min_and_max(const std::vector<double>& matrix, 
                                                  const int nrow,
                                                  const int ncol,
                                                  const int col); 
    
#endif
