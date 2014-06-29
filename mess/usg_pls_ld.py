#! /usr/bin/python
#
# usage of parallel lines loading
#

from mpi4py import MPI

if __name__ == '__main__':
  comm = MPI.COMM_WORLD
  from load import fns_partition
  from expand import expd_f_lst
  from np_scheduler import scheduler_load
  # a dictory name
  fl = '/home/xunzhang/demo'
  fns = expd_f_lst(fl)
  rank = comm.Get_rank()
  sz = comm.Get_size()
  loads = fns_partition(fns, sz) 
  lst = scheduler_load(comm, loads)
  if rank == 0:
    print lst
