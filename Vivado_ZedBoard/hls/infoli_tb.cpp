/*
Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.

Copyright 2017 Erasmus Brain Project

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.



#******************************************************************************
#* Vivado_ZedBoard/hls/infoli_tb.cpp
#*
#* Written by: George Smaragdos.
#* Modified by : Carlos Salazar-García, 2017
#* This code is the testbech when you compile using gcc
#* 
#*
#******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>
#include <time.h>
#include <stdbool.h>
#include "infoli.h"
//#include "infoli_wrapped.h"



#ifndef Native_IT

#include "dma_interface.h"

int main(void)
{
	int ret_val = 0;

	ret_val = test_matrix_mult();

	return ret_val;
}

#else


int main(int argc, char *argv[]){

    
    const char *outFileName = "InferiorOlive_Output.txt";
    const char *outFileNameVdend = "InferiorOlive_OutputVdend.txt";
    FILE *pOutFile;
    FILE *pOutFileVdend;
    
    mod_prec iAppArray[MAX_TIME_MUX];
    int i, j;
    int simSteps = 0;
    int simTime = 0;
    int N_Size = NUM_NEIGH_CELLS + TIME_MUX_FACTOR;
    int Mux_Factor = TIME_MUX_FACTOR;
    returnState cellOut;
    cellState IniArray[MAX_TIME_MUX];
    char temp[1000];//warning: this buffer may overflow
    mod_prec iApp;
    mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE];
    
    mod_prec Test[TESTERNUMBER];
    
    mod_prec neighVdendE[MAX_NEIGH_SIZE];
    mod_prec neighVdendOut[MAX_TIME_MUX];
   
    printf("Inferior Olive Model (%d cell network)\n", TIME_MUX_FACTOR);

   

    //Open output file
    pOutFile = fopen(outFileName,"w");
    if(pOutFile==NULL){
        printf("Error: Couldn't create %s\n", outFileName);
        exit(EXIT_FAILURE);
    }
    
    //Open output file
    pOutFileVdend = fopen(outFileNameVdend,"w");
    if(pOutFileVdend==NULL){
        printf("Error: Couldn't create %s\n", outFileNameVdend);
        exit(EXIT_FAILURE);
    }
    
    sprintf(temp, "#simSteps Time(ms) Input(Iapp) Output(V_axon)\n");
    fputs(temp, pOutFile);
    
    sprintf(temp, "#simSteps Time(ms) Input(Iapp) Output(V_dend)\n");
    fputs(temp, pOutFileVdend);

    

    for(j=0;j<Mux_Factor;j++){
    	IniArray[j] = InitState();
    }
	
    //Initialize g_CaL
    for(j=0;j<Mux_Factor;j++){
        IniArray[j].soma.g_CaL = 0.68f;
    }

    //initialize connection Matrix
    for (j=0;j<N_Size; j++){
    	Connectivity_Matrix[j] = CONDUCTANCE;
    }


	//printf("******************************** Initialization PC *******************************\n"); 
    //printf("******************************** DENDRITE*******************************\n"); 
    //printf("V_dend: %f\t, Hcurrent_q: %f\t, Calcium_r: %f\t, Potassium_s: %f\t, I_CaH: %f\t, Ca2Plus: %f\n",IniArray[0].dend.V_dend, IniArray[0].dend.Hcurrent_q, IniArray[0].dend.Calcium_r, IniArray[0].dend.Potassium_s, IniArray[0].dend.I_CaH, IniArray[0].dend.Ca2Plus);
    
    //printf("********************************   SOMA  *******************************\n"); 
    //printf("g_CaL: %f\t, V_soma: %f\t, Sodium_m: %f\t, Sodium_h: %f\t, Calcium_k: %f\t, Calcium_l:  %f\t, Potassium_n:  %f\t, Potassium_p:  %f\t, Potassium_x_s: %f\n", IniArray[0].soma.g_CaL, IniArray[0].soma.V_soma, IniArray[0].soma.Sodium_m, IniArray[0].soma.Sodium_h, IniArray[0].soma.Calcium_k, IniArray[0].soma.Calcium_l, IniArray[0].soma.Potassium_n, IniArray[0].soma.Potassium_p, IniArray[0].soma.Potassium_x_s );

	//printf("********************************   AXON  *******************************\n"); 
	//printf("V_axon: %f\t, Sodium_m_a: %f\t, Sodium_h_a: %f\t, Potassium_x_a: %f\n",IniArray[0].axon.V_axon, IniArray[0].axon.Sodium_m_a, IniArray[0].axon.Sodium_h_a, IniArray[0].axon.Potassium_x_a); 


    
    simTime = SIMTIME; // in miliseconds
    simSteps = ceil(simTime/DELTA);

	for(i=0;i<simSteps;i++){
            //Compute one sim step for all cells

        if(i>20000-1 && i<20500-1){ iApp = 6.0f;} // start @ 1 because skipping initial values
        else{ iApp = 0.0f;}

        for(j=0;j<Mux_Factor;j++){
            iAppArray[j] = iApp;
        }
        
        sprintf(temp, "%d %.2f %.1f ", i+1, i*0.05,  iAppArray[0]); // start @ 1 because skipping initial values
        fputs(temp, pOutFile);
        fputs(temp, pOutFileVdend);
            

        for(j=0; j<N_Size-Mux_Factor; j++){
			if(i==0){
				neighVdendE[j] = -60.0f;
			} else {
				if (j > Mux_Factor) 
					neighVdendE[j] = neighVdendOut[1];
				else
					neighVdendE[j] = neighVdendOut[j];
			}
		}
			
			
                    
        ComputeNetwork( IniArray, neighVdendE, iAppArray, N_Size, Mux_Factor,Connectivity_Matrix, cellOut.axonOut, neighVdendOut, Test);
                                     
            
        for(j=0;j<Mux_Factor;j++){
            sprintf(temp, "%d: %.8f ",j,cellOut.axonOut[j]);
            fputs(temp, pOutFile); 
            sprintf(temp, "%d: %.8f ",j,neighVdendOut[j]);
            fputs(temp, pOutFileVdend);         
        }

        sprintf(temp, "\n");
       	fputs(temp, pOutFile);
       	fputs(temp, pOutFileVdend);
       	    
    }
    

    printf("%d ms of brain time in %d simulation steps\n", simTime, simSteps);

    fclose (pOutFile);
    fclose (pOutFileVdend);
    

    return 0;
}



cellState InitState(){
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


#endif

/*
#******************************************************************************
#* Vivado_ZedBoard/hls/infoli_tb.cpp
#*
#* Written by: George Smaragdos.
#* Modified by : Carlos Salazar-García, 2017
#* This code is the testbech when you compile using gcc
#* 
#*
#******************************************************************************/
