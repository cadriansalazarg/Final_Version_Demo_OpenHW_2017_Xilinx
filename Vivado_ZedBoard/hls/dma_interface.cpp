#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>

#include <assert.h>
#include <ap_axi_sdata.h>
#include "dma_interface.h"



typedef ap_axiu<32,4,5,5> AXI_VAL;







/* ****************************** C++ TEMPLATES ************************************** */

// functions to insert and extract elements from an axi stream
// includes conversion to correct data type

template <typename T, int U, int TI, int TD>
T pop_stream(ap_axiu <sizeof(T)*8,U,TI,TD> const &e)
{
#pragma HLS INLINE

	assert(sizeof(T) == sizeof(int));
	union
	{
		int ival;
		T oval;
	} converter;
	converter.ival = e.data;
	T ret = converter.oval;

	volatile ap_uint<sizeof(T)> strb = e.strb;
	volatile ap_uint<sizeof(T)> keep = e.keep;
	volatile ap_uint<U> user = e.user;
	volatile ap_uint<1> last = e.last;
	volatile ap_uint<TI> id = e.id;
	volatile ap_uint<TD> dest = e.dest;

	return ret;
}

template <typename T, int U, int TI, int TD> ap_axiu <sizeof(T)*8,U,TI,TD> push_stream(T const &v, bool last = false)
{
	#pragma HLS INLINE
	ap_axiu<sizeof(T)*8,U,TI,TD> e;

	assert(sizeof(T) == sizeof(int));
	union
	{
		int oval;
		T ival;
	} converter;
	converter.ival = v;
	e.data = converter.oval;

	// set it to sizeof(T) ones
	e.strb = -1;
	e.keep = 15; //e.strb;
	e.user = 0;
	e.last = last ? 1 : 0;
	e.id = 0;
	e.dest = 0;
	return e;
}




// function to be accelerated in HW wrapped with AXI4-Stream interface

//template <typename T, int SIZE, int U, int TI, int TD>
void wrapped_ComputeNetwork (cellState local_state0[MAX_TIME_MUX],  AXI_VAL in_stream[MAX_TIME_MUX+NUM_NEIGH_CELLS] , int N_Size, int Mux_Factor,mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], AXI_VAL out_stream[MAX_TIME_MUX+NUM_NEIGH_CELLS], mod_prec Test[TESTERNUMBER])
{
	
	#pragma HLS INLINE
	
	mod_prec iAppinN[MAX_TIME_MUX+NUM_NEIGH_CELLS]; // En esta variable se almacena los datos que vienen del AXI stream, es decir los iAppin y los V_dend de la otra ZedBoard
	mod_prec iAppin[MAX_TIME_MUX]; // Esta variable contiene los iAppin, es decir el estímulo de esta FPGA
	mod_prec cellOutN[MAX_TIME_MUX+NUM_NEIGH_CELLS]; // En esta variable se almacena los datos que van a ser enviado por AXI stream hacia el Zynq, este contiene los Vaxon y en la otra mitad del arreglo los Vdend
	mod_prec cellOut[MAX_TIME_MUX]; // En esta variable se almacenan los Vaxon que salen de la función Compute Network
	mod_prec neighVdendE[NUM_NEIGH_CELLS]; // En esta variable se almacenan los Vdend de la otra ZedBoard, son la entrada de Vdend de la otra ZedBoard
	mod_prec neighVdendOut[MAX_TIME_MUX]; // En esta variable se almacenan los Vdend de la ZedBoard en la que se esta ejecutando, esta es una salida de la función computeBNetwork
	
	int i;
	

	// stream in first array
	for(i=0; i<N_Size; i++){   //for(i=0; i<MAX_TIME_MUX+NUM_NEIGH_CELLS; i++){
		#pragma HLS PIPELINE II=1
		iAppinN[i] = pop_stream<float,4,5,5>(in_stream[i]);
	}
	
	for (i=0; i<Mux_Factor; i++){//for (i=0; i<MAX_TIME_MUX; i++){
		#pragma HLS PIPELINE II=1
		iAppin[i] = iAppinN[i];
	}
	
	for (i=Mux_Factor; i<N_Size; i++){ //for (i=MAX_TIME_MUX; i<NUM_NEIGH_CELLS+MAX_TIME_MUX; i++){
		#pragma HLS PIPELINE II=1
		neighVdendE[i-Mux_Factor] = iAppinN[i];
	}
	
	//printf("El valor de la primera neurona dma %f\t, la segunda neurona es %f\n", neighVdendE[0], neighVdendE[1]);
		
	
	// do HW compute Network
	ComputeNetwork(local_state0, neighVdendE, iAppin, N_Size, Mux_Factor, Connectivity_Matrix, cellOut,  neighVdendOut,  Test);
	
	
	//printf("El valor de la primera neurona %f\t, la segunda neurona es %f\n", neighVdendOut[0], neighVdendOut[1]);
	
	for(i=0; i<Mux_Factor; i++){//for(i=0; i<MAX_TIME_MUX; i++){
		#pragma HLS PIPELINE II=1
		cellOutN[i] = cellOut[i];
	}
	
	//k = 0;
	
	for(i=Mux_Factor; i<N_Size; i++){//for(i=MAX_TIME_MUX; i<MAX_TIME_MUX+NUM_NEIGH_CELLS; i++){
		#pragma HLS PIPELINE II=1
		cellOutN[i] = neighVdendOut[i-Mux_Factor];
	}

	// stream out result array
	for(i=0; i<N_Size; i++){   //for(i=0; i<MAX_TIME_MUX+NUM_NEIGH_CELLS; i++){
		#pragma HLS PIPELINE II=1
		out_stream[i] = push_stream<float,4,5,5>(cellOutN[i],i == (N_Size-1));
		//out_stream[i] = push_stream<float,4,5,5>(cellOutN[i],i == (MAX_TIME_MUX-1+NUM_NEIGH_CELLS));
	}
			
	return;
}



