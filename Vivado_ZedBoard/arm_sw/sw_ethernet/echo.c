/*

Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.

Copyright <YEAR> <COPYRIGHT HOLDER>

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/******************************************************************************
* src/sdk/echo.c: The callback response generator
*
* Modified by: Kaleb Alfaro-Badilla , 2017.
*
* This code establish the communication between server
* script called ethernet.py.
*
******************************************************************************/



#include <stdio.h>
#include <string.h>

#include "lwip/err.h"
#include "lwip/tcp.h"
#if defined (__arm__) || defined (__aarch64__)
#include "xil_printf.h"
#endif

/* Important parameters and routines for initialization
 * of the hls_IP are located here*/
#include "lib_xaccel_hw.h"


/*The sequence of this program is:
**		At first, the program is waiting
**		for init data coming from server (INIT_CELLS),
**		when data is completed the program
**		init the hardware cells by writing it
**		in memory mapped axi lite of the IP.
**		Then, the program waits data for running one step
**		of the network, and reply with the output generated.
**		If the server send the STOP_FLAG, the program get back
**		to the first state.
 * */

enum states_for_computation{
	INIT_CELLS = 0,
	COMPUTATION
};

//Offset tags for init data order

enum cell_struct_order{
	dend_vdend = 0,
	dend_calc,
	dend_pot,
	dend_hcurr,
	dend_ca2p,
	dend_icah,
	soma_gcal,
	soma_vsoma,
	soma_sodm,
	soma_sodh,
	soma_potn,
	soma_potp,
	soma_potxs,
	soma_calk,
	soma_call,
	axon_vaxon,
	axon_sodma,
	axon_sodha,
	axon_potxa
};
//*************************
//Variables for time measure

// TIMER Instance
XTmrCtr timer_dev;

unsigned int init_time, curr_time, calibration;
unsigned int begin_time;
unsigned int end_time;
unsigned int run_time_ethernet = 0;

//Variables for min and max of time
int min_t = 1000000000;
int max_t = 0;
float acc = 0;
//*************************

//Variables for flexible
int io_n_size = IO_N_SIZE;
int num_neigh_cell = NUM_NEIGH_CELLS; //Neighbors cells in FPGA
int init_data_lenght = INIT_DATA_LENGHT;
int step_data_lenght = STEP_DATA_LENGHT;
int response_data_lenght = RESPONSE_DATA_LENGHT;

/*Memory blocks for reception and sending data*/
mod_prec iniState[INIT_DATA_LENGHT/4]; //Data structure for init-data received
mod_prec iapp[IO_N_SIZE]; //Data structure for iapp received
mod_prec cellOut[RESPONSE_DATA_LENGHT/4]; //Data structure for axon and dendrite voltage data


int transfer_data() {
	return 0;
}

void print_app_header()
{
	xil_printf("\n\r\n\r-----lwIP TCP Network server ------\n\r");
	xil_printf("TCP packets sent to port 6001 will be echoed back\n\r");
}

/*******************************************************************************/

//Initialization routines

// HLS network HW instance
XHls_accel computenetwork;
// AXI DMA Instance
XAxiDma AxiDma;
// TIMER Instance
XTmrCtr timer_dev;


