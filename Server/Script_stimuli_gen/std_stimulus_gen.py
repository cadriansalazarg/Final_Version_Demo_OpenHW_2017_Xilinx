#Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.
#Copyright 2017 Erasmus Brain Project
#Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
#1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
#2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
#3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
#THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#******************************************************************************************************************
#FILE NAME = script_std.py
#Written by: Kaleb Alfaro-Badilla & Daniel Zamora-Umaña.
#This script was created to performs the communication between a Web Server and peripheral devices(Zedboards).
#CREATED = 2016
#LAST ACTUALIZATION = 27/06/2017
#Python 2.7
#******************************************************************************************************************

n=100	#numero celdas
m=0
acum=0
rep=50000	#replicas  iapp
init=19998	#inicio escalon restar 2 en cada limite
fin=20498	#final escalon restar 2 en cada limite


archivo = open("data1.txt","r") 
for line in archivo.readlines():
	for i in range(0,len(line)):
		if line[i]=='\t':
			acum=line[i+1:]
			acum=acum.rstrip()
			varstr=line[:i]
			if m==0:
				f = open("data.txt","w")
				m=1
			else:
				f = open("data.txt","a+") 
			f.write(varstr)
			print(varstr)
			f.write('\t') 
			for b in range(0,n):
				f.write(acum)
				f.write('\t') 
			f.write('\n')
f.close()
archivo.close() 

f = open("data.txt","a+")
line=f.readlines()
for i in range(0,rep):
	acum = str(i)+':'
	if (i>=init and i<=fin):
		for b in range(0,n):
			if(b>n/2):
				acum+= '\t'+'6'
			else:
				acum+= '\t'+'0'
		acum+='\n'
		f.write(acum)
	elif(i>=init+5000 and i<=fin+5000):
		for b in range(0,n):
			if(b>n/2):
				acum+= '\t'+'0'
			else:
				acum+= '\t'+'6'
		acum+='\n'
		f.write(acum)
	else:
		for b in range(0,n):
				acum+= '\t'+'0'
		acum+='\n'
		f.write(acum) 
f.close()
#******************************************************************************************************************
#FILE NAME = script_std.py
#Written by: Kaleb Alfaro-Badilla & Daniel Zamora-Umaña.
#This script was created to performs the communication between a Web Server and peripheral devices(Zedboards).
#CREATED = 2016
#LAST ACTUALIZATION = 2017
#Python 2.7
#Copyright 2017 Erasmus Brain Project
#******************************************************************************************************************

