# ZedBrain platform
# A research application from the Erasmus Brain project at Erasmus Medical Center (Erasmus MC) Neuroscience Department 
# Site: http://www.erasmusbrainproject.com/

# Collaborating institutions: Tecnologico de Costa Rica (TEC). Vineyard EU Project.

**Simulate brain cells using complex biologically accurate models, with the power of multiple ZedBoards**

ZedBrain is an open hardware demo that shows the capabilities of using the Zedboard Zynq platform and Xilinx Vivado HLS tools for hardware accelerated simulations of biologically accurate artificial neuron networks. 

Computing intensive models such as the Hodgkin-Huxley's can be implemented on C or C++ and synthesized on the PL fabric of the Zynq platform for high speed performance. Multiple Zedboards connected through the TCP-IP protocol can extend simulations to networks with thousands of cells, with quick, accurate results.

## Directory structure
Vivado_ZedBoard/ ------------ Project files required to program the ZedBoards. Vivado ver. 2016.4

Server/          ------------ Webserver interface to the ZedBrain platform

hw/              ------------ Bitstream files

ip/              ------------ Hardware accelarator ION IP. Generated with Vivado HLS 2016.4

doc/             ------------ Documentation.

User's_guide/    ------------ A  README.md file is provided as a brief user guide. Here you can find two links to YouTube videos with a step-by-step guide on how to get the ZedBrain platform going.

## Collaborators

PhD Students: Georgios Smaragdos, Carlos Salazar-Garcia

Undergraduate Assistants: Kaleb Alfaro-Badilla, Daniel Zamora-Umaña, Marco Acuña-Vargas

Advisers: Christos Strydis (Erasmus MC), Alfonso Chacon-Rodriguez (TEC)

## License
Copyright 2017 Erasmus Brain project

BSD-3-Clause
