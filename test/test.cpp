// #include <mpi.h>
// #include <stdio.h>

// int main(int argc, char** argv) {
//       printf("hello darkness");
//     // Initialize the MPI environment
//     MPI_Init(NULL, NULL);

//     // Get the number of processes
//     int world_size;
//     MPI_Comm_size(MPI_COMM_WORLD, &world_size);

//     // Get the rank of the process
//     int world_rank;
//     MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

//     // Get the name of the processor
//     char processor_name[MPI_MAX_PROCESSOR_NAME];
//     int name_len;
//     MPI_Get_processor_name(processor_name, &name_len);


//     printf("how many times");
//     MPI_Finalize();
//     // Print off a hello world message
//     printf("Hello world from processor %s, rank %d out of %d processors\n",
//            processor_name, world_rank, world_size);

//     // Finalize the MPI environment.
   
// }


#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv){
    int process_Rank, size_Of_Cluster;

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size_Of_Cluster);
    MPI_Comm_rank(MPI_COMM_WORLD, &process_Rank);

    printf("Hello World from process %d of %d\n", process_Rank, size_Of_Cluster);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Finalize();
    return 0;
}