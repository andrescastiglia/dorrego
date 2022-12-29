#include <iostream>
#include <cstring>
#include <mpi/mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int tag = 0;
    char msg[20] = {};

    if ( rank == 0 ) {

        strcpy( msg, "hello world" );

        MPI_Send(msg, strlen(msg) + 1, MPI_CHAR, 1, tag, MPI_COMM_WORLD);

        std::cout << "sent " << rank << std::endl;

    } else if ( rank == 1 ) {

        MPI_Status status;

        std::cout << "wait " << rank << std::endl;

        MPI_Recv(msg, sizeof(msg), MPI_CHAR, 0, tag, MPI_COMM_WORLD, &status);

        std::cout << "received " << msg << " " << rank << std::endl;
    }

    MPI_Finalize();

    return EXIT_SUCCESS;
}