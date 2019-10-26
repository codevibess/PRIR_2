#ifndef CSV
#define CSV

#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <iostream>

#include <my_math.h>

template <typename T>
std::vector<T> read_csv(const std::string file_path, 
                        const int nrow,
                        const int ncol); 

template <typename T>
std::vector<T> read_csv(const std::string file_path, 
                        const int nrow,
                        const int ncol){
    std::vector<T> signals(nrow * ncol);
    std::ifstream train_stream(file_path);
    std::string line;
    int col = 0;
    int row = 0;
    while(std::getline(train_stream,line)){
        std::stringstream  line_stream(line);
        std::string        cell;
        while(std::getline(line_stream,cell,',')){
            if(std::is_same<T, double>::value){
                signals.at(addr(row, col, ncol)) = stod(cell);
            }
            else if(std::is_same<T, int>::value){
                signals.at(addr(row, col, ncol)) = stoi(cell);
            }
            ++col;
        }
        col = 0;
        ++row;
    }
    return signals;
} 

#endif
