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

