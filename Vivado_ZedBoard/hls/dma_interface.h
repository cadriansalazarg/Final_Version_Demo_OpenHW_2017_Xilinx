
#ifndef DMA_INTERFACE
#define DMA_INTERFACE


#include "infoli.h"
#include <ap_axi_sdata.h>
#include "infoli_wrapped.h"

typedef ap_axiu<32,4,5,5> AXI_VAL;

void wrapped_ComputeNetwork (cellState local_state0[MAX_TIME_MUX],  AXI_VAL in_stream[MAX_TIME_MUX+NUM_NEIGH_CELLS] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], AXI_VAL out_stream[MAX_TIME_MUX+NUM_NEIGH_CELLS], mod_prec Test[TESTERNUMBER]);


cellState InitState1();

int test_matrix_mult(void);


#endif
