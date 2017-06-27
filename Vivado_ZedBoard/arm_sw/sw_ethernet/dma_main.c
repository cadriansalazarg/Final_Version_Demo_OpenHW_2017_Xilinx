#include <stdio.h>
#include <stdlib.h>
#include "platform.h"
#include "xparameters.h"
#include "xtmrctr.h"
#include "xaxidma.h"
#include "lib_xmmult_hw.h"
//#include "xhls_accel.h"




#define XPAR_AXI_TIMER_DEVICE_ID 		(XPAR_AXI_TIMER_1_DEVICE_ID) 

// HLS network HW instance
XHls_accel computenetwork;

// TIMER Instance
XTmrCtr timer_dev;

// AXI DMA Instance
XAxiDma AxiDma;


int init_dmakek(){
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


int dma_main(int argc, char **argv)
{
	int i, j;
	int status, status1;


	float data;
	float iAppin[N_SIZE_TEST+NUM_NEIGH_CELLS];
	float cellOut[N_SIZE_TEST+NUM_NEIGH_CELLS];

	//unsigned int dma_size = (N_SIZE_TEST +NUM_NEIG_CELLS)* sizeof(float); //1024*4 = 4096 bbytes

	unsigned int init_time, curr_time, calibration;
	unsigned int begin_time;
	unsigned int end_time;
	//unsigned int run_time_sw = 0;
	unsigned int run_time_hw = 0;

	//init_platform();

	status1 = hls_init(&computenetwork);

	if(status1 != XST_SUCCESS){
		print("HLS peripheral setup failed\n\r");
	 	exit(-1);
	}

	xil_printf("***************************************************************\r");
	xil_printf("  COMPUTE NETWORK -> AXI DMA -> ARM ACP                   \r");
	xil_printf("  DESIGNED WITH  Vivado + HLS + IP Integrator 2016.3 \r");
	xil_printf("***************************************************************\r");

	/* ******************************************************************************* */
	// Init DMA
	status = init_dmakek();
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
	
	for(i=0;i<N_SIZE_TEST; i++){

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
    }
    
    for(i=0;i<N_SIZE_TEST+NUM_NEIGH_CELLS; i++){
		data = 0.04;
		XHls_accel_Write_Connectivity_Matrix_Words(&computenetwork, i,(int *) &data, 1);
	}

	XHls_accel_Set_Mux_Factor(&computenetwork,N_SIZE_TEST);
	XHls_accel_Set_N_Size(&computenetwork, N_SIZE_TEST);
	//XHls_accel_Set_Conn_Matrix_Size(&computenetwork, N_SIZE_TEST);
	
	/** End of Initiation */
	
	
	
	for(i=0;i<SIM_STEP_TEST;i++){
		
		for (j=0; j<N_SIZE_TEST; j++){
 		    iAppin[j] = 0.0f;
 	    }
 	    
 	    for (j=N_SIZE_TEST; j<N_SIZE_TEST+NUM_NEIGH_CELLS; j++){
 		    if (i==0)	
				iAppin[j] = -60.0f;
			else
				iAppin[j] = cellOut[j];
 	    }

	

		/* ******************************************************************************* */
		// call the HW accelerator
		XTmrCtr_Reset(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
		begin_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
		// Setup the HW Accelerator
		status = Setup_HW_Accelerator(iAppin, cellOut, DMA_SIZE);
	

		status = Start_HW_Accelerator();
		status = Run_HW_Accelerator(iAppin, cellOut, DMA_SIZE);

	
		end_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_DEVICE_ID);
		run_time_hw = end_time - begin_time - calibration;
		xil_printf("Step %d:\n", i);
		xil_printf("Total run time for AXI DMA + HW accelerator is %d cycles\n", run_time_hw);

		/* ******************************************************************************* */
		

		for(j=0; j<N_SIZE_TEST-1+NUM_NEIGH_CELLS; j++){
    		printf("Va %d: %f\t", j+1, cellOut[j]);
    	}
    	
    	printf("Va %d: %f\n", N_SIZE_TEST+NUM_NEIGH_CELLS, cellOut[N_SIZE_TEST-1+NUM_NEIGH_CELLS]);
    	
	}


    //cleanup_platform();
    return 0;
}
