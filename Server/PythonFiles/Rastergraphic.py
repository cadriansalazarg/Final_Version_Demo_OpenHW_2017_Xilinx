#!/usr/bin/python

#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.
#Copyright 2017 Erasmus Brain Project
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#******************************************************************************************************************
#FILE NAME = Rastergraphic.py
#Written by: Kaleb Alfaro-Badilla.
#This script creates a 2D graphic that represents the axonal voltage output from the network.
#CREATED = 2016
#LAST ACTUALIZATION = 2017
#Python 2.7
#******************************************************************************************************************

#Libraries
#------------------------------o----------------------------#
import matplotlib
matplotlib.use('Agg') #compatibility with Apache
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors
import sys
#------------------------------o----------------------------#

graph= int(sys.argv[1]) #server arg, arg==1 if desire to plot

steps = 0
x = [] #list used to allocate the enumaration of cells, x axis
y = [] #list used to save the time step of the simulation
v = [] #list where is transfered the cell network voltage output

f = open('dataG.txt','r') # opening file where is allocated the response

if(graph==1):	#plot the graph according data in the file
	for line in f.readlines():
		ln_spl = line.split()
		v += [float(num) for num in ln_spl]
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1

	f.close()
else:			#plot a empty graph 
	for line in f.readlines():
		ln_spl = line.split()
		v += [0 for num in ln_spl]
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1
	
nrows, ncols = steps,int(sys.argv[2])	#second arguments means the number of cells to plot
#translating the lists to numpy array
v = np.asarray(v)
x = np.asarray(x)
y = np.asarray(y)
grid = v.reshape((nrows,ncols)) #matrix of colors

#It is created the figure according the data and the color assignation.
plt.figure(figsize=(10.5,4.5))
plt.imshow(grid, extent=(x.min(), x.max(), y.max(), y.min()),interpolation = 'nearest', cmap= 'gnuplot',aspect='auto')
cbar = plt.colorbar()	#the sidebar is placed in the figure
#it is added a title label and axis labels to the figure.
cbar.set_label('Axon output voltage')
if (graph==0):
d 	plt.title('No Response')
else:
	plt.title('Raster plot of axon responses for neuron population')
plt.xlabel('Neuron enumeration')
plt.ylabel('Time steps')
plt.savefig('images/Network.png')	#this command saves a png figure to show it in the website

#******************************************************************************************************************
#FILE NAME = Rastergraphic.py
#Written by: Kaleb Alfaro-Badilla.
#This script creates a 2D graphic that represents the axonal voltage output from the network.
#CREATED = 2016
#LAST ACTUALIZATION = 2017
#Python 2.7
#******************************************************************************************************************

