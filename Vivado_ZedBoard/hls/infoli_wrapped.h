#ifndef INFOLI_WRAPPED
#define INFOLI_WRAPPED

#include "infoli.h"


#ifndef Native_IW 

#include "dma_interface.h"
#include <ap_axi_sdata.h>

typedef ap_axiu<32,4,5,5> AXI_VAL;

// function prototypes

void HLS_accel (cellState local_state0[MAX_TIME_MUX],  AXI_VAL INPUT_STREAM[MAX_TIME_MUX+NUM_NEIGH_CELLS] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], AXI_VAL OUTPUT_STREAM[MAX_TIME_MUX+NUM_NEIGH_CELLS], mod_prec Test[TESTERNUMBER]);

#else
void standalone_ComputeNetwork(cellState*, mod_prec*,  mod_prec* , int , int ,mod_prec*, mod_prec*, mod_prec*, mod_prec*);

#endif

#endif



	
