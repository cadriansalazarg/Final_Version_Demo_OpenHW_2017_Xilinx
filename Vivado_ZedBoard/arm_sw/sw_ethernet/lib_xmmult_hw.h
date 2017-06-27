/*

Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.

Copyright <YEAR> <COPYRIGHT HOLDER>

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef H_LIB_EXAMPLE_HW_H
#define H_LIB_EXAMPLE_HW_H

#include "xhls_accel.h"
#include "xscugic.h"
#include "xaxidma.h"
#include "xtmrctr.h"


typedef float mod_prec;

#define NOPRINT 1
//#define NUM_FPGA_DEVICES 1

//Simulation parameters
#define N_SIZE_TEST 100	//Simulation total
#define NUM_NEIGH_CELLS 25 //Neighbors cells in FPGA
#define STOP_FLAG 0xFFFFFFFF//Stop simulation flag
#define CELL_STRUCT_SIZE 19
#define PRECISION_DATA_SIZE 4
#define INIT_DATA_LENGHT CELL_STRUCT_SIZE*NUM_NEIGH_CELLS*PRECISION_DATA_SIZE
#define STEP_DATA_LENGHT (N_SIZE_TEST+1)*PRECISION_DATA_SIZE //Including Flag
#define RESPONSE_DATA_LENGHT 2*NUM_NEIGH_CELLS*4
#define CONTINUE_FLAG 0xFFFF0000
#define STOP_FLAG 0xFFFFFFFF

#define SIM_STEP_TEST 1000


#define DMA_SIZE (N_SIZE_TEST)* sizeof(float)

int Setup_HW_Accelerator(float A[N_SIZE_TEST],  float res_hw[NUM_NEIGH_CELLS+NUM_NEIGH_CELLS], int dma_size);

int Run_HW_Accelerator(float A[N_SIZE_TEST], float res_hw[NUM_NEIGH_CELLS+NUM_NEIGH_CELLS], int dma_size);

int Start_HW_Accelerator(void);


int hls_init(XHls_accel *hls_Ptr);







#endif
