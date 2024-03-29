#include <vector>
#include <iostream>
#include <limits>
#include <mpi.h>

#include <csv_reading.h>
#include <minmax_normalization.h>
#include <std_normalization.h>
#include <my_math.h>

using namespace std;

int knn(const vector<double>& features,
        const vector<int>& labels,
        const int nrow,
        const int ncol,
        const vector<double>::const_iterator sample_to_fit_begin,
        const vector<double>::const_iterator sample_to_fit_end);

double accuracy(const vector<int>& labels,
                const vector<int>& predicted_labels);

void print_matrix(const vector<int>& matrix, int nrow, int ncol);
void print_matrix(const vector<double>& matrix, int nrow, int ncol);

const double train_ratio = 0.8;


double measure_minmax_normalization(vector<double>& signals_minmax, 
                                  const int nrow,
                                  const int ncol,
                                  int world_size,
                                  int world_rank){
    double begin = MPI_Wtime();
    // //#pragma omp parallel for default(shared)
    int partition =  ncol/ world_size;
    for(int col = world_rank * partition; col < world_rank * partition + partition; col++){
        normalize_column_minmax(signals_minmax, nrow, ncol, col);
    }
    double end = MPI_Wtime();
    double elapsed_secs = end - begin;
    return elapsed_secs;
}

double measure_std_normalization(vector<double>& signals_std, 
                                  const int nrow,
                                  const int ncol,
                                  int world_size,
                                  int world_rank){
    double begin = MPI_Wtime();
    //#pragma omp parallel for default(shared)
    int partition =  ncol/ world_size;
    for(int col = world_rank * partition; col < world_rank * partition + partition; col++){
        normalize_column_std(signals_std, nrow, ncol, col);
    }
    double end = MPI_Wtime();
    double elapsed_secs = end - begin;
    return elapsed_secs;
}
double measure_knn(const vector<double>& train_features,
                 const vector<int>& train_labels,
                 const int train_nrow,
                 const int ncol,
                 const vector<double>& test_features,
                 const vector<int>& test_labels,
                 vector<int>& predicted_labels,
                 int world_size,
                 int world_rank){
    double begin = MPI_Wtime();
    int partition =  (int)test_labels.size() / world_size;
    //#pragma omp parallel for default(shared)
    for(int i = world_rank * partition; i < world_rank * partition + partition; ++i){
        predicted_labels[i] =  knn(train_features,
                                   train_labels,
                                   train_nrow,
                                   ncol,
                                   test_features.cbegin() + i * ncol,
                                   test_features.cbegin() + (i + 1) * ncol);
    }
    double end = MPI_Wtime();
    double elapsed_secs = end - begin;
    return elapsed_secs;
}

