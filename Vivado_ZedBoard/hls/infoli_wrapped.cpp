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
#* Vivado_ZedBoard/hls/infoli_wrapped.c
#*
#* Written by: Carlos Salazar-García, 2017.
#*
#* This code is a wrapped of the function infoli.c
#* 
#*
#******************************************************************************/

//#include "infoli.h"
#include "infoli_wrapped.h"

#ifndef Native_IW

#include <assert.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<32,4,5,5> AXI_VAL;

void HLS_accel (cellState local_state0[MAX_TIME_MUX],  AXI_VAL INPUT_STREAM[MAX_TIME_MUX +MAX_NEIGH_SIZE] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], AXI_VAL OUTPUT_STREAM[MAX_TIME_MUX+MAX_NEIGH_SIZE], mod_prec Test[TESTERNUMBER])
{
	
	#pragma HLS INTERFACE s_axilite port=local_state0 bundle=bus
	#pragma HLS INTERFACE axis      port=INPUT_STREAM
	#pragma HLS INTERFACE s_axilite port=N_Size bundle=bus
	#pragma HLS INTERFACE s_axilite port=Mux_Factor bundle=bus
	#pragma HLS INTERFACE s_axilite port=Connectivity_Matrix bundle=bus
	#pragma HLS INTERFACE axis      port=OUTPUT_STREAM
	#pragma HLS INTERFACE s_axilite port=Test bundle=bus  
	#pragma HLS INTERFACE s_axilite port=return     bundle=CONTROL_BUS
	
	
	
	

	wrapped_ComputeNetwork (local_state0,  INPUT_STREAM, N_Size, Mux_Factor, Connectivity_Matrix, OUTPUT_STREAM, Test);

	return;
}

#else

void standalone_ComputeNetwork(cellState local_state0[MAX_TIME_MUX], mod_prec neighVdendE[MAX_NEIGH_SIZE],  mod_prec iAppin[MAX_TIME_MUX] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], mod_prec cellOut[MAX_TIME_MUX], mod_prec neighVdendOut[MAX_TIME_MUX], mod_prec Test[TESTERNUMBER]){
	
	ComputeNetwork(local_state0, neighVdendE[MAX_NEIGH_SIZE],  iAppin, N_Size, Mux_Factor, Connectivity_Matrix, cellOut, neighVdendOut, Test);

}

#endif









/*
#******************************************************************************
#* Vivado_ZedBoard/hls/infoli_wrapped.c
#*
#* Written by: Carlos Salazar-García, 2017.
#*
#* This code is a wrapped of the function infoli.c
#* 
#*
#******************************************************************************/


