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
#* Vivado_ZedBoard/hls/main.cpp
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#* This is a test to simulate in a ZedBoard. It simulates the algorithm without using Ethernet and nor other ZedBoards. The idea of that it demonstrates
#* the correct functionally of a ZedBoard. In this case, the ZedBoard believes that it is working together with other ZedBoards.
#* This software is used to test all the functionality of the system.
#******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xparameters.h"
#include "xtmrctr.h"
#include "xaxidma.h"
#include "lib_xaccel_hw.h"




#define XPAR_AXI_TIMER_DEVICE_ID 		(XPAR_AXI_TIMER_1_DEVICE_ID) 

// HLS network HW instance
XHls_accel computenetwork;

// TIMER Instance
XTmrCtr timer_dev;

// AXI DMA Instance
XAxiDma AxiDma;


int init_dma(){
	XAxiDma_Config *CfgPtr;
	int status;

	CfgPtr = XAxiDma_LookupConfig( (XPAR_AXI_DMA_1_DEVICE_ID) );
	
	if(!CfgPtr){
		print("Error looking for AXI DMA config\n\r");
		return XST_FAILURE;
	}
	
	status = XAxiDma_CfgInitialize(&AxiDma,CfgPtr);
	
	if(status != XST_SUCCESS){
		print("Error initializing DMA\n\r");
		return XST_FAILURE;
	}
	
	//check for scatter gather mode
	if(XAxiDma_HasSg(&AxiDma)){
		print("Error DMA configured in SG mode\n\r");
		return XST_FAILURE;
	}
	
	/* Disable interrupts, we use polling mode */
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DEVICE_TO_DMA);
	XAxiDma_IntrDisable(&AxiDma, XAXIDMA_IRQ_ALL_MASK, XAXIDMA_DMA_TO_DEVICE);

	// Reset DMA
	XAxiDma_Reset(&AxiDma);
	while (!XAxiDma_ResetIsDone(&AxiDma)) {}

	return XST_SUCCESS;
}


