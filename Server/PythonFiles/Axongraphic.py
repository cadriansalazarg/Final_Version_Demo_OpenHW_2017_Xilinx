#!/usr/bin/python


#Libraries
#------------------------------o----------------------------#
import matplotlib
matplotlib.use('Agg')
import matplotlib.pyplot as plt
import numpy as np
import sys 
from matplotlib.collections import LineCollection
from matplotlib.colors import ListedColormap, BoundaryNorm
#------------------------------o----------------------------#

number = int(sys.argv[1])    #arguments

#data loaded from output file, this data corresponds to Axon voltage from specific neuron and the number of steps.
data=[]
steps=[]
acum=0
archivo = open("dataG1.txt","r+") 
lines=archivo.readlines()
for line in lines:
	line=line.strip()
	data.append(float(line))		#Axon voltage array
	acum+=1
	steps.append(acum)			#Steps array
color = np.array(data)				#the color array represents the color intensity according the axon voltage array.


#cmap assigns the color intensity according a range in norm.
cmap = ListedColormap(['b','g','m','r'])
norm = BoundaryNorm([-100, -58,-50,-35, 100], cmap.N)

points = np.array([steps, data]).T.reshape(-1, 1, 2)
segments = np.concatenate([points[:-1], points[1:]], axis=1)


lc = LineCollection(segments, cmap=cmap, norm=norm)
lc.set_array(color)
lc.set_linewidth(1.2)

#It's created the figure according the data and the color assignation.
plt.figure(figsize=(10.5,4.5))
plt.gca().add_collection(lc)
plt.axis([0,len(data),min(data)-5,max(data)+5])   #Fixed axis are assigned.

#it's added a title label and axis labels to the figure.
if number ==0:
	plt.title('No Response')
else:
	plt.title('Detailed axon response for neuron '+str(number))
plt.ylabel('Value(milliVolts)')
plt.xlabel('Simulation Steps')
plt.grid(True) 					#grid ON
plt.savefig('images/myfig.png')			#this command saves a png figure to show it in the website


