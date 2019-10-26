# rrir

Usage:

- $ git clone https://github.com/codevibess/PRIR_2.git
- $ cd prir_2
- $ make
- $ mpirun -np NUM_THREADS ./bin/knn 

NUM_THREADS should be an integer constant, e.g:
$ mpirun -np 4 ./bin/knn
P.S. Provide only the number of threads corresponding for number of cores at you machine.
