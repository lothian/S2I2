#include <mpi.h>
#include <iostream>

void server() {
  int rank, size;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);/* get current process id */
  MPI_Comm_size (MPI_COMM_WORLD, &size);/* get number of processes */

  int value = 0; // Value being counted
  int ndone = 0; // Keeps track of number of finished clients
  
  while (1) {
    MPI_Status status;
    int what;
    MPI_Recv(&what, 1, MPI_INT, MPI_ANY_SOURCE, 1, MPI_COMM_WORLD, &status);
    if (what == 1) {
      MPI_Send(&value, 1, MPI_INT, status.MPI_SOURCE, 1, MPI_COMM_WORLD);
      value++;
    }
    else if (what == 2) {
      ndone++;
      if (ndone == (size-1)) return;
    }
    else {
      MPI_Abort(MPI_COMM_WORLD, 99);
    }
  }
}

int get_next() {
  int value;
  const int what = 1; 
  MPI_Send(&what, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
  MPI_Recv(&value, 1, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  return value;
}

void send_done() {
  const int what = 2; 
  MPI_Send(&what, 1, MPI_INT, 0, 1, MPI_COMM_WORLD);
}
  
void client() {
  int rank;
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);/* get current process id */

  int sum;
  for (int i=0; i<10; i++) {
    int value = get_next();
    std::cout << "Process " << rank << " got " << value << std::endl;
  }

  send_done();
}

int main (int argc, char** argv)
{
  int rank;
  MPI_Init (&argc, &argv);/* starts MPI */
  MPI_Comm_rank (MPI_COMM_WORLD, &rank);/* get current process id */

  if (rank == 0) {
    server();
  }
  else {
    client();
  }

  MPI_Finalize();
  return 0;
}
  
