#! /usr/bin/python
#
# usage of crtblkmtx.py
#

if __name__ == '__main__':
  from mpi4py import MPI 
  from crtblkmtx import ge_blkmtx
  comm = MPI.COMM_WORLD
  rank = comm.Get_rank()
  fi = '/home/xunzhang/demo/'
  rmap, cmap, mtx = ge_blkmtx(fi, comm)
  if rank == 0:
    print rmap
    print cmap
    print mtx 