int init_dma_hw(){
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

int init_hw(){

	int i;
	int status, status1;
	float data;

	status1 = hls_init(&computenetwork);

	if(status1 != XST_SUCCESS){
		print("HLS peripheral setup failed\n\r");
	 	exit(-1);
	}

	/* ******************************************************************************* */
	// Init DMA
	status = init_dma_hw();
	if(status != XST_SUCCESS){
		print("\rError: DMA init failed\n");
		return XST_FAILURE;
	}
	print("\nDMA Init done\n");


	/* ******************************************************************************* */
	/* ******************************************************************************* */
		// Setup HW timer
		status = XTmrCtr_Initialize(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID);
		if(status != XST_SUCCESS)
		{
			print("Error: timer setup failed\n");
		}
		XTmrCtr_SetOptions(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID, XTC_ENABLE_ALL_OPTION);

		// Calibrate HW timer
		XTmrCtr_Reset(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID);
		init_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID);
		curr_time = XTmrCtr_GetValue(&timer_dev,XPAR_AXI_TIMER_1_DEVICE_ID);
		calibration = curr_time - init_time;

		// Loop measurement
		XTmrCtr_Reset(&timer_dev,XPAR_AXI_TIMER_1_DEVICE_ID);
		begin_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID);
		end_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID);
		run_time_ethernet = end_time - begin_time - calibration;


		/* ******************************************************************************* */


	/* Start init  */

	for(i=0;i<init_data_lenght/4; i+=19){

    	XHls_accel_Write_local_state0_dend_V_dend_Words(&computenetwork, i/19,(int *) &iniState[i+dend_vdend], 1);
		XHls_accel_Write_local_state0_dend_Hcurrent_q_Words(&computenetwork, i/19,(int *) &iniState[i+dend_hcurr], 1);
		XHls_accel_Write_local_state0_dend_Calcium_r_Words(&computenetwork, i/19,(int *) &iniState[i+dend_calc], 1);
		XHls_accel_Write_local_state0_dend_Potassium_s_Words(&computenetwork, i/19,(int *) &iniState[i+dend_pot], 1);
		XHls_accel_Write_local_state0_dend_I_CaH_Words(&computenetwork, i/19,(int *) &iniState[i+dend_icah], 1);
		XHls_accel_Write_local_state0_dend_Ca2Plus_Words(&computenetwork, i/19,(int *) &iniState[i+dend_ca2p], 1);
		XHls_accel_Write_local_state0_soma_g_CaL_Words(&computenetwork, i/19,(int *) &iniState[i+soma_gcal], 1);
		XHls_accel_Write_local_state0_soma_V_soma_Words(&computenetwork, i/19,(int *) &iniState[i+soma_vsoma], 1);
		XHls_accel_Write_local_state0_soma_Sodium_m_Words(&computenetwork, i/19,(int *) &iniState[i+soma_sodm], 1);
		XHls_accel_Write_local_state0_soma_Sodium_h_Words(&computenetwork, i/19,(int *) &iniState[i+soma_sodh], 1);
		XHls_accel_Write_local_state0_soma_Calcium_k_Words(&computenetwork, i/19,(int *) &iniState[i+soma_calk], 1);
		XHls_accel_Write_local_state0_soma_Calcium_l_Words(&computenetwork, i/19,(int *) &iniState[i+soma_call], 1);
		XHls_accel_Write_local_state0_soma_Potassium_n_Words(&computenetwork, i/19,(int *) &iniState[i+soma_potn], 1);
		XHls_accel_Write_local_state0_soma_Potassium_p_Words(&computenetwork, i/19,(int *) &iniState[i+soma_potp], 1);
		XHls_accel_Write_local_state0_soma_Potassium_x_s_Words(&computenetwork, i/19,(int *) &iniState[i+soma_potxs], 1);
		XHls_accel_Write_local_state0_axon_V_axon_Words(&computenetwork, i/19,(int *) &iniState[i+axon_vaxon], 1);
		XHls_accel_Write_local_state0_axon_Sodium_m_a_Words(&computenetwork, i/19,(int *) &iniState[i+axon_sodma], 1);
		XHls_accel_Write_local_state0_axon_Sodium_h_a_Words(&computenetwork, i/19,(int *) &iniState[i+axon_sodha], 1);
		XHls_accel_Write_local_state0_axon_Potassium_x_a_Words(&computenetwork, i/19,(int *) &iniState[i+axon_potxa], 1);
    }
	data = 0.04;

    for(i=0;i<io_n_size; i++){
		XHls_accel_Write_Connectivity_Matrix_Words(&computenetwork, i,(int *) &data, 1);
	}
	XHls_accel_Set_Mux_Factor(&computenetwork,num_neigh_cell);
	XHls_accel_Set_N_Size(&computenetwork, io_n_size);
	//XHls_accel_Set_Conn_Matrix_Size(&computenetwork, io_n_size);


    return 0;

}

int ComputeNetwork(){
	int status;
	static int i=0;


	end_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID);
	run_time_ethernet = end_time - begin_time - calibration;
	if(min_t>run_time_ethernet){min_t=run_time_ethernet;}
	if(max_t<run_time_ethernet){max_t=run_time_ethernet;}
	acc += run_time_ethernet;

	status = Setup_HW_Accelerator(iapp, cellOut, DMA_SIZE);

	status = Start_HW_Accelerator();

	status = Run_HW_Accelerator(iapp, cellOut, DMA_SIZE);

	// Loop measurement
	XTmrCtr_Reset(&timer_dev,XPAR_AXI_TIMER_1_DEVICE_ID);
	begin_time = XTmrCtr_GetValue(&timer_dev, XPAR_AXI_TIMER_1_DEVICE_ID);

	i++;

	return 0;
}


/*******************************************************************************/

//If a packet comes, it gets here for generate callback msg
err_t recv_callback(void *arg, struct tcp_pcb *tpcb,
                               struct pbuf *p, err_t err)
{
	/* do not read the packet if we are not in ESTABLISHED state */
	if (!p) {
		tcp_close(tpcb);
		tcp_recv(tpcb, NULL);
		return ERR_OK;
	}

	/* indicate that the packet has been received */
	tcp_recved(tpcb, p->len);



	static int State_counter=0; //State operation mode
	static int init_array_offset_counter=0;
	static int iapp_array_offset_counter=0;
	const int first_reply = STOP_FLAG;
	int i;

