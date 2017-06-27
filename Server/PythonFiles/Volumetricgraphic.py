
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

os.system('rm *.png')

nsize = int(sys.argv[1])

fig = plt.figure(figsize=(8,6))
ax = fig.add_subplot(111,projection='3d')



x=[]
y=[]
z=[]
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
	
xs = np.array(x)
ys = np.array(y)
zs = np.array(z)
colmap = cm.ScalarMappable(norm=colors.Normalize(vmin=-80.,vmax=-50.),cmap=cm.gnuplot)
colmap.set_array(np.array([-80,-70]))
cb = fig.colorbar(colmap)
cb.set_label('Axon output(V)')
img_num=0
with open("dataG.txt") as fp:
	for i, line in enumerate(fp):
		
		if(i%500==0):			
			plt.cla()
			ax.set_title('Volumetric plot of axon responses\nfor neuron population')
			ax.text(10, 0, 0, "Step: "+str(i)+' out of '+'50000')
			ax.axes.get_xaxis().set_ticklabels([])
			ax.axes.get_yaxis().set_ticklabels([])
			ax.set_zticks([])
			four_dim = np.array([float(x) for x in line.split()])
			norm = plt.Normalize(vmin=-80.,vmax=-50.)
			colors = cm.gnuplot(norm(four_dim))
			yg = ax.scatter(xs, ys, zs, c=colors,s=80)
			ax.view_init(elev=15., azim=9.)
			savefig(str(img_num)+'.png')
			img_num+=1
os.system('convert -delay 20 -loop 0 *.png images/anim.gif')
os.system('rm *.png')
