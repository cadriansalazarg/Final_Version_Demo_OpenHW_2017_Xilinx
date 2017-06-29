/*
Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.

Copyright 2017 Erasmus Brain Project

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/

/*
#******************************************************************************
#* Vivado_ZedBoard/hls/lib_xmmult_hw.h
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#* Instruction: The maximum value for IO_N_SIZE is 8000, The maximum value for TIME_MUX_FACTOR is 2000, TIME_MUX_FACTOR < IO_N_SIZE
#* For 4000<=IO_N_SIZE<= 8000 open file lscript.ld and you must put Stack and Heap Size in 0x7000, when TIME_MUX_FACTOR = 2000, other values requieres differents values of Heap and Stack Size.
#******************************************************************************/

#ifndef H_LIB_EXAMPLE_HW_H
#define H_LIB_EXAMPLE_HW_H

#include "xhls_accel.h"

#define TIME_MUX_FACTOR 2000 // it represents the number of  local cells executing in a ZedBoard
#define IO_N_SIZE 8000 // it represents the total number of cell running in N ZedBoards, it must be greater than TIME_MUX_FACTOR
#define NUM_NEIG_CELLS IO_N_SIZE - TIME_MUX_FACTOR

#define PRINTFAST 1 // This variable must be uncommented  if you want to print faster, highly recommendly

#define SIM_STEP_TEST 10


int Setup_HW_Accelerator(float*,  float*, int);

int Run_HW_Accelerator(float*, float*, int);

int Start_HW_Accelerator(void);


int hls_init(XHls_accel *hls_Ptr);

#endif

/*
#******************************************************************************
#* Vivado_ZedBoard/hls/lib_xmmult_hw.h
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/
