#!/usr/bin/python

#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.
#Copyright <YEAR> <COPYRIGHT HOLDER>
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#/******************************************************************************
#* Server/PythonFiles/Rastergraphic.py
#* Python version: 2.7.12
#* Written by: Kaleb Alfaro 2017
#* 
#* This script generates a raster plot and saves it as a image
#* 
#*
#******************************************************************************/



#Libraries
#------------------------------o----------------------------#
import matplotlib
matplotlib.use('Agg') #Apache compatibility
import numpy as np
import matplotlib.pyplot as plt
from matplotlib import colors
import sys
#------------------------------o----------------------------#


steps = 0
x = [] # axis of number of cell
v = [] # axis of cell output (color)
y = [] # axis of time step
f = open('dataG.txt','r') #open output simulation file
graph= int(sys.argv[1]) #It receives a flag to plot
if(graph==1): #plot data
	for line in f.readlines():#fill lists
		ln_spl = line.split()
		v += [float(num) for num in ln_spl] 
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1

	f.close()
else: #default image generation if server desire 
	for line in f.readlines():#fill lists
		ln_spl = line.split()
		v += [0 for num in ln_spl]
		x += range(1,len(ln_spl)+1)
		y += [steps]*len(ln_spl)
		steps+=1
	
nrows, ncols = steps,int(sys.argv[2]) #cols represent the number of cells to plot
#adapt from list to numpy array
v = np.asarray(v) 
x = np.asarray(x)
y = np.asarray(y)

#plotting commands
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
#save plot as image
plt.savefig('images/Network.png')
