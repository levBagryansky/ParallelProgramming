#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

int main(int argc, char* argv[])
{
    MPI_Init(&argc, &argv);

    int my_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    MPI_File_delete("file1.txt", MPI_SUCCESS);
    MPI_File handle;
    int access_mode = MPI_MODE_CREATE /* Create the file if it does not exist */
                      | MPI_MODE_RDWR /* With read / write access */
                      | MPI_MODE_UNIQUE_OPEN; /* The file will not be opened concurrently elsewhere */
    if(MPI_File_open(MPI_COMM_WORLD, "file.txt", access_mode, MPI_INFO_NULL, &handle) != MPI_SUCCESS)
    {
        printf("[MPI process %d] Failure in opening the file.\n", my_rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    printf("[MPI process %d] File opened successfully.\n", my_rank);
    //char buf = '1' + my_rank;
    int buf_size = 4;
    char buf[buf_size];
    sprintf(buf, "%.2d, ", my_rank);
    int offset = buf_size *  (world_size - my_rank - 1);
    printf("[MPI process %d] offset = %i.\n", my_rank, offset);
    MPI_Status status;
    if (MPI_File_write_at(handle, offset, buf, buf_size, MPI_CHAR, &status)) {
        printf("[MPI process %d] Failure in writing to the file.\n", my_rank);
    }
    if(MPI_File_close(&handle) != MPI_SUCCESS)
    {
        printf("[MPI process %d] Failure in closing the file.\n", my_rank);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
    }

    printf("[MPI process %d] File closed successfully.\n", my_rank);

    MPI_Finalize();

    return EXIT_SUCCESS;
}