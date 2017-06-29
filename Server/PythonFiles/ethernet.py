
#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.
#Copyright 2017 Erasmus Brain Project
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#******************************************************************************************************************
#FILE NAME = ethernet.py
#Written by: Kaleb Alfaro-Badilla & Daniel Zamora-Umana.
#This script was created to performs the communication between a Web Server and peripheral devices(Zedboards).
#CREATED = 2016
#LAST ACTUALIZATION = 27/06/2017
#Python 2.7
#******************************************************************************************************************

#Libraries
#------------------------------o----------------------------#
import socket
import sys                          
from threading import Thread
import struct
import sched, time
#------------------------------o----------------------------#

# Definitions of data structures for data received from client 
class data_transfer(Thread):
	def __init__(self,ip,port,tx,state,n_size): 
		Thread.__init__(self)
		self.Ip = ip
		self.port = port 
		self.tx = tx
		self.state = state 
		self.n_size = n_size
	def run(self): 
		sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)    #Create a socket
		sock.setsockopt(socket.SOL_SOCKET, socket.SO_SNDBUF, 1024)
		# Connect the socket to the port where the server is listening
		server_address = (self.Ip, 7)
		sock.connect(server_address)
		try:
			sock.sendall(self.tx)		     #Send the data.
			self.rx = ""
			if self.state==1:
				totalsent=0
				MSGLEN= 4*self.n_size*2      #Received message lenght= 2 vdend & 4 float.
				while totalsent < MSGLEN:    #Wait for the device response
					
            				self.rx = self.rx + sock.recv(1024)
            				totalsent= len(self.rx)
              		else:
				self.rx = sock.recv(150)			
			
						
		finally:
			sock.close()



class FPGA_device: #device's structure, here every device receive an ip, id and local n_size.
	ID_count = 0
	def __init__(self, ip, port, n_size):
		self.Ip = ip
		self.port = port
		self.fpga_n_size = n_size
		self.ID = self.ID_count
		FPGA_device.ID_count += 1 

class simCell: #Structure for one cell #check
	dend_V_dend=0
	dend_Calcium_r=0
	dend_Potassium_s =0
	dend_Hcurrent_q	=0
	dend_Ca2Plus =0
	dend_I_CaH =0
	soma_g_CaL =0
	soma_V_soma =0
	soma_Sodium_m =0
	soma_Sodium_h =0
	soma_Potassium_n =0
	soma_Potassium_p =0
	soma_Potassium_x_s =0
	soma_Calcium_k	=0
	soma_Calcium_l	=0
	axon_V_axon =0
	axon_Sodium_m_a	=0
	axon_Sodium_h_a	=0
	axon_Potassium_x_a =0
	def __init__(self, simSteps): #stimulus Iapp
		self.iapp = [0]*simSteps
	
class simNetwork: #this class initialize the network from received file

	network = []
	vdend = []
	cellout = []
	stepCount = 0
	accum = 0
	MAX = 0
	MIN = 1000
	def __init__(self, N_size, simSteps): #N_size is the simCell quantity to append in the list #check
		self.steps = simSteps		
		for n in range(N_size):	#Create the network.
			self.size = N_size
			self.network.append(simCell(simSteps))
		for n in range(simSteps):
			self.cellout.append([0]*N_size)
	def set_network(self, source): #source is the file location (read only) #check
		archivo = open(source,"r")
		lines=archivo.readlines()
		for i in range(self.size):	#is loaded the initialization from file.
			self.network[i].dend_V_dend=        float(lines[0].split('\t')[i+1])
			self.vdend.append(self.network[i].dend_V_dend)
			self.network[i].dend_Calcium_r=     float(lines[1].split('\t')[i+1])
			self.network[i].dend_Potassium_s =  float(lines[2].split('\t')[i+1])
			self.network[i].dend_Hcurrent_q	=   float(lines[3].split('\t')[i+1])
			self.network[i].dend_Ca2Plus =      float(lines[4].split('\t')[i+1])
			self.network[i].dend_I_CaH =        float(lines[5].split('\t')[i+1])
			self.network[i].soma_g_CaL =        float(lines[6].split('\t')[i+1])
			self.network[i].soma_V_soma =       float(lines[7].split('\t')[i+1])
			self.network[i].soma_Sodium_m =     float(lines[8].split('\t')[i+1])
			self.network[i].soma_Sodium_h =     float(lines[9].split('\t')[i+1])
			self.network[i].soma_Potassium_n =  float(lines[10].split('\t')[i+1])
			self.network[i].soma_Potassium_p =  float(lines[11].split('\t')[i+1])
			self.network[i].soma_Potassium_x_s =float(lines[12].split('\t')[i+1])
			self.network[i].soma_Calcium_k	=  float(lines[13].split('\t')[i+1])
			self.network[i].soma_Calcium_l	=  float(lines[14].split('\t')[i+1])
			self.network[i].axon_V_axon =       float(lines[15].split('\t')[i+1])
			self.network[i].axon_Sodium_m_a=    float(lines[16].split('\t')[i+1])
			self.network[i].axon_Sodium_h_a=    float(lines[17].split('\t')[i+1])
			self.network[i].axon_Potassium_x_a= float(lines[18].split('\t')[i+1])
		
		for i in range(self.steps):
			for j in range(self.size):
				self.network[j].iapp[i] = float(lines[19+i].split('\t')[j+1])

		archivo.close()		#close the file


######################################## INITIALIZATION SECTION #################################################
	def hw_init(self, fpga_devices): # setup the hardware with the initialization parameters
			
		count_simCell = 0
		tx_list = []
		state=0
		
		for device in fpga_devices: #generates messages for each device
			tx=""	
			#package that contains the N size for every device.
			packer = struct.Struct('i'*2)
			tx += packer.pack(self.size,device.fpga_n_size)
			#package of 19 float data of the init state for every device initialization.
			packer = struct.Struct('f'*19)
			cell_count=0
			for i in range(count_simCell, device.fpga_n_size):
				tx += packer.pack(self.network[i].dend_V_dend, self.network[i].dend_Calcium_r,self.network[i].dend_Potassium_s,self.network[i].dend_Hcurrent_q,self.network[i].dend_Ca2Plus, self.network[i].dend_I_CaH, self.network[i].soma_g_CaL, self.network[i].soma_V_soma, self.network[i].soma_Sodium_m, self.network[i].soma_Sodium_h, self.network[i].soma_Potassium_n, self.network[i].soma_Potassium_p, self.network[i].soma_Potassium_x_s, self.network[i].soma_Calcium_k, self.network[i].soma_Calcium_l, self.network[i].axon_V_axon, self.network[i].axon_Sodium_m_a, self.network[i].axon_Sodium_h_a, self.network[i].axon_Potassium_x_a) 

			
			tx_list.append(tx)
		count_simCell += device.fpga_n_size
		threads = []
		

		for device in fpga_devices: #build threads to transfer data.
			new_thread = data_transfer(device.Ip,device.port,tx_list[device.ID],state,device.fpga_n_size)
			new_thread.start()
			threads.append(new_thread)
			

		for t in threads: #waiting for threads	
			t.join()
		
		
