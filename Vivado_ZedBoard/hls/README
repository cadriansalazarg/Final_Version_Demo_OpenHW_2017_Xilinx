# ZedBrain platform
# A research application from the Erasmus Brain project at Erasmus Medical Center (Erasmus MC) Neuroscience Department 
# Site: http://www.erasmusbrainproject.com/

# Collaborating institutions: Tecnologico de Costa Rica (TEC)

**Simulate brain cells using complex biologically accurate models, with the power of multiple ZedBoards**

ZedBrain is an open hardware demo that shows the capabilities of using the Zedboard Zynq platform and Xilinx Vivado HLS tools for hardware accelerated simulations of biologically accurate artificial neuron networks. 

Computing intensive models such as the Hodgkin-Huxley's can be implemented on C or C++ and synthesized on the PL fabric of the Zynq platform for high speed performance. Multiple Zedboards connected through the TCP-IP protocol can extend simulations to networks with thousands of cells, with quick, accurate results.

## Instructions (Last revision: 6-28-2017)

In order to compile the C++ reference design with its respective wrappers and testbench, execute these commands from the shell: 

g++ -c  infoli.cpp -lm
g++ -c  infoli_wrapped.cpp -DNative_IW
g++ -c  infoli_tb.cpp -DNative_IT

g++ -o FinalProgram infoli.o infoli_wrapped.o infoli_tb.o


A Makefile has been provided as well.

If you want to generate the IP, you must execute the following comand from the shell:

vivado_hls -f run_hls.tcl


## Collaborators

PhD Students: Georgios Smaragdos, Carlos Salazar-Garcia

Undergraduate Assistants: Kaleb Alfaro-Badilla, Daniel Zamora-Umaña, Marco Acuña-Vargas

Advisers: Christos Strydis (Erasmus MC), Alfonso Chacon-Rodriguez (TEC)