int main(int argc, char **argv)
{
	int i, j;
	int status, status1;


	float data, iApp;
	float iAppin[IO_N_SIZE];
	float cellOut[TIME_MUX_FACTOR+TIME_MUX_FACTOR];

	unsigned int dma_size;

	if (IO_N_SIZE>(TIME_MUX_FACTOR+TIME_MUX_FACTOR))
		dma_size= (IO_N_SIZE)* sizeof(float); //1024*4 = 4096 bbytes
	else
		dma_size= (TIME_MUX_FACTOR+TIME_MUX_FACTOR)* sizeof(float); //1024*4 = 4096 bbytes


	unsigned int init_time, curr_time, calibration;
	unsigned int begin_time;
	unsigned int end_time;
	//unsigned int run_time_sw = 0;
	unsigned int run_time_hw = 0;

	init_platform();

	status1 = hls_init(&computenetwork);

	if(status1 != XST_SUCCESS){
		print("HLS peripheral setup failed\n\r");
	 	exit(-1);
	}

	xil_printf("***************************************************************\r");
	xil_printf("  COMPUTE NETWORK -> AXI DMA -> ARM ACP                   \r");
	xil_printf("  DESIGNED WITH  Vivado + HLS + IP Integrator 2016.4 \r");
	xil_printf("***************************************************************\r");

	/* ******************************************************************************* */
	// Init DMA
	status = init_dma();
	if(status != XST_SUCCESS){
		print("\rError: DMA init failed\n");
		return XST_FAILURE;
	}
	print("\nDMA Init done\n");

	/* ******************************************************************************* */
	// Setup HW timer
	status = XTmrCtr_Initialize(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
	if(status != XST_SUCCESS)
	{
		print("Error: timer setup failed\n");
	}
	XTmrCtr_SetOptions(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID, XTC_ENABLE_ALL_OPTION);

	// Calibrate HW timer
	XTmrCtr_Reset(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
	init_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
	curr_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
	calibration = curr_time - init_time;

	// Loop measurement
	XTmrCtr_Reset(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
	begin_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
	end_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
	run_time_hw = end_time - begin_time - calibration;


	/* ******************************************************************************* */

	
	/** Start of Initiation */
	
	for(i=0;i<TIME_MUX_FACTOR; i++){

    	data = -60;
    	XHls_accel_Write_local_state0_dend_V_dend_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.0337836;
		XHls_accel_Write_local_state0_dend_Hcurrent_q_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.0112788;
		XHls_accel_Write_local_state0_dend_Calcium_r_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.0049291;
		XHls_accel_Write_local_state0_dend_Potassium_s_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.5;
		XHls_accel_Write_local_state0_dend_I_CaH_Words(&computenetwork, i,(int *) &data, 1);
		data = 3.7152;
		XHls_accel_Write_local_state0_dend_Ca2Plus_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.68;
		XHls_accel_Write_local_state0_soma_g_CaL_Words(&computenetwork, i,(int *) &data, 1);
		data = -60;
		XHls_accel_Write_local_state0_soma_V_soma_Words(&computenetwork, i,(int *) &data, 1);
		data = 1.0127807;
		XHls_accel_Write_local_state0_soma_Sodium_m_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.3596066;
		XHls_accel_Write_local_state0_soma_Sodium_h_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.7423159;
		XHls_accel_Write_local_state0_soma_Calcium_k_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.0321349;
		XHls_accel_Write_local_state0_soma_Calcium_l_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.2369847;
		XHls_accel_Write_local_state0_soma_Potassium_n_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.2369847;
		XHls_accel_Write_local_state0_soma_Potassium_p_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.1;
		XHls_accel_Write_local_state0_soma_Potassium_x_s_Words(&computenetwork, i,(int *) &data, 1);
		data = -60;
		XHls_accel_Write_local_state0_axon_V_axon_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.003596066;
		XHls_accel_Write_local_state0_axon_Sodium_m_a_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.9;
		XHls_accel_Write_local_state0_axon_Sodium_h_a_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.2369847;
		XHls_accel_Write_local_state0_axon_Potassium_x_a_Words(&computenetwork, i,(int *) &data, 1);
		data = 0.04;
		XHls_accel_Write_Connectivity_Matrix_Words(&computenetwork, i,(int *) &data, 1);
    }
    
    for(i=0;i<IO_N_SIZE; i++){
		data = 0.04;
		XHls_accel_Write_Connectivity_Matrix_Words(&computenetwork, i,(int *) &data, 1);
	}

	XHls_accel_Set_Mux_Factor(&computenetwork,TIME_MUX_FACTOR);
	XHls_accel_Set_N_Size(&computenetwork, IO_N_SIZE);
	
	/** End of Initiation */
	
	
	
	for(i=0;i<SIM_STEP_TEST;i++){
		

		if(i>20000-1 && i<20500-1){ iApp = 6;} // start @ 1 because skipping initial values
		else{ iApp = 0;}

		for(j=0;j<TIME_MUX_FACTOR;j++){
			iAppin[j] = iApp;
		}

 	    
 	    for (j=TIME_MUX_FACTOR; j<IO_N_SIZE; j++){
 		    if (i==0)	
				iAppin[j] = -60.0f;
			else
				if (IO_N_SIZE>(TIME_MUX_FACTOR+TIME_MUX_FACTOR))
					iAppin[j] = cellOut[TIME_MUX_FACTOR];
				else
					iAppin[j] = cellOut[j];
 	    }

	

		/* ******************************************************************************* */
		// call the HW accelerator
		XTmrCtr_Reset(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
		begin_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
		// Setup the HW Accelerator
		status = Setup_HW_Accelerator(iAppin, cellOut, dma_size);
	

		status = Start_HW_Accelerator();
		status = Run_HW_Accelerator(iAppin, cellOut, dma_size);

	
		end_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
		run_time_hw = end_time - begin_time - calibration;
		xil_printf("Step %d:\n", i+1);
		xil_printf("Total run time for AXI DMA + HW accelerator is %d cycles\n", run_time_hw);

		/* ******************************************************************************* */
		





		#ifdef PRINTFAST
			printf("Va  %d: %f,\t Va  %d: %f,\t Va  %d: %f,\t Va  %d: %f,\t", 0, cellOut[3], 1, cellOut[4], TIME_MUX_FACTOR-2, cellOut[TIME_MUX_FACTOR-2],TIME_MUX_FACTOR-1, cellOut[TIME_MUX_FACTOR-1]);
			printf("Vd  %d: %f,\t Vd  %d: %f,\t Vd  %d: %f,\t Vd  %d: %f\n", 0, cellOut[TIME_MUX_FACTOR+2], 1, cellOut[TIME_MUX_FACTOR+2+1], TIME_MUX_FACTOR-2, cellOut[TIME_MUX_FACTOR+TIME_MUX_FACTOR-2],TIME_MUX_FACTOR-1, cellOut[TIME_MUX_FACTOR+TIME_MUX_FACTOR-1]);
		#else

		    for(j=0; j<TIME_MUX_FACTOR+TIME_MUX_FACTOR-1; j++){
		    	if(j<TIME_MUX_FACTOR)
		    		printf("Va %d: %f\t", j+1, cellOut[j]);
		    	else
		    		printf("Vd %d: %f\t", j+1-TIME_MUX_FACTOR, cellOut[j]);
		    }

		    printf("Vd %d: %f\n", TIME_MUX_FACTOR, cellOut[TIME_MUX_FACTOR+TIME_MUX_FACTOR-1]);

		#endif
    	
	}


    cleanup_platform();
    return 0;
}

/*
#******************************************************************************
#* Vivado_ZedBoard/hls/main.cpp
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/
