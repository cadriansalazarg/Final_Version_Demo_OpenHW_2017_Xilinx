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
#* Vivado_ZedBoard/hls/dma_interface.h
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/


#ifndef DMA_INTERFACE
#define DMA_INTERFACE


#include "infoli.h"
#include <ap_axi_sdata.h>
#include "infoli_wrapped.h"

typedef ap_axiu<32,4,5,5> AXI_VAL;

void wrapped_ComputeNetwork (cellState local_state0[MAX_TIME_MUX],  AXI_VAL in_stream[MAX_TIME_MUX+MAX_NEIGH_SIZE] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], AXI_VAL out_stream[MAX_TIME_MUX+MAX_NEIGH_SIZE], mod_prec Test[TESTERNUMBER]);


cellState InitState1();

int test_matrix_mult(void);


#endif


/*
#******************************************************************************
#* Vivado_ZedBoard/hls/dma_interface.h
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/
