#include <mpi.h>
#include <stdio.h>
#include <pthread.h>

#define	BLK_SZ	8	

int main(int argc, char* argv[])
{
  int rank, size;
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank); 
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int a = 0;
  int sum = 7;
  int flag = 0;
  int k = 0;
  MPI_Status stat;
  pthread_mutex_t cnt_mutex = PTHREAD_MUTEX_INITIALIZER;
  if (rank != 0) {
    for(; k < BLK_SZ; ++k) {
      MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Recv(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
      printf("rank %d recved from host with sum %d\n", rank, sum);
      // loading lines
      sleep(1);
    }
  } else {
    while(sum < BLK_SZ * size - 1) {
    pthread_mutex_lock(&cnt_mutex);
    MPI_Recv(&a, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
    //printf("recvd from %d and now sum is %d\n", stat.MPI_SOURCE, sum);
    sum += 1;
    MPI_Send(&sum, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
    pthread_mutex_unlock(&cnt_mutex);
    }
  }
  MPI_Finalize();
  return 0;
}

