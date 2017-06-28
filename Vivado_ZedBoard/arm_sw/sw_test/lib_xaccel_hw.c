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
#* Vivado_ZedBoard/hls/lib_xmmult_hw.c
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/


#include "platform.h"
#include "xparameters.h"
#include "xscugic.h"
#include "xaxidma.h"
#include "xhls_accel.h"
#include "lib_xaccel_hw.h"
#include "xil_printf.h"



volatile static int RunExample = 0;
volatile static int ResultExample = 0;

XHls_accel xcompute_Network;

XHls_accel_Config xCompute_Network_config = {
	0,
	XPAR_HLS_ACCEL_1_S_AXI_CONTROL_BUS_BASEADDR
};

//Interrupt Controller Instance
XScuGic ScuGic;

// AXI DMA Instance
extern XAxiDma AxiDma;


int XAccelSetup(){
	return XHls_accel_CfgInitialize(&xcompute_Network,&xCompute_Network_config);
}

void XAccelStart(void *InstancePtr){
	XHls_accel *pExample = (XHls_accel *)InstancePtr;
	XHls_accel_InterruptEnable(pExample,1);
	XHls_accel_InterruptGlobalEnable(pExample);
	XHls_accel_Start(pExample);
}


void XAccelIsr(void *InstancePtr){
	XHls_accel *pExample = (XHls_accel *)InstancePtr;

	//Disable the global interrupt
	XHls_accel_InterruptGlobalDisable(pExample);
	//Disable the local interrupt
	XHls_accel_InterruptDisable(pExample,0xffffffff);

	// clear the local interrupt
	XHls_accel_InterruptClear(pExample,1);

	ResultExample = 1;
	// restart the core if it should run again
	if(RunExample){
		XAccelStart(pExample);
	}
}

int XAccelSetupInterrupt()
{
	//This functions sets up the interrupt on the ARM
	int result;
	XScuGic_Config *pCfg = XScuGic_LookupConfig(XPAR_SCUGIC_SINGLE_DEVICE_ID);
	if (pCfg == NULL){
		print("Interrupt Configuration Lookup Failed\n\r");
		return XST_FAILURE;
	}
	result = XScuGic_CfgInitialize(&ScuGic,pCfg,pCfg->CpuBaseAddress);
	if(result != XST_SUCCESS){
		return result;
	}
	// self test
	result = XScuGic_SelfTest(&ScuGic);
	if(result != XST_SUCCESS){
		return result;
	}
	// Initialize the exception handler
	Xil_ExceptionInit();
	
	// Register the exception handler
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,(Xil_ExceptionHandler)XScuGic_InterruptHandler,&ScuGic);
	
	//Enable the exception handler
	Xil_ExceptionEnable();
	
	// Connect the Adder ISR to the exception table
	result = XScuGic_Connect(&ScuGic,XPAR_FABRIC_HLS_ACCEL_1_INTERRUPT_INTR,(Xil_InterruptHandler)XAccelIsr,&xcompute_Network);
	
	if(result != XST_SUCCESS){
		return result;
	}

	XScuGic_Enable(&ScuGic,XPAR_FABRIC_HLS_ACCEL_1_INTERRUPT_INTR);
	return XST_SUCCESS;
}

int Setup_HW_Accelerator(float A[IO_N_SIZE],  float res_hw[TIME_MUX_FACTOR+TIME_MUX_FACTOR], int dma_size)
{
	//Setup the Vivado HLS Block
	int status = XAccelSetup();
	if(status != XST_SUCCESS){
		print("Error: example setup failed\n");
		return XST_FAILURE;
	}
	
	status =  XAccelSetupInterrupt();
	if(status != XST_SUCCESS){
		print("Error: interrupt setup failed\n");
		return XST_FAILURE;
	}

	XAccelStart(&xcompute_Network);

	//flush the cache
	Xil_DCacheFlushRange((unsigned int)A,dma_size);

	Xil_DCacheFlushRange((unsigned int)res_hw,dma_size);
	print("\rCache cleared\n\r");

	return 0;
}





int Start_HW_Accelerator(void)
{
	int status = XAccelSetup();
	if(status != XST_SUCCESS){
	   print("Error: example setup failed\n");
	   return XST_FAILURE;
	}
	
	status =  XAccelSetupInterrupt();
	
	if(status != XST_SUCCESS){
		print("Error: interrupt setup failed\n");
		return XST_FAILURE;
	}

	XAccelStart(&xcompute_Network);

	return 0;
}

int Run_HW_Accelerator(float A[IO_N_SIZE], float res_hw[TIME_MUX_FACTOR+TIME_MUX_FACTOR], int dma_size)
{
	//transfer A to the Vivado HLS block
	int status = XAxiDma_SimpleTransfer(&AxiDma, (unsigned int) A, dma_size, XAXIDMA_DMA_TO_DEVICE);
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	/* Wait for transfer to be done */
	while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE)) ;

	


	//get results from the Vivado HLS block
	status = XAxiDma_SimpleTransfer(&AxiDma, (unsigned int) res_hw, dma_size, XAXIDMA_DEVICE_TO_DMA);
	
	if (status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	
	/* Wait for transfer to be done */
	while (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE)) ;

	//poll the DMA engine to verify transfers are complete
	/* Waiting for data processing */
	/* While this wait operation, the following action would be done
	 * First: First matrix will be sent.
	 * After: Multiplication will be compute.
	 * Then: Output matrix will be sent from the accelerator to DDR and
	 * it will be stored at the base address that you set in the first SimpleTransfer
	 */
	while ((XAxiDma_Busy(&AxiDma, XAXIDMA_DEVICE_TO_DMA)) || (XAxiDma_Busy(&AxiDma, XAXIDMA_DMA_TO_DEVICE))) ;

	return 0;
}



int hls_init(XHls_accel *hls_Ptr)
{
	XHls_accel_Config *cfgPtr;
   int status;

   cfgPtr = XHls_accel_LookupConfig(XPAR_XHLS_ACCEL_0_DEVICE_ID);
   if (!cfgPtr) {
      print("ERROR: Lookup of accelerator configuration failed.\n\r");
      return XST_FAILURE;
   }
   status = XHls_accel_CfgInitialize(hls_Ptr, cfgPtr);
   if (status != XST_SUCCESS) {
      print("ERROR: Could not initialize accelerator.\n\r");
      return XST_FAILURE;
   }
   return status;
}



/*
#******************************************************************************
#* Vivado_ZedBoard/hls/lib_xmmult_hw.c
#*
#* Example taken of: https://www.xilinx.com/support/documentation/application_notes/xapp1170-zynq-hls.pdf
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/
