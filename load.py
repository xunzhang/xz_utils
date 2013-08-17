#! /usr/bin/python
#
# load file(s) line by line
#

def f_ld_lines(fn, st, en):
  '''
  Load lines of file fn between st and en(offset)
 
  Notes
  -----
  Real lines are loaded in the range of [st + edge_sz1, en + edge_sz2]

  Parameters
  ----------
  fn : filename
  st : start offset
  en : end offset
  
  Returns
  -------
  l : Interable lines

  Examples
  --------
  >>> lines = f_ld_lines(fn, st, en)
  >>> for line in lines:
  >>>   print line
  '''
  f = open(fn, 'rb')
  offset = st
  if offset:
    f.seek(offset - 1)
    l = f.readline()
    # add edge offset. if no edge, add 1:'\n'
    offset += len(l) - 1
  while offset < en:
    l = f.readline()
    offset += len(l)
    yield l

def fn_partition(fn, np):
  '''
  Patition file in np blocks/chunks

  Notes
  -----
  the interval is compact: [0, size / np], [size / np, 2 * (size / np)], ...

  Parameters
  ----------
  fn : filename
  np : block size/chunk number

  Returns
  -------
  func_loaders : a list of partial load funcs(loaders)

  Examples
  --------
  >>> loads = fn_partition('demo.txt', 8)
  >>> for it in loads:
  >>>   lines = it()
  >>>   # it is the return of function fd_ld_lines
  >>>   for line in lines:
  >>>     print line
  >>>   print '----'
  '''
  func_loaders = []
  import os
  import functools
  sz = os.stat(fn).st_size
  nbk = np
  bk_sz = sz / nbk
  for i in xrange(nbk):
    s = i * bk_sz
    if i == nbk - 1:
      e = sz
    else:
      e = (i + 1) * bk_sz
    #print i
    #print s, e
    func_loaders.append(functools.partial(f_ld_lines, fn, s, e))
  return func_loaders

#def expd_f_lst(fns):
def fs_ld_lines(fn, displs, st, en):
  '''
  Load lines of files between st and en(offset), file_sz is seperated by displs
 
  Notes
  -----
  Real lines are loaded in the range of [st + edge_sz1, en + edge_sz2]
  interval can be seen between files

  Parameters
  ----------
  fn : filename
  displs : seperate files by sz
  st : start offset
  en : end offset
  
  Returns
  -------
  l : Interable lines

  Examples
  --------
  >>> lines = f_ld_lines(fn, st, en)
  >>> for line in lines:
  >>>   print line
  '''
  for f in xrange():
  
  
  f = open(fn, 'rb')
  offset = st
  if offset:
    f.seek(offset - 1)
    l = f.readline()
    # add edge offset. if no edge, add 1:'\n'
    offset += len(l) - 1
  while offset < en:
    l = f.readline()
    offset += len(l)
    yield l

def fns_partition(fns, np):
  '''
  Patition files in np blocks/chunks

  Notes
  -----
  the interval is compact: [0, size / np], [size / np, 2 * (size / np)], ...
  ?Why return loads: can be parallelize because list contains a mapping between index to load

  Parameters
  ----------
  fns : filename list
  np : block size/chunk number
  use_func : user-defined function(define to tackle a line)

  Returns
  -------
  func_loaders : a list of partial load funcs(loaders)

  Examples
  --------
  >>> fns = ['a.txt', 'b.txt', 'c.txt']
  >>> loads = fns_partition(fns, 8)
  >>> for it in loads:
  >>>   lines = it()
  >>>   # it is the return of function fd_ld_lines
  >>>   for line in lines:
  >>>     print line
  >>>   print '----'
  '''
  func_loaders = []
  displs = [0] * (len(fns) + 1)
  import os
  import functools
  for i in xrange(len(displs) - 1):
    displs[i + 1] = displs[i] + os.stat(fns[i]).st_size
  sz = displs[-1]
  nbk = np
  bk_sz = sz / np
  for i in xrange(nbk):
    s = i * bk_sz
    if i == nbk - 1:
      e = sz
    else:
      e = (i + 1) * bk_sz
    func_loaders.append(functools.partial(fs_ld_lines, fn, displs, s, e))
  return func_loaders

if __name__ == '__main__':
  fns = ['a.txt', 'b.txt'] 
  loads = fns_partition(fns, 2)
  #loads = fn_partition('test.txt', 4)
  for it in loads:
    lines = it()
    for line in lines:
      print line
    print '----'
