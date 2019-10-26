#ifndef STDNORM
#define STDNORM

#include <vector>

void normalize_column_std(std::vector<double>& matrix,
                          const int nrow,
                          const int ncol,
                          const int col);

std::pair<double, double> find_column_avg_and_std(const std::vector<double>& matrix, 
                                                  const int nrow,
                                                  const int ncol,
                                                  const int col); 
    
#endif
