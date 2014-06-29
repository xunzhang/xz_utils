#! /usr/bin/python
#
# usage of relevant(used for blocked matrix) parallel lines loading
#

from mpi4py import MPI

if __name__ == '__main__':
  comm = MPI.COMM_WORLD
  from load import fns_partition
  from expand import expd_f_lst
  from np_scheduler import scheduler_load, exchange
  from hashtransfer import putlines
  # a dictory name
  fl = '/home/xunzhang/demo'
  fns = expd_f_lst(fl)
  rank = comm.Get_rank()
  sz = comm.Get_size()
  loads = fns_partition(fns, sz) 
  # get lines 
  lines = scheduler_load(comm, loads)
  # get a list of list
  slotslst = putlines(lines, sz)
  ret = exchange(slotslst, comm)
  print ret
