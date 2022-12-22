#include <stdlib.h>
#include <time.h>
#include "io.h"
#include "compute.h"
#include "mpi.h"


#define TRUE 1
#define FALSE 0
#define MASTER 0


int main(int argc, char* argv[])
{
    int rank, mpi_size;     // MPI
    
    int file_size;          // file lines
    int* file_buffer;       // entire file (input n output)
    int* buffer;            // process buffer
    int N;                  // size of process buffer

    time_t begin, end;
    double diff;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);

    if (rank == MASTER) {
        // get file size (in lines)
        file_size = get_input_size();

        // calc process buffer size
        N = file_size / mpi_size;

        // read input file
        file_buffer = (int*) malloc (file_size * sizeof(int));
        read_file(file_buffer, file_size);
    }

    // broadcast process buffer size
    MPI_Bcast(&N, 1, MPI_INT, MASTER, MPI_COMM_WORLD);

    // init buffer
    buffer = (int*) malloc (N * sizeof(int));

    // scatter input values 
    MPI_Scatter(file_buffer, N, MPI_INT, buffer, N, MPI_INT, MASTER, MPI_COMM_WORLD);

    // compute process buffered values and get time begin/end
    begin = time(NULL);
        compute_array(buffer, N);
    end = time(NULL);

    // gather output values
    MPI_Gather(buffer, N, MPI_INT, file_buffer, N, MPI_INT, MASTER, MPI_COMM_WORLD);
    
    if (rank == MASTER)
        // write output values to file
        write_file(file_buffer, file_size);
    
    
    // send time to MASTER to write in file
    diff = (double)(end - begin);
    MPI_Send(&diff, 1, MPI_DOUBLE, MASTER, 0, MPI_COMM_WORLD);

    if (rank == MASTER){
        double recv;    // receive data
        
        // receive process time spent and writes to file
        for (int i = 0; i < mpi_size; i++){
            MPI_Recv(&recv, 1, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            write_p_time(recv, i);
        }
    }
    
    MPI_Finalize();
}
