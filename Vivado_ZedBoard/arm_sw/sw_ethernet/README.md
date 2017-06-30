# ZedBrain platform
# A research application from the Erasmus Brain project at Erasmus Medical Center (Erasmus MC) Neuroscience Department 
# Site: http://www.erasmusbrainproject.com/

# Collaborating institutions: Tecnologico de Costa Rica (TEC)

**Simulate brain cells using complex biologically accurate models, with the power of multiple ZedBoards**

ZedBrain is an open hardware demo that shows the capabilities of using the Zedboard Zynq platform and Xilinx Vivado HLS tools for hardware accelerated simulations of biologically accurate artificial neuron networks. 

Computing intensive models such as the Hodgkin-Huxley's can be implemented on C or C++ and synthesized on the PL fabric of the Zynq platform for high speed performance. Multiple Zedboards connected through the TCP-IP protocol can extend simulations to networks with thousands of cells, with quick, accurate results.


## How to compile the Ethernet driver into de ZedBoard
1. From Vivado project, export the hardware and bitstream to the project.sdk directory
2. Create a Xilinx SDK project using the Xilinx example "lwip echo server"
3. Replace the following files:
	- echo.c
	- main.c
4. Add the following files:
	- dma_main.c (optional, used only for testing purposes)
	- lib_xaccel_hw.c
	- lib_xaccel_hw.h
5. Modify BSP by changing these parameteres on the lwIP
	- "tcp_send_buff" to 32MB
	- DHCP to false
6. Build project

## Directory structure
In order to load the SDK software

1. Open the SDK from Vivado
2. Create a new project using the Hello World template
3. Delete the file main.c
4. Import the files "lib_xmmult_hw.c",  "lib_xmmult_hw.h" and "main.c" located in this folder to the SDK project
5.  In "lib_xmmult_hw.h" modify all the parameters of the network that you want to adjust.
6. Click-on the SDK Terminal
7. Add the UART port
8. Modify the size of the stack and heap as required by the maximum number of neurons you want to simulate.
9. Run the application


## Collaborators

PhD Students: Georgios Smaragdos, Carlos Salazar-Garcia

Undergraduate Assistants: Kaleb Alfaro-Badilla, Daniel Zamora-Umaña, Marco Acuña-Vargas

Advisers: Christos Strydis (Erasmus MC), Alfonso Chacon-Rodriguez (TEC)

## License
Copyright 2017 Erasmus Brain project

BSD-3-Clause