int main(int argc, char **argv){
    const int ncol = 11;
    const int nrow = 6497;
    double minmax_normalization_time_local = 0;
    double std_normalization_time_local = 0;
    double minmax_knn_time_local = 0;
    double minmax_std_time_local = 0;
    double minmax_knn_acc_local;
    double minmax_std_acc_local;


    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    
    if(world_rank == 0) {
    cout << "Setting number of threads to " << world_size << endl;
    }

    
    vector<double> signals_minmax = read_csv<double>("data/train_signals.csv", 
                                                        nrow,
                                                        ncol);
    const vector<int> labels = read_csv<int>("data/train_labels.csv", 
                                                nrow,
                                                1);
    vector<double> signals_std = signals_minmax;
    
    minmax_normalization_time_local += measure_minmax_normalization(signals_minmax, nrow, ncol, world_size, world_rank);
    std_normalization_time_local += measure_std_normalization(signals_std, nrow, ncol, world_size, world_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    double minmax_normalization_time = 0;
    double std_normalization_time = 0;
    MPI_Allreduce(&minmax_normalization_time_local, &minmax_normalization_time, 2, MPI_DOUBLE, MPI_SUM,
           MPI_COMM_WORLD);
    MPI_Allreduce(&std_normalization_time_local, &std_normalization_time, 2, MPI_DOUBLE, MPI_SUM,
           MPI_COMM_WORLD);
    
    //Split rows
    const int train_nrow = train_ratio * nrow;
    vector<double> train_features_minmax(0);
    vector<double> train_features_std(0);
    vector<int> train_labels(0);
    vector<double> test_features_minmax(0);
    vector<double> test_features_std(0);
    vector<int> test_labels(0);
    for(int row = 0; row < train_nrow; row++){
        train_labels.push_back(labels[row]);
        for(int col = 0; col < ncol; col++){
            train_features_minmax.push_back(signals_minmax[addr(row, col, ncol)]);
            train_features_std.push_back(signals_std[addr(row, col, ncol)]);
        }
    }
    for(int i = train_nrow; i < nrow ; i++){
        test_labels.push_back(labels[i]);
        for(int col = 0; col < ncol; col++){
            test_features_minmax.push_back(signals_minmax[addr(i, col, ncol)]);
            test_features_std.push_back(signals_std[addr(i, col, ncol)]);
        }
    }
    vector<int> predicted_labels_minmax(test_labels.size());
    vector<int> predicted_labels_std(test_labels.size());
    
    minmax_knn_time_local += measure_knn(train_features_minmax,
                                    train_labels,
                                    train_nrow,
                                    ncol,
                                    test_features_minmax,
                                    test_labels,
                                    predicted_labels_minmax,
                                    world_size,
                                    world_rank);

    MPI_Barrier(MPI_COMM_WORLD);
    double minmax_knn_time = 0;
    double minmax_knn_acc = 0;

    minmax_knn_acc_local = accuracy(test_labels, predicted_labels_minmax);
    MPI_Allreduce(&minmax_knn_time_local, &minmax_knn_time, 2, MPI_DOUBLE, MPI_SUM,
           MPI_COMM_WORLD);
    MPI_Allreduce(&minmax_knn_acc_local, &minmax_knn_acc, 2, MPI_DOUBLE, MPI_SUM,
           MPI_COMM_WORLD);

  

    
    
    minmax_std_time_local += measure_knn(train_features_std,
                                    train_labels,
                                    train_nrow,
                                    ncol,
                                    test_features_std,
                                    test_labels,
                                    predicted_labels_std,
                                    world_size,
                                    world_rank);

    minmax_std_acc_local = accuracy(test_labels, 
                                    predicted_labels_std);

    MPI_Barrier(MPI_COMM_WORLD);
    double minmax_std_time = 0;
    double minmax_std_acc = 0;
    MPI_Allreduce(&minmax_std_time_local, &minmax_std_time, 2, MPI_DOUBLE, MPI_SUM,
           MPI_COMM_WORLD);  
    MPI_Allreduce(&minmax_std_acc_local, &minmax_std_acc, 2, MPI_DOUBLE, MPI_SUM,
           MPI_COMM_WORLD);                              
    if(world_rank == 0){
    cout << "Minmax normalization time, Std normalization time, Minmax knn time, Minmax std time, Minmax accuracy, Std accuracy" << endl;
    cout << minmax_normalization_time << ", " << std_normalization_time
         << ", " << minmax_knn_time << ", " << minmax_std_time
         << ", " << minmax_knn_acc
         << ", " << minmax_std_acc << endl;
    }
    MPI_Finalize();
    return 0;
}

int knn(const vector<double>& features,
        const vector<int>& labels,
        const int nrow,
        const int ncol,
        const vector<double>::const_iterator sample_to_fit_begin,
        const vector<double>::const_iterator sample_to_fit_end){
    double min_distance = numeric_limits<double>::max();
    int nearest_neighbour_row_index = -1;
    int row_index = 0;
    const vector<double>::const_iterator feature_it = features.begin();
    while(row_index < nrow){
        const vector<double>::const_iterator row_it_begin = feature_it + addr(row_index, 0, ncol);
        const vector<double>::const_iterator row_it_end = row_it_begin + ncol;
        double sample_to_row_distance = euclidean(row_it_begin,
                                                  row_it_end,
                                                  sample_to_fit_begin,
                                                  sample_to_fit_end);
        if(sample_to_row_distance < min_distance){
            min_distance = sample_to_row_distance;
            nearest_neighbour_row_index = row_index;
        }
        ++row_index;
    }
    return labels[nearest_neighbour_row_index];
}

double accuracy(const vector<int>& labels,
                const vector<int>& predicted_labels){
    int hits = 0;
    int total = labels.size();
    for(int i = 0; i < (int)labels.size(); i++){
        if(labels[i] == predicted_labels[i]){
             ++hits;
        }
    }
    return (double)hits/total;
}

void print_matrix(const vector<int>& matrix, int nrow, int ncol){
    cout << "matrix size: " << matrix.size() << endl;
    for(int i = 0; i < nrow; i++){
        if(i < 5 or i > nrow - 6){
            cout << "Row " << i;
            for(int col = 0; col < ncol; col++){
                cout << ": " << matrix[addr(i, col, ncol)] << ", ";
            }
        }
    }
}

void print_matrix(const vector<double>& matrix, int nrow, int ncol){
    cout << "matrix size: " << matrix.size() << endl;
    for(int i = 0; i < nrow; i++){
        if(i < 5 or i > nrow - 6){
            cout << "Row " << i << ": ";
            for(int col = 0; col < ncol; col++){
                cout << matrix[addr(i, col, ncol)] << ", ";
            }
            cout << endl;
        }
    }
}
