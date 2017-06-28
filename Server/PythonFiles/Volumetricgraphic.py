#!/usr/bin/python

#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.
#Copyright 2017 Erasmus Brain Project
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


#******************************************************************************************************************
#FILE NAME : Volumetricgraphic.py
#Written by: Kaleb Alfaro-Badilla.
#This script creates a 3D graphic that represents the axonal voltage output from the network.
#CREATED : 2017
#LAST ACTUALIZATION : 2017
#Python 2.7
#******************************************************************************************************************


#Libraries
#-------------------------o---------------------------#
import numpy as np
from pylab import *
from mpl_toolkits.mplot3d import Axes3D
import matplotlib.animation as animation
import matplotlib.pyplot as plt
import matplotlib.colors as colors
from matplotlib.widgets import Slider
import sys
import os
#-------------------------o---------------------------#

#os.system('rm *.png')

nsize = int(sys.argv[1]) #server gives size of the network

fig = plt.figure(figsize=(8,6))
ax = fig.add_subplot(111,projection='3d')


#coordinates of each dot
x=[]
y=[]
z=[]

#at this moment, only graphics of 100, 400 and 1000 cells are supported
#by visualization reasons 
if(nsize==100):
	x=list(range(0,5))*5*4
	for i in range(0,5):
		y+=[i]*5
	y=y*4
	for i in range(0,4):
		z+=[i]*5*5
elif(nsize==400):
	x=list(range(0,5))*10*8
	for i in range(0,10):
		y+=[i]*5
	y=y*8
	for i in range(0,8):
		z+=[i]*10*5
elif(nsize==1000):
	x=list(range(0,10))*10*10
	for i in range(0,10):
		y+=[i]*10
	y=y*10
	for i in range(0,10):
		z+=[i]*10*10

#adapting the lists of coordinates into numpy array	
xs = np.array(x)
ys = np.array(y)
zs = np.array(z)
#setting min and max voltages for color map
colmap = cm.ScalarMappable(norm=colors.Normalize(vmin=-80.,vmax=-50.),cmap=cm.gnuplot)
colmap.set_array(np.array([-80,-50]))
#adding color bar on the figure
cb = fig.colorbar(colmap)
cb.set_label('Axon output(V)')

img_num=0 #variable used for image enumaration
with open("dataG.txt") as fp: #opening file of network simulation output
	for i, line in enumerate(fp):
		
		if(i%500==0): #for each 500 steps of simulation, is created a image
			plt.cla()
			ax.set_title('Volumetric plot of axon responses\nfor neuron population')
			ax.text(10, 0, 0, "Step: "+str(i)+' out of '+'50000')
			ax.axes.get_xaxis().set_ticklabels([]) #Enumaration of axis is cleared
			ax.axes.get_yaxis().set_ticklabels([]) #Enumaration of axis is cleared
			ax.set_zticks([]) #Enumaration of axis is cleared
			four_dim = np.array([float(x) for x in line.split()])
			norm = plt.Normalize(vmin=-80.,vmax=-50.) #mapping from number to color
			colors = cm.gnuplot(norm(four_dim))
			yg = ax.scatter(xs, ys, zs, c=colors,s=80) #creating plot
			ax.view_init(elev=15., azim=9.) #setting a fixed view
			savefig(str(img_num)+'.png') 
			img_num+=1 
#build a gif animation from images
os.system('convert -delay 20 -loop 0 *.png images/anim.gif')
#delete temporal images created
os.system('rm *.png')

#******************************************************************************************************************
#FILE NAME : Volumetricgraphic.py
#Written by: Kaleb Alfaro-Badilla.
#This script creates a 3D graphic that represents the axonal voltage output from the network.
#CREATED : 2017
#LAST ACTUALIZATION : 2017
#Python 2.7
#******************************************************************************************************************