######################################## STEP GENERATION SECTION #################################################
	def gen_step(self, fpga_devices,finish) :	
		vdend = []		
		tx_list = []
		count_simCell = 0
		state=1
	
		for device in fpga_devices: #generates messages for each device
			tx=""
			#package that contains the iapp for every cell in the device.
			packer = struct.Struct('f')		
			for i in range(count_simCell,count_simCell+device.fpga_n_size):
				tx += packer.pack(self.network[i].iapp[self.stepCount])
			#package that contains the vdend from cells in another devices.
			if(self.size > device.fpga_n_size):
				packer = struct.Struct('f')
				vdend = self.vdend[:count_simCell]+self.vdend[count_simCell+device.fpga_n_size:]
				
				for v in vdend :
					tx += packer.pack(v)
			

			if finish==True:
				tx += "\xFF\xFF\xFF\xFF" #stop condition= the last iapp have this condition
			else:				
				tx += "\x00\x00\xFF\xFF" #continue condition= the last iapp doesn't have this condition
			
			tx_list.append(tx)
			count_simCell += device.fpga_n_size
		

		#lines from time() library are for time execution measurement purposes. In this case, only data transfer time is measured.
		start = time.time()	#get initial time value. 

		threads = []
		for device in fpga_devices: #build threads
			new_thread=data_transfer(device.Ip,device.port,tx_list[device.ID],state,device.fpga_n_size)
			new_thread.start()
			threads.append(new_thread)
		
	
		for t in threads: #waiting for threads		
			t.join()
		
		end= time.time()	#get final time value. 
		ref=end-start		#get the time execution in seconds.
		
		##Only for measurement purposes(average, maximun and minimun)
		self.accum+=ref	
		if self.MAX < ref:
			self.MAX=ref
		if self.MIN>ref:
			self.MIN=ref

		self.vdend = []
		cellout_fpga = []
		vdend_fpga = []
		vdend_cont=0
		#save the cellout(Vaxon) and Vdend.
		for device in fpga_devices:
			packer = struct.Struct('f'*device.fpga_n_size)
			cellout_fpga = packer.unpack(threads[device.ID].rx[0:4*device.fpga_n_size])
			vdend_fpga = packer.unpack(threads[device.ID].rx[4*device.fpga_n_size:])
			for a in range(len(cellout_fpga)):
				self.cellout[simNetwork.stepCount][a+(device.ID*device.fpga_n_size)] =cellout_fpga[a]
				self.vdend.append(vdend_fpga[a])
			
			
		simNetwork.stepCount += 1
		print "gen_step", simNetwork.stepCount	#for test purposes
		 		


	def print_file (self, file_name): #write file from data
		print "print_file"
		f = open(file_name,"w")
		buff = ""		
		for s in range(simNetwork.stepCount):		
			for n in range(self.size):
				buff += str(self.cellout[s][n]) + '\t'
			buff += '\n'
		f.write(buff)
		f.close()

		#Only for measurement purposes.
		PROM=self.accum/simNetwork.stepCount
		print "PROMEDIO" , PROM , "MAX", self.MAX , "MIN", self.MIN
		
#######################################################################################################
######################################## MAIN SECTION #################################################


N=int(sys.argv[1])            #It take arguments, the first one is for Nsize, the second one it's number of steps 
STEP=int(sys.argv[2])	      #and third one, number of devices connected.
FPGA=int(sys.argv[3])

			      #It associate devices connected with an IP, port and N size, the global N size is divided for each device.
if FPGA==4:
	fpga_devices = [FPGA_device('192.168.1.10', 7,N/4), FPGA_device('192.168.1.11', 7,N/4), FPGA_device('192.168.1.12', 7,N/4), FPGA_device('192.168.1.13', 7,N/4)]
elif FPGA==3:
	fpga_devices = [FPGA_device('192.168.1.10', 7,N/3), FPGA_device('192.168.1.11', 7,N/3), FPGA_device('192.168.1.12', 7,N/3)]
elif FPGA==2:
	fpga_devices = [FPGA_device('192.168.1.10', 7,N/2), FPGA_device('192.168.1.11', 7,N/2)]
else:
	fpga_devices = [FPGA_device('192.168.1.11', 7,N)]



NETWORK = simNetwork(N,STEP)		#It create the network.
NETWORK.set_network("file/data.txt")

NETWORK.hw_init(fpga_devices)		#It initialize the network.

for s in range(STEP):			#It generate the steps.
	if s==(STEP-1):
		NETWORK.gen_step(fpga_devices,True)
	else:	
		NETWORK.gen_step(fpga_devices,False)


NETWORK.print_file("dataG.txt")		#Save a file with the output.


#******************************************************************************************************************
#FILE NAME = ethernet.py
#Written by: KalebAlfaro-Badilla & Daniel Zamora-Umana.
#This script was created to performs the communication between a Web Server and peripheral devices(Zedboards).
#CREATED = 2016
#LAST ACTUALIZATION = 2017
#Python 2.7
#Copyright 2017 Erasmus Brain Project
#******************************************************************************************************************

