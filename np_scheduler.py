#! /usr/bin/python
#
# Scheduler for np proces loading lines
#

import sys
from mpi4py import MPI
import threading

# default BLK_SZ is 8, total tasks must be np * BLK_SZ
# use BLK_SZ to make np = 1 still works(to test performance)
BLK_SZ = 8

mutex = threading.Lock()

def scheduler_load(comm, loads, host = 0):
  '''
  Schedule loading lines for each proces

  Notes
  -----
  By default, BLK_SZ is 8. So there are np * 8 tasks in total.
  Procs host loads lines from loads[0-7] while other procs loads lines in unordered way

  Parameters
  ----------
  comm : communicator of MPI prog  
  loads : loads list generated from fns_partition function in load.py  
  host : host procs, 0 by default
  
  Returns
  -------
  ret : lines list
  
  Examples
  --------
  >>> comm = MPI.COMM_WORLD
  >>> rank = comm.Get_rank()
  >>> from load import fns_partition
  >>> loads = fns_partition(['a.txt'], sz)
  >>> lst = scheduler_load(comm, loads)
  >>> if rank == 0:
  >>>   print lst
  '''
  ret = []
  rank = comm.Get_rank()
  size = comm.Get_size()
  cnt = BLK_SZ - 1
 
  if rank == host:
    # load tasks [0,BLK_SZ-1]
    for i in xrange(BLK_SZ):
      lines = loads[i]()
      for line in lines:
         ret.append(line)
  if rank != host:
    for i in xrange(BLK_SZ):
      comm.send(rank, host, 2013)
      cnt = comm.recv(source = host, tag = 2013)
      #print 'rank %d received from host with cnt val %d' % (rank, cnt)
      # loading lines
      lines = loads[cnt]()
      for line in lines:
        ret.append(line)
  else:
    while cnt < (BLK_SZ * size - 1):
      global mutex
      mutex.acquire()
      stat = MPI.Status()
      tmp = comm.recv(source = MPI.ANY_SOURCE, tag = MPI.ANY_TAG, status = stat)
      cnt += 1
      src = stat.Get_source()
      comm.send(cnt, src, 2013)
      mutex.release()
  return ret

if __name__ == '__main__':
  comm = MPI.COMM_WORLD
  from load import fns_partition
  rank = comm.Get_rank()
  sz = comm.Get_size()
  loads = fns_partition(['a.txt'], sz)
  print len(loads)
  aa = scheduler_load(comm, loads)
  if rank == 0:
    print aa
