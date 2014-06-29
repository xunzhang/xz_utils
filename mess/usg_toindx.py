#! /usr/bin/python
#
# Usage of toinds.py
#

if __name__ == '__main__': 
  from mpi4py import MPI
  from toinds import ind_mapping
  comm = MPI.COMM_WORLD
  rank = comm.Get_rank()
  slotslst = []
  if rank == 0:
    slotslst = [(64, 6, 3), (64, 42, 4)] 
  if rank == 1:
    slotslst = [(64, 21, 1)] 
  if rank == 2:
    slotslst = [(27, 28, 1), (27, 42, 2), (37, 42, 3), (29, 28, 1), (29, 6, 3)] 
  if rank == 3:
    slotslst = [(37, 21, 5)] 
  rm, cm, slotslst = ind_mapping(slotslst, comm)
  if rank == 0:
    print rm
    print cm
    print slotslst
