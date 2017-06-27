//#include "infoli.h"
#include "infoli_wrapped.h"

#ifndef Native_IW

#include <assert.h>
#include <ap_axi_sdata.h>

typedef ap_axiu<32,4,5,5> AXI_VAL;

void HLS_accel (cellState local_state0[MAX_TIME_MUX],  AXI_VAL INPUT_STREAM[MAX_TIME_MUX +NUM_NEIGH_CELLS] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], AXI_VAL OUTPUT_STREAM[MAX_TIME_MUX+NUM_NEIGH_CELLS], mod_prec Test[TESTERNUMBER])
{
	
	#pragma HLS INTERFACE s_axilite port=local_state0 bundle=bus
	#pragma HLS INTERFACE axis      port=INPUT_STREAM
	#pragma HLS INTERFACE s_axilite port=N_Size bundle=bus
	#pragma HLS INTERFACE s_axilite port=Mux_Factor bundle=bus
	#pragma HLS INTERFACE s_axilite port=Connectivity_Matrix bundle=bus
	#pragma HLS INTERFACE axis      port=OUTPUT_STREAM
	#pragma HLS INTERFACE s_axilite port=Test bundle=bus   // Erase Only for test
	#pragma HLS INTERFACE s_axilite port=return     bundle=CONTROL_BUS
	
	
	
	

	wrapped_ComputeNetwork (local_state0,  INPUT_STREAM, N_Size, Mux_Factor, Connectivity_Matrix, OUTPUT_STREAM, Test);

	return;
}

#else

void standalone_ComputeNetwork(cellState local_state0[MAX_TIME_MUX], mod_prec neighVdendE[NUM_NEIGH_CELLS],  mod_prec iAppin[MAX_TIME_MUX] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], mod_prec cellOut[MAX_TIME_MUX], mod_prec neighVdendOut[MAX_TIME_MUX], mod_prec Test[TESTERNUMBER]){
	
	ComputeNetwork(local_state0, neighVdendE[NUM_NEIGH_CELLS],  iAppin, N_Size, Mux_Factor, Connectivity_Matrix, cellOut, neighVdendOut, Test);

}

#endif






