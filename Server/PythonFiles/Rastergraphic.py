#!/usr/bin/python


#Libraries
#------------------------------o----------------------------#
import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors
import sys
#------------------------------o----------------------------#


steps = 0
x = []
v = []
y = []
f = open('dataG.txt','r')
graph= int(sys.argv[1])
if(graph==1):
	for line in f.readlines():
		ln_spl = line.split()
		v += [float(num) for num in ln_spl]
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1

	f.close()
else:
	for line in f.readlines():
		ln_spl = line.split()
		v += [0 for num in ln_spl]
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1
	
nrows, ncols = steps,int(sys.argv[2])
v = np.asarray(v)
x = np.asarray(x)
y = np.asarray(y)

grid = v.reshape((nrows,ncols))
plt.figure(figsize=(10.5,4.5))
plt.imshow(grid, extent=(x.min(), x.max(), y.max(), y.min()),interpolation = 'nearest', cmap= 'gnuplot',aspect='auto')
cbar = plt.colorbar()
cbar.set_label('Axon output voltage')
if (graph==0):
	plt.title('No Response')
else:
	plt.title('Raster plot of axon responses for neuron population')
plt.xlabel('Neuron enumeration')
plt.ylabel('Time steps')
plt.savefig('images/Network.png')