// test the functions


cellState InitState1(){
    cellState initState;
    //Initial dendritic parameters
    initState.dend.V_dend = -60;
    initState.dend.Calcium_r = 0.0112788;// High-threshold calcium
    initState.dend.Potassium_s = 0.0049291;// Calcium-dependent potassium
    initState.dend.Hcurrent_q = 0.0337836;// H current
    initState.dend.Ca2Plus = 3.7152;// Calcium concentration
    initState.dend.I_CaH   = 0.5;// High-threshold calcium current
    //Initial somatic parameters
    initState.soma.g_CaL = 0.68; //default arbitrary value but it should be randomized per cell
    initState.soma.V_soma = -60;
    initState.soma.Sodium_m = 1.0127807;// Sodium (artificial)
    initState.soma.Sodium_h = 0.3596066;
    initState.soma.Potassium_n = 0.2369847;// Potassium (delayed rectifier)
    initState.soma.Potassium_p = 0.2369847;
    initState.soma.Potassium_x_s = 0.1;// Potassium (voltage-dependent)
    initState.soma.Calcium_k = 0.7423159;// Low-threshold calcium
    initState.soma.Calcium_l = 0.0321349;
    // Initial axonal parameters
    initState.axon.V_axon = -60;
    //sisaza: Sodium_m_a doesn't have a state, therefore this assignment doesn'thave any effect
    initState.axon.Sodium_m_a = 0.003596066;// Sodium (thalamocortical)
    initState.axon.Sodium_h_a = 0.9;
    initState.axon.Potassium_x_a = 0.2369847;// Potassium (transient)

    return (initState);
}