	if(State_counter==INIT_CELLS){
		if(init_array_offset_counter<1){
			*&io_n_size = *(int *)(p->payload+0);
			*&num_neigh_cell = *(int *)(p->payload+4);
			for(i=8; i < p->len; i+=4){
				*(int *)&iniState[(i+init_array_offset_counter)/4]= *(int *)(p->payload+i);
				//printf("%d\t%d\t%d\t%f\n",p->len,i,init_array_offset_counter,iniState[(i+init_array_offset_counter)/4]);
			}
			/*count how many bytes has been received*/
			init_array_offset_counter += p->len;
			init_array_offset_counter += -8;
			pbuf_free(p);
			/*update values*/
			init_data_lenght = CELL_STRUCT_SIZE*num_neigh_cell*PRECISION_DATA_SIZE;
			step_data_lenght = (io_n_size+1)*PRECISION_DATA_SIZE;
			response_data_lenght = 2*num_neigh_cell*4;
			//printf("Nsize:%d\tMux_factor:%d\n",io_n_size,num_neigh_cell);
		}else{
			/*Data is not completed by now*/
			if(init_array_offset_counter<init_data_lenght){
				/*Copy the data payload of the packet in memory*/
				for(i=0; i < p->len; i+=4){
					*(int *)&iniState[(i+init_array_offset_counter)/4]= *(int *)(p->payload+i);
					//printf("%d\t%d\t%d\t%f\n",p->len,i,init_array_offset_counter,iniState[(i+init_array_offset_counter)/4]);
				}
				/*count how many bytes has been received*/
				init_array_offset_counter += p->len;
				pbuf_free(p);
			}
				/*If the data received is complete, start init of HW*/
			if(init_array_offset_counter==init_data_lenght){
				init_hw();
				State_counter=COMPUTATION;
				init_array_offset_counter=0;
				print("ENDINIT");

				/* in this case, we assume that the payload is < TCP_SND_BUF */
				if (tcp_sndbuf(tpcb) > 4) {
						err = tcp_write(tpcb, &first_reply, 4, 1); //reply finished flag
				} else xil_printf("no space in tcp_sndbuf\n\r");
			}
		}
	}else{
		if(State_counter==COMPUTATION){
			// If computation data is not complete yet
			if(iapp_array_offset_counter<step_data_lenght){
				for(i=0; i < p->len; i+=4){
					*(int *)&iapp[(i+iapp_array_offset_counter)/4]= *(int *)(p->payload+i);
			//		printf("%d\t%d\t%d\t%f\n",p->len,i,iapp_array_offset_counter,iapp[(i+iapp_array_offset_counter)/4]);
				}
				//printf("len %d\toff %d\n",p->len,iapp_array_offset_counter);
				iapp_array_offset_counter += p->len;

				//printf("len %d\toff %d\n",p->len,iapp_array_offset_counter);

				pbuf_free(p);

			}
			//printf("%d\t%d\t%d\t%f\n",p->len,i,iapp_array_offset_counter,iapp[100]);
			//If data is complete, Compute Network
			if(iapp_array_offset_counter==step_data_lenght){

				/*If the stop flag is received*/

				if(*(int *)&iapp[N_SIZE_TEST]==STOP_FLAG){
					State_counter=INIT_CELLS;
					printf("RESET\nMin time: %d\tMax time:%d\tAvrg: %f\n",min_t,max_t,acc/(SIM_STEP_TEST*100000));
					acc = 0;
				}
				ComputeNetwork();
				iapp_array_offset_counter=0;
				//printf("REPLYING\t%f\t%f\n",cellOut[0],cellOut[999]);
				if (tcp_sndbuf(tpcb) > response_data_lenght) {
					err = tcp_write(tpcb, cellOut, response_data_lenght, 1);
				} else
					print("no space in tcp_sndbuf\n\r");
			}
			;

		}
	}


	/* free the received pbuf */


	return ERR_OK;
}

err_t accept_callback(void *arg, struct tcp_pcb *newpcb, err_t err)
{
	static int connection = 1;

	/* set the receive callback for this connection */
	tcp_recv(newpcb, recv_callback);

	/* just use an integer number indicating the connection id as the
	   callback argument */
	tcp_arg(newpcb, (void*)(UINTPTR)connection);

	/* increment for subsequent accepted connections */
	connection++;

	return ERR_OK;
}


int start_application()
{
	struct tcp_pcb *pcb;
	err_t err;
	unsigned port = 7;

	/* create new TCP PCB structure */
	pcb = tcp_new();
	if (!pcb) {
		xil_printf("Error creating PCB. Out of Memory\n\r");
		return -1;
	}

	/* bind to specified @port */
	err = tcp_bind(pcb, IP_ADDR_ANY, port);
	if (err != ERR_OK) {
		xil_printf("Unable to bind to port %d: err = %d\n\r", port, err);
		return -2;
	}

	/* we do not need any arguments to callback functions */
	tcp_arg(pcb, NULL);

	/* listen for connections */
	pcb = tcp_listen(pcb);
	if (!pcb) {
		xil_printf("Out of memory while tcp_listen\n\r");
		return -3;
	}

	/* specify callback to use for incoming connections */
	tcp_accept(pcb, accept_callback);

	xil_printf("TCP echo server started @ port %d\n\r", port);

	return 0;
}

/******************************************************************************
* src/sdk/echo.c: The callback response generator
*
* Modified by: Kaleb Alfaro-Badilla , 2017.
*
* This code establish the communication between server
* script called ethernet.py.
*
******************************************************************************/

