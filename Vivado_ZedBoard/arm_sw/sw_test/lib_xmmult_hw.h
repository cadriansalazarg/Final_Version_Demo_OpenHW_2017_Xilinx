#ifndef H_LIB_EXAMPLE_HW_H
#define H_LIB_EXAMPLE_HW_H

#include "xhls_accel.h"

#define TIME_MUX_FACTOR 50
#define IO_N_SIZE 100
#define NUM_NEIG_CELLS IO_N_SIZE - TIME_MUX_FACTOR

#define PRINTFAST 1 // This variable must be uncommented  if you want to print faster, highly recommendly

#define SIM_STEP_TEST 5


int Setup_HW_Accelerator(float*,  float*, int);

int Run_HW_Accelerator(float*, float*, int);

int Start_HW_Accelerator(void);


int hls_init(XHls_accel *hls_Ptr);

#endif
