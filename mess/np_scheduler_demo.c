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
  int sum = BLK_SZ - 1;
  int cnt = BLK_SZ;
  int flag = 0;
  int k = 0;
  int ntasks = size * BLK_SZ;
  int bnd = ntasks + size - 2;
  int winner = -1;
  MPI_Status stat;
  pthread_mutex_t cnt_mutex = PTHREAD_MUTEX_INITIALIZER;

  if(rank == 0) {
    while(cnt < bnd) {
      cnt += 1;
      pthread_mutex_lock(&cnt_mutex);
      MPI_Recv(&a, 1, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &stat);
      //printf("recvd from %d and now sum is %d\n", stat.MPI_SOURCE, sum);
      if(flag == 0) 
        sum += 1;
      MPI_Send(&sum, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
      if((sum == ntasks - 1) && (flag == 0)) {
        MPI_Send(&flag, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
        flag = 1;
      } else {
        MPI_Send(&flag, 1, MPI_INT, stat.MPI_SOURCE, 0, MPI_COMM_WORLD);
      }
      pthread_mutex_unlock(&cnt_mutex);
    }
  } else {
    while(1) {
      if(flag == 1) break; 
      if(sum == size * BLK_SZ - 1) break;
      MPI_Send(&rank, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
      MPI_Recv(&sum, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
      MPI_Recv(&flag, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &stat);
      
      if(flag == 0)  printf("rank %d recved from host with sum %d\n", rank, sum);
      // loading lines
      //if(flag == 1 && winner == rank)
      //  printf("rank %d is loading index of %d\n", rank, sum);
      sleep(1);
    }
  }
  MPI_Finalize();
  return 0;
}
