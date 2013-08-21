#! /usr/bin/python
#
# normalize data with (entity - feature_min) / (feature_max - feature_min)
#

def norm(filename, outfile, fmt = 'csv'):
  f = open(filename)
  srtlinelst = ''
  if fmt == 'csv':
    sp = ','
  if fmt == 'txt':
    sp = '\t'

  srtlinelst = f.readline().strip('\n').split(sp)
  minlst = [float(item) for item in srtlinelst[1:]]
  maxlst = [float(item) for item in srtlinelst[1:]]

  for line in f:
    l = line.strip('\n').split(sp)[1:]
    for i in xrange(len(l[1:])):
      if float(l[i]) < minlst[i]:
        minlst[i] = float(l[i])
      if float(l[i]) > maxlst[i]:
        maxlst[i] = float(l[i])

  f.seek(0)

  f2 = open(outfile, 'w')
  for line in f:
    l = line.strip('\n').split(sp)
    f2.write(l[0])
    f2.write(sp)
    l = l[1:]
    for i in xrange(len(minlst)):
      if maxlst[i] == minlst[i]:
        stf = maxlst[i]
        f2.write(str(stf))
	if i != len(minlst) - 1:
          f2.write(sp)
      else:
        stf = (float(l[i]) - minlst[i]) / (maxlst[i] - minlst[i])
        f2.write(str(stf))
	if i != len(minlst) - 1:
          f2.write(sp)
    f2.write('\n')
    
if __name__ == '__main__':
  from dpark import optParser
  optParser.add_option('--in', dest='inputf')
  optParser.add_option('--out', dest='outputf')
  options, args = optParser.parse_args()
  inf = options.inputf
  outf = options.outputf
  norm(inf, outf)
