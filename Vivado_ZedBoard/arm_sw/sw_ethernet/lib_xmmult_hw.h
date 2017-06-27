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
