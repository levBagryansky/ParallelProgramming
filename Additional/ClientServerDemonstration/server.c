#include "mpi.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    printf("argv[0] = %s\n", argv[1]);
    // Open port.
    char port_name[MPI_MAX_PORT_NAME];
    MPI_Open_port(MPI_INFO_NULL, port_name);

    // Publish port name and accept client.
    MPI_Comm client;
    MPI_Publish_name("name", MPI_INFO_NULL, port_name);
    MPI_Comm_accept(port_name, MPI_INFO_NULL, 0, MPI_COMM_WORLD, &client);

    // Receive data from client.
    int recvbuf;
    MPI_Recv(&recvbuf, 1, MPI_INT, 0, 0, client, MPI_STATUS_IGNORE);
    printf("recvbuf = %d\n", recvbuf);

    MPI_Unpublish_name("name", MPI_INFO_NULL, port_name);

    MPI_Finalize();
    return 0;
}