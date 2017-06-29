# ZedBrain platform
# A research application from the Erasmus Brain project at Erasmus Medical Center (Erasmus MC) Neuroscience Department 
# Site: http://www.erasmusbrainproject.com/

# Collaborating institutions: Tecnologico de Costa Rica (TEC)

**Simulate brain cells using complex biologically accurate models, with the power of multiple ZedBoards**

ZedBrain is an open hardware demo that shows the capabilities of using the Zedboard Zynq platform and Xilinx Vivado HLS tools for hardware accelerated simulations of biologically accurate artificial neuron networks. 

Computing intensive models such as the Hodgkin-Huxley's can be implemented on C or C++ and synthesized on the PL fabric of the Zynq platform for high speed performance. Multiple Zedboards connected through the TCP-IP protocol can extend simulations to networks with thousands of cells, with quick, accurate results.

## Instructions (Last revision: 6-28-2017)

All of these files are used to test the correct functionallity of the design. Ethernet is not included hear. It is the best case to test the system.

In order to loaded the SDK software

1. Open the SDK from Vivado
2. Create a new project using  Hello World template
3. Delete the file main.c
4. Import the files lib_xmmult_hw.c,  lib_xmmult_hw.h,  main.c located in this folder to the SDK project
5. Modify in the  lib_xmmult_hw.h all the parameters of the network tht you want change.
6. Click-on the SDK Terminal
7. Add the UART port
8. Modify the size of the stack and heap in function of the number of neurons.
9. Run the apllication


## Collaborators

PhD Students: Georgios Smaragdos, Carlos Salazar-Garcia

Undergraduate Assistants: Kaleb Alfaro-Badilla, Daniel Zamora-Umaña, Marco Acuña-Vargas

Advisers: Christos Strydis (Erasmus MC), Alfonso Chacon-Rodriguez (TEC)

## License
Copyright 2017 Erasmus Brain project

BSD-3-Clause
