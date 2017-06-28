#!/usr/bin/python

#******************************************************************************************************************
#FILE NAME = Rastergraphic.py
#Written by: Kaleb Alfaro Badilla.
#This script creates a 2D graphic that represents the axonal voltage output from the network.
#CREATED = 2016
#LAST ACTUALIZATION = 2017
#Python 2.7
#******************************************************************************************************************

#Libraries
#------------------------------o----------------------------#
import matplotlib
matplotlib.use('Agg')
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors
import sys
#------------------------------o----------------------------#

graph= int(sys.argv[1])		#arguments

steps = 0
x = []
v = []
y = []
f = open('dataG.txt','r')

if(graph==1):				#plot the graph according data in the file
	for line in f.readlines():
		ln_spl = line.split()
		v += [float(num) for num in ln_spl]
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1

	f.close()
else:					#plot a empty graph 
	for line in f.readlines():
		ln_spl = line.split()
		v += [0 for num in ln_spl]
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1
	
nrows, ncols = steps,int(sys.argv[2])		#arguments
v = np.asarray(v)
x = np.asarray(x)
y = np.asarray(y)
grid = v.reshape((nrows,ncols))

#It's created the figure according the data and the color assignation.
plt.figure(figsize=(10.5,4.5))
plt.imshow(grid, extent=(x.min(), x.max(), y.max(), y.min()),interpolation = 'nearest', cmap= 'gnuplot',aspect='auto')
cbar = plt.colorbar()			#the sidebar is placed
#it's added a title label and axis labels to the figure.
cbar.set_label('Axon output voltage')
if (graph==0):
	plt.title('No Response')
else:
	plt.title('Raster plot of axon responses for neuron population')
plt.xlabel('Neuron enumeration')
plt.ylabel('Time steps')
plt.savefig('images/Network.png')	#this command saves a png figure to show it in the website

#******************************************************************************************************************
#FILE NAME = Rastergraphic.py
#Written by: Kaleb Alfaro Badilla.
#This script creates a 2D graphic that represents the axonal voltage output from the network.
#CREATED = 2016
#LAST ACTUALIZATION = 2017
#Python 2.7
#******************************************************************************************************************