//template <typename T, int SIZE, int U, int TI, int TD>
int test_matrix_mult(void)
{
	// For write the file ********************
	FILE *pOutFile;
	FILE *pOutFileVdend;
	const char *outFileName = "InferiorOlive_Output.txt";
	const char *outFileNameVdend = "InferiorOlive_OutputVdend.txt";
	char temp[1000];//warning: this buffer may overflow
	
	// Variables for the test**************
	int i, j;
	int err = 0;
	
	mod_prec iAppArray_sw[MAX_TIME_MUX]; // iAppArray para la ejecución del modelo de referencia, es el estímulo de entrada del software
	mod_prec iAppArray_hw[MAX_TIME_MUX+NUM_NEIGH_CELLS]; // iAppArray_hw para la ejecución en hardware, es el estímulo de entrada del hardware
	
	mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE];
	
	cellState IniArray_sw[MAX_TIME_MUX];  // Estado inicial de la celda modelo de referencia
	cellState IniArray_hw[MAX_TIME_MUX];  // Estado inicial de la celda
	
	mod_prec iApp;
	int simSteps = 0;
    int simTime = 0;
    
    int Mux_Factor = TIME_MUX_FACTOR;
    int N_Size = TIME_MUX_FACTOR + NUM_NEIGH_CELLS;
    
    AXI_VAL inp_stream[MAX_TIME_MUX+NUM_NEIGH_CELLS];
	AXI_VAL out_stream[MAX_TIME_MUX+NUM_NEIGH_CELLS];
	
	mod_prec Test_sw[TESTERNUMBER];
	mod_prec Test_hw[TESTERNUMBER];
	
	mod_prec cellOut_sw[MAX_TIME_MUX]; // Contiene los Vaxon de salida del modelo de referencia
	mod_prec cellOut_hw[MAX_TIME_MUX+NUM_NEIGH_CELLS]; // Contiene los Vaxon de la salida del hardware
	
	mod_prec neighVdendE[NUM_NEIGH_CELLS];   // Variable de entrada, contiene los Vdend de la otra ZedBoard
	
	mod_prec neighVdendOut_sw[MAX_TIME_MUX]; // Vdend de salida del modelo de referencia, almacena los Vdend locales
	
	//************************************************
	
	printf("Inferior Olive Model (%d cell network)\n", TIME_MUX_FACTOR);
	
	//Open output file *********************************
    pOutFile = fopen(outFileName,"w");
    if(pOutFile==NULL){
        printf("Error: Couldn't create %s\n", outFileName);
        exit(EXIT_FAILURE);
    }
    
    
    printf("Inferior Olive Model (%d cell network)\n", TIME_MUX_FACTOR);
	
	//Open output file *********************************
    pOutFileVdend = fopen(outFileNameVdend,"w");
    if(outFileNameVdend==NULL){
        printf("Error: Couldn't create %s\n", outFileNameVdend);
        exit(EXIT_FAILURE);
    }
    
    
    sprintf(temp, "#simSteps Time(ms) Input(Iapp) Output(V_axon)\n");
    fputs(temp, pOutFile);
    
    sprintf(temp, "#simSteps Time(ms) Input(Iapp) Output(V_dend)\n");
    fputs(temp, pOutFileVdend);
    
    
	//************************* Initialization ***************************************
	    
    for (j=0;j<CONN_MATRIX_SIZE; j++){
    	Connectivity_Matrix[j] = CONDUCTANCE;
    }
    
    //for(j=0;j<MAX_TIME_MUX;j++){
    for(j=0;j<Mux_Factor;j++){
    	IniArray_sw[j] = InitState1();
    	IniArray_hw[j] = InitState1();
    }
    
    //for(j=0;j<MAX_TIME_MUX;j++){
    for(j=0;j<Mux_Factor;j++){
        IniArray_sw[j].soma.g_CaL = 0.68f;
        IniArray_hw[j].soma.g_CaL = 0.68f;
    }
    
    simTime = SIMTIME; // in miliseconds SIMTIME = 1
    simSteps = ceil(simTime/DELTA); // DELTA es igual a 0.05 = 1/20
    


    for(i=0;i<simSteps;i++){ 
		//Input stimulus is generated
	    if(i>20000-1 && i<20500-1){ iApp = 6.0f;} // start @ 1 because skipping initial values
        else{ iApp = 0.0f;}
        
        for(j=0;j<Mux_Factor;j++){ //for(j=0;j<MAX_TIME_MUX;j++){  // Se carga la primera parte del estímulo ******************
            iAppArray_hw[j] = iApp; // Primera parte del estimulo para el hardware
            iAppArray_sw[j] = iApp; // Estimulo para el software
        }
        
        for(j=Mux_Factor;j<N_Size;j++){ //for(j=MAX_TIME_MUX;j<NUM_NEIGH_CELLS+MAX_TIME_MUX;j++){  // Se carga la segunda parte del arreglo
			if (i==0){  // Si es el primer paso, se cargan el valor de Vdend por defecto
				iAppArray_hw[j] = -60.0f;
			}else{
				iAppArray_hw[j] = cellOut_hw[j]; // Revisar esto
			}
        }
        
        for(j=0;j<N_Size-Mux_Factor;j++){  //for(j=0;j<NUM_NEIGH_CELLS;j++){ 
			if (i==0){ // si es el primer paso
				neighVdendE[j] = -60.0f;
			}else{
				neighVdendE[j] = neighVdendOut_sw[j];
			}
        }
        
        
        // Print in the file the step, the time, and the stimulus *********
        sprintf(temp, "%d %.2f %.1f ", i+1, i*0.05,  iAppArray_sw[0]); // start @ 1 because skipping initial values
        fputs(temp, pOutFile);
        fputs(temp, pOutFileVdend);
        
        
        // stream in the first input  array
		for(j=0; j<N_Size; j++){   //for(j=0; j<MAX_TIME_MUX+NUM_NEIGH_CELLS; j++){
			inp_stream[j] = push_stream<float,4,5,5>(iAppArray_hw[j],0);
		}
		
		//printf("El valor de la primera neurona %f\t, la segunda neurona es %f\n", iAppArray_hw[2], iAppArray_hw[3]);
		
		// Do the algorithm in software ********************************
		ComputeNetwork( IniArray_sw, neighVdendE, iAppArray_sw, N_Size, Mux_Factor, Connectivity_Matrix, cellOut_sw, neighVdendOut_sw, Test_sw);
        
        // Do the algorithm in hardware ********************************
        HLS_accel (IniArray_hw,  inp_stream, N_Size, Mux_Factor, Connectivity_Matrix, out_stream, Test_hw);
        
        // extract the output array from the out stream 
		for(j=0; j<N_Size; j++){//for(j=0; j<MAX_TIME_MUX+NUM_NEIGH_CELLS; j++){
			cellOut_hw[j] = pop_stream<float,4,5,5>(out_stream[j]);
		}
				
		// Compare the software result vs hardware results
		for (j = 0; j<Mux_Factor; j++){     //for (j = 0; j<MAX_TIME_MUX; j++){ // Compara los Vaxon
			if (cellOut_hw[j] != cellOut_sw[j])
				err++;
			if (cellOut_hw[j+Mux_Factor] != neighVdendOut_sw[j])
				err++;
				
			//printf("Step %d y Neuron %d Vaxon_SW: %f\t y Vaxon_HW %f\t", i, j, cellOut_sw[j], cellOut_hw[j]);
			//printf("VDend_SW: %f\t and VDend_HW %f\n", neighVdendOut_sw[j], cellOut_hw[j+MAX_TIME_MUX]);
		}
				
		// Write the result in the file *******************************
		for(j=0;j<Mux_Factor;j++){   //for(j=0;j<MAX_TIME_MUX;j++){
            sprintf(temp, "%d: %.8f ", j, cellOut_hw[j]);
            fputs(temp, pOutFile);
        }
        
        // Write the result in the file *******************************
		for(j=Mux_Factor;j<N_Size;j++){   //for(j=MAX_TIME_MUX;j<MAX_TIME_MUX+NUM_NEIGH_CELLS;j++){
            sprintf(temp, "%d: %.8f ", j-Mux_Factor, cellOut_hw[j]); //sprintf(temp, "%d: %.8f ", j-MAX_TIME_MUX, cellOut_hw[j]);
            fputs(temp, pOutFileVdend);
        }
        
        sprintf(temp, "\n");
       	fputs(temp, pOutFile);
       	fputs(temp, pOutFileVdend);
	}
    
    if (err == 0){
		printf("Results match! Test successful!\r\n");
	}else{
		printf("Test failed!\r\n");
		fclose (pOutFile); // Close the output file
		fclose (pOutFileVdend); // Close the output file
		return 1;
	}
	
	fclose (pOutFile); // Close the output file
	fclose (pOutFileVdend); // Close the output file

	return 0;        
}