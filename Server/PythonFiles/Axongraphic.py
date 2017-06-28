#!/usr/bin/python

#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.
#Copyright 2017 Erasmus Brain Project
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#******************************************************************************************************************
#FILE NAME = Axongraphic.py
#Written by: Daniel Zamora_Umaña.
#This script creates a 2D graphic that represents the axonal voltage output from a specific neuron in the network.
#CREATED = 2016
#LAST ACTUALIZATION = 27/06/2017
#Python 2.7
#******************************************************************************************************************

#Libraries
#------------------------------o----------------------------#
import matplotlib
matplotlib.use('Agg') #solves compatibility with Apache
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

# Create the line collection object.
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

#******************************************************************************************************************
#FILE NAME = Axongraphic.py
#Written by: Daniel Zamora_Umaña.
#This script creates a 2D graphic that represents the axonal voltage output from a specific neuron in the network.
#CREATED = 2016
#LAST ACTUALIZATION = 27/06/2017
#Python 2.7
#******************************************************************************************************************

