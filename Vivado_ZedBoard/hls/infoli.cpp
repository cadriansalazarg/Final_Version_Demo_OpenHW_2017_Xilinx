/*
Note: This license has also been called the "New BSD License" or "Modified BSD License". See also the 2-clause BSD License.

Copyright <YEAR> <COPYRIGHT HOLDER>

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


*/

/*
#/******************************************************************************
#* Vivado_ZedBoard/hls/infoli.cpp
#*
#* Written by: George Smaragdos.
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
#include "infoli.h"



void ComputeNetwork(cellState local_state0[MAX_TIME_MUX], mod_prec neighVdendE[NUM_NEIGH_CELLS], mod_prec iAppin[MAX_TIME_MUX], int N_Size, int Mux_Factor, mod_prec Connectivity_Matrix[CONN_MATRIX_SIZE], mod_prec cellOut[MAX_TIME_MUX], mod_prec neighVdendOut[MAX_TIME_MUX], mod_prec Test[TESTERNUMBER]){
	
	int j;  

	//returnState AxonOut;
	mod_prec neighVdend0[MAX_TIME_MUX+NUM_NEIGH_CELLS];  // Changing MAX_N_SIZE by TIME_MUX_FACTOR
	cellState new_state0[MAX_TIME_MUX]; 		// Changing MAX_TIME_MUX by TIME_MUX_FACTOR

	//Neighboring inputs preparation
	ComputeNetwork_label2:	for(j=0;j<Mux_Factor;j++){ // Se inicializan solo la cantidad de neuronas establecidas por la red
		neighVdend0[j] = local_state0[j].dend.V_dend ;		
	}
	
	
	ComputeNetwork_label20:	for(j=Mux_Factor;j<N_Size;j++){
		neighVdend0[j] = neighVdendE[j-Mux_Factor];
		//k++;		
	}
	
	//printf("El valor de la primera neurona %f\t, la segunda neurona es %f\n", neighVdend0[2], neighVdend0[3]);

	
	
	ComputeOneCellTimeMux0_label10:for(j=0;j<Mux_Factor;j++){
		new_state0[j].dend = CompDend(local_state0[j].dend, local_state0[j].soma.V_soma, iAppin[j], neighVdend0, N_Size, Connectivity_Matrix);
		new_state0[j].soma = CompSoma(local_state0[j].soma, local_state0[j].dend.V_dend, local_state0[j].axon.V_axon, Test);
		new_state0[j].axon = CompAxon(local_state0[j].axon, local_state0[j].soma.V_soma);
	}


	ComputeNetwork_label8:	for(j=0;j<Mux_Factor;j++){
		local_state0[j]=new_state0[j];
		cellOut[j] = local_state0[j].axon.V_axon;
		neighVdendOut[j] = local_state0[j].dend.V_dend ;
	}
	
}

Dend CompDend(Dend prevDend, mod_prec prevSoma , mod_prec iAppIn,mod_prec neighVdend[MAX_N_SIZE+NUM_NEIGH_CELLS], int N_Size,mod_prec Connectivity_Matrix[CONN_MATRIX_MAX]){

	struct Dend d_output;
    struct channelParams chPrms;
    struct dendCurrVoltPrms chComps, newchComps;

    //H current calculation
    chPrms.prevComp1 = prevDend.Hcurrent_q;
    d_output.Hcurrent_q = DendHCurr(chPrms, prevDend.V_dend);
    
    //Ca current calculation
    chPrms.prevComp1 = prevDend.Calcium_r;
    d_output.Calcium_r  = DendCaCurr(chPrms, prevDend.V_dend);
    
    //K plus current calculation
    chPrms.prevComp1 = prevDend.Potassium_s;
    chPrms.prevComp2 = prevDend.Ca2Plus;
    d_output.Potassium_s = DendKCurr(chPrms );
    
    //K plus current calculation
    chPrms.prevComp1 = prevDend.Ca2Plus;
    chPrms.prevComp2 =prevDend.I_CaH;
    d_output.Ca2Plus  = DendCal(chPrms);

    //Neighbors Ic accumulation
    chComps.iC = IcNeighbors(neighVdend, prevDend.V_dend,N_Size,Connectivity_Matrix);
    
    //Compartment output calculation
    chComps.iApp = iAppIn;
    chComps.vDend = prevDend.V_dend;
    chComps.vSoma = prevSoma;
    chComps.q = d_output.Hcurrent_q;
    chComps.r = d_output.Calcium_r;
    chComps.s = d_output.Potassium_s;
    newchComps = DendCurrVolt(chComps);
    d_output.I_CaH = newchComps.newI_CaH;
    d_output.V_dend = newchComps.newVDend;

    return d_output;
}

mod_prec DendHCurr(struct channelParams chPrms , mod_prec prevV_dend){

    mod_prec q_inf, tau_q, dq_dt, q_local;

    //opt for division by constant
    mod_prec const div_four = 0.25;

    //Get inputs
    mod_prec prevHcurrent_q = chPrms.prevComp1;

    // Update dendritic H current component
    q_inf = 1 /(1 + expf((prevV_dend + 80) * div_four));
    tau_q = 1 /(expf(-0.086 * prevV_dend - 14.6) + expf(0.070 * prevV_dend - 1.87));
    dq_dt = (q_inf - prevHcurrent_q) / tau_q;
    q_local = DELTA * dq_dt + prevHcurrent_q;
    //Put result
    return q_local;
}

mod_prec DendCaCurr(struct channelParams chPrms, mod_prec prevV_dend){

    mod_prec alpha_r, beta_r, r_inf, tau_r, dr_dt, r_local;

    //opt for division by constant
    mod_prec const div_five = 0.2;
    mod_prec const div_thirteen = 1/13.9;
    
    //Get inputs
    mod_prec prevCalcium_r = chPrms.prevComp1;

    // Update dendritic high-threshold Ca current component
    alpha_r = 1.7 / (1 + expf( -(prevV_dend - 5) *div_thirteen));
    beta_r = 0.02 * (prevV_dend + 8.5) / (expf((prevV_dend + 8.5) * div_five) - 1);
    r_inf = alpha_r / (alpha_r + beta_r);
    tau_r = 5 / (alpha_r + beta_r);
    dr_dt = (r_inf - prevCalcium_r) / tau_r;
    r_local = DELTA * dr_dt + prevCalcium_r;
    
    //Put result
    return r_local;
}

mod_prec DendKCurr(struct channelParams chPrms){

    mod_prec  alpha_s, beta_s, s_inf, tau_s, ds_dt, s_local;

    //Get inputs
    mod_prec prevPotassium_s = chPrms.prevComp1;
    mod_prec prevCa2Plus = chPrms.prevComp2;

    // Update dendritic Ca-dependent K current component
    alpha_s = min((0.00002*prevCa2Plus), 0.01);
    beta_s = 0.015;
    s_inf = alpha_s / (alpha_s + beta_s);
    tau_s = 1 / (alpha_s + beta_s);
    ds_dt = (s_inf - prevPotassium_s) / tau_s;
    s_local = DELTA * ds_dt + prevPotassium_s;

    //Put result
    return s_local;
}

mod_prec DendCal(struct channelParams chPrms){

    mod_prec  dCa_dt, Ca2Plus_local;

    //Get inputs
    mod_prec prevCa2Plus = chPrms.prevComp1;
    mod_prec prevI_CaH = chPrms.prevComp2;

    // update Calcium concentration
    dCa_dt = -3 * prevI_CaH - 0.075 * prevCa2Plus;
    Ca2Plus_local = DELTA * dCa_dt + prevCa2Plus;
    //Put result
    return Ca2Plus_local;
}

dendCurrVoltPrms DendCurrVolt(struct dendCurrVoltPrms chComps){

    //Local variables
    mod_prec I_sd, I_CaH, I_K_Ca, I_ld, I_h, dVd_dt;

    //Get inputs
    mod_prec I_c = chComps.iC;
    mod_prec I_app = chComps.iApp;
    mod_prec prevV_dend = chComps.vDend;
    mod_prec prevV_soma = chComps.vSoma;
    mod_prec q = chComps.q;
    mod_prec r = chComps.r;
    mod_prec s = chComps.s;

    // DENDRITIC CURRENTS

    // Soma-dendrite interaction current I_sd
    I_sd   = (G_INT / (1 - P1)) * (prevV_dend - prevV_soma);
    // Inward high-threshold Ca current I_CaH
    I_CaH  =  G_CAH * r * r * (prevV_dend - V_CA);
    // Outward Ca-dependent K current I_K_Ca
    I_K_Ca =  G_K_CA * s * (prevV_dend - V_K);
    // Leakage current I_ld
    I_ld   =  G_LD * (prevV_dend - V_L);
    // Inward anomalous rectifier I_h
    I_h    =  G_H * q * (prevV_dend - V_H);

    dVd_dt = (-(I_CaH   + I_sd  + I_ld + I_K_Ca + I_c + I_h) + I_app) / C_M;

    //Put result (update V_dend)
    chComps.newVDend = DELTA * dVd_dt + prevV_dend;
    chComps.newI_CaH = I_CaH;//This is a state value read in DendCal
    return chComps;
}
mod_prec IcNeighbors(mod_prec neighVdend[MAX_N_SIZE+NUM_NEIGH_CELLS], mod_prec prevV_dend, int N_Size ,mod_prec Connectivity_Matrix[CONN_MATRIX_MAX]){

    int i;
    mod_prec f, V, I_c, V_acc, F_acc;
   
    //opt division by constant
    mod_prec const hundred = -1/100.0;

    I_c = 0.0f;

	IcNeighbors_label0:	for(i=0;i<N_Size;i++){  // El for llega hasta 16

		V = prevV_dend - neighVdend[i];
		f= V*expf(V*V*hundred);
		F_acc=+f*Connectivity_Matrix[i] ;
		V_acc=+V*Connectivity_Matrix[i] ;
	}

	I_c = (0.8 * F_acc + 0.2 * V_acc);

    return I_c;
}


Soma CompSoma(Soma prevSoma , mod_prec prevDend, mod_prec prevAxon, mod_prec Test[TESTERNUMBER]){

	struct Soma s_output;
    struct channelParams chPrms, chPrmsNew;
    struct somaCurrVoltPrms chComps;

    // update somatic components
    // SCHWEIGHOFER:
    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Calcium_k;
    chPrms.prevComp2 = prevSoma.Calcium_l;

    //Ca current calculation
    chPrmsNew = SomaCalcium(chPrms);

    s_output.Calcium_k = chPrmsNew.newComp1;
    s_output.Calcium_l = chPrmsNew.newComp2;

    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Sodium_m;
    chPrms.prevComp2 = prevSoma.Sodium_h;

    //Sodium current calculation
    chPrmsNew = SomaSodium(chPrms);

    s_output.Sodium_m = chPrmsNew.newComp1;
    s_output.Sodium_h = chPrmsNew.newComp2;

    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Potassium_n;
    chPrms.prevComp2 = prevSoma.Potassium_p;

    //Potassium current calculation
    chPrmsNew = SomaPotassium(chPrms, Test);

    s_output.Potassium_n = chPrmsNew.newComp1;
    s_output.Potassium_p = chPrmsNew.newComp2;

    chPrms.v = prevSoma.V_soma;
    chPrms.prevComp1 = prevSoma.Potassium_x_s;

    //Potassium X current calculation
    chPrmsNew.newComp1 = SomaPotassiumX(chPrms);

    s_output.Potassium_x_s = chPrmsNew.newComp1;
    //Compartment output calculation
    chComps.g_CaL = prevSoma.g_CaL;
    s_output.g_CaL = prevSoma.g_CaL;
    chComps.vDend = prevDend;
    chComps.vSoma = prevSoma.V_soma;
    chComps.vAxon = prevAxon;
    chComps.k = s_output.Calcium_k;
    chComps.l = s_output.Calcium_l;
    chComps.m = s_output.Sodium_m;
    chComps.h = s_output.Sodium_h;
    chComps.n = s_output.Potassium_n;
    chComps.x_s = s_output.Potassium_x_s;
    chComps.newVSoma = SomaCurrVolt(chComps, Test);

    s_output.g_CaL = prevSoma.g_CaL;
    s_output.V_soma = chComps.newVSoma;

    return s_output;
}

channelParams SomaCalcium(struct channelParams chPrms){

    mod_prec k_inf, l_inf, tau_k, tau_l, dk_dt, dl_dt, k_local, l_local;

    //Get inputs
    mod_prec prevV_soma = chPrms.v;
    mod_prec prevCalcium_k = chPrms.prevComp1;
    mod_prec prevCalcium_l = chPrms.prevComp2;

    //opt for division with constant
    mod_prec const four = 1/4.2;
    mod_prec const eight = 1/8.5; //no change in area by synthesizer on V7
    mod_prec const thirty = 1/30.0;  //no change in area by synthesizer on V7
    mod_prec const seven = 1/7.3; //no change in area by synthesizer on V7

    k_inf = (1 / (1 + expf(-1 * (prevV_soma + 61)  * four)));
    l_inf = (1 / (1 + expf((     prevV_soma + 85.5) * eight)));
    tau_k = 1;
    tau_l = ((20 * expf((prevV_soma + 160) * thirty) / (1 + expf((prevV_soma + 84) * seven ))) +35);
    dk_dt = (k_inf - prevCalcium_k) / tau_k;
    dl_dt = (l_inf - prevCalcium_l) / tau_l;
    k_local = DELTA * dk_dt + prevCalcium_k;
    l_local = DELTA * dl_dt + prevCalcium_l;
    //Put result
    chPrms.newComp1= k_local;
    chPrms.newComp2= l_local;

    return chPrms;
}

channelParams SomaSodium(struct channelParams chPrms){

    mod_prec m_inf, h_inf, tau_h, dh_dt, m_local, h_local;

    //Get inputs
    mod_prec prevV_soma = chPrms.v;
    //mod_prec prevSodium_m = *chPrms->prevComp1;
    mod_prec prevSodium_h = chPrms.prevComp2;

    //opt for division by constant
    mod_prec const five_five = 1/5.5; //no change in area by synthesizer on V7
    mod_prec const five_eight = -1/5.8; //no change in area by synthesizer on V7
    mod_prec const thirty_three = 1/33.0;  //no change in area by synthesizer on V7

    // RAT THALAMOCORTICAL SODIUM:

    m_inf   = 1 / (1 + (expf((-30 - prevV_soma)* five_five)));
    h_inf   = 1 / (1 + (expf((-70 - prevV_soma)* five_eight)));
    tau_h   =       3 * expf((-40 - prevV_soma)* thirty_three);
    dh_dt   = (h_inf - prevSodium_h)/tau_h;
    m_local       = m_inf;
    h_local       = prevSodium_h + DELTA * dh_dt;

    //Put result
    chPrms.newComp1 = m_local;
    chPrms.newComp2 = h_local;
    return chPrms;
}

channelParams SomaPotassium(struct channelParams chPrms, mod_prec Test[TESTERNUMBER]){

    mod_prec n_inf, p_inf, tau_n, dn_dt, dp_dt, n_local, p_local;
    
    //Get inputs
    mod_prec prevV_soma = chPrms.v;
    mod_prec prevPotassium_n = chPrms.prevComp1;
    mod_prec prevPotassium_p = chPrms.prevComp2;

    //opt for division by constant
    mod_prec const ten = 0.1; //precision error if 1/10 is used
    mod_prec const twelve = 1/12.0; //no change in area by synthesizer on V7
    mod_prec const nine_hundred = 1/900.0;  //no change in area by synthesizer on V7

    // NEOCORTICAL
    n_inf = 1 / (1 + expf( ( -3 - prevV_soma) * ten));
    p_inf = 1 / (1 + expf( (51 + prevV_soma) * twelve));
    tau_n =   5 + (  47 * expf( (50 + prevV_soma) * nine_hundred));
    dn_dt = (n_inf - prevPotassium_n) / tau_n;
	dp_dt = (p_inf - prevPotassium_p) / tau_n;
    n_local = DELTA * dn_dt + prevPotassium_n;
    p_local = DELTA * dp_dt + prevPotassium_p;

    //Put result
    chPrms.newComp1 = n_local;
    chPrms.newComp2 = p_local;
    Test[0] = prevV_soma;
    Test[1] = prevPotassium_p;
    Test[2] = p_inf;
    Test[3] = tau_n;
    Test[4] = dp_dt;
    Test[5] = p_local;
    return chPrms;
}

mod_prec SomaPotassiumX(struct channelParams chPrms){

    mod_prec alpha_x_s, beta_x_s, x_inf_s, tau_x_s, dx_dt_s, x_s_local;

    //Get inputs
    mod_prec prevV_soma = chPrms.v;
    mod_prec prevPotassium_x_s = chPrms.prevComp1;

    //opt for division by constant
     mod_prec const ten = 0.1; //no change in area by synthesizer on V7

    // Voltage-dependent (fast) potassium
    alpha_x_s = 0.13 * (prevV_soma + 25) / (1 - expf(-(prevV_soma + 25) * ten));
    beta_x_s  = 1.69 * expf(-0.0125 * (prevV_soma + 35));
    x_inf_s   = alpha_x_s / (alpha_x_s + beta_x_s);
    tau_x_s   =         1 / (alpha_x_s + beta_x_s);
    dx_dt_s   = (x_inf_s - prevPotassium_x_s) / tau_x_s;
    x_s_local       = 0.05 * dx_dt_s + prevPotassium_x_s;

    //Put result
    chPrms.newComp1 = x_s_local;
    return chPrms.newComp1;
}

mod_prec SomaCurrVolt(struct somaCurrVoltPrms chComps , mod_prec Test[TESTERNUMBER]){

    //Local variables
    mod_prec I_ds, I_CaL, I_Na_s, I_ls, I_Kdr_s, I_K_s, I_as, dVs_dt;

    //Get inputs
    mod_prec g_CaL = chComps.g_CaL;
    mod_prec prevV_dend = chComps.vDend;
    mod_prec prevV_soma = chComps.vSoma;
    mod_prec prevV_axon = chComps.vAxon;
    mod_prec k = chComps.k;
    mod_prec l = chComps.l;
    mod_prec m = chComps.m;
    mod_prec h = chComps.h;
    mod_prec n = chComps.n;
    mod_prec x_s = chComps.x_s;
    
    // SOMATIC CURRENTS

    // Dendrite-soma interaction current I_ds
    I_ds  = (G_INT / P1) * (prevV_soma - prevV_dend);
    // Inward low-threshold Ca current I_CaL
    I_CaL = g_CaL * k * k * k * l * (prevV_soma - V_CA); //k^3
    // Inward Na current I_Na_s
    I_Na_s  = G_NA_S * m * m * m * h * (prevV_soma - V_NA);
    // Leakage current I_ls
    I_ls  = G_LS * (prevV_soma - V_L);
    // Outward delayed potassium current I_Kdr
    I_Kdr_s = G_KDR_S * n * n * n * n * (prevV_soma - V_K); // SCHWEIGHOFER
    // I_K_s
    I_K_s   = G_K_S * x_s*x_s*x_s*x_s * (prevV_soma - V_K);
    // Axon-soma interaction current I_as
    I_as    = (G_INT / (1 - P2)) * (prevV_soma - prevV_axon);

    dVs_dt = (-(I_CaL   + I_ds  + I_as + I_Na_s + I_ls   + I_Kdr_s + I_K_s)) / C_M;
    chComps.newVSoma = DELTA * dVs_dt + prevV_soma;

	Test[6] = g_CaL;
	Test[7] = prevV_dend;
	Test[8] = prevV_soma; 
	Test[9] = prevV_axon;
	Test[10] = k;
	Test[11] = l;
	Test[12] = m;
	Test[13] = h;
	Test[14] = n;
	Test[15] = x_s;
	
	Test[16] = I_ds;
	Test[17] =  I_CaL; 
	Test[18] = I_Na_s; 
	Test[19] = I_ls;
	Test[20] = I_Kdr_s; 
	Test[21] = I_K_s; 
	Test[22] = I_as;
	Test[23] = dVs_dt;
	Test[24] = chComps.newVSoma;
    return chComps.newVSoma;
}

Axon CompAxon(Axon prevAxon, mod_prec prevSoma){

	struct Axon a_output;
    struct channelParams chPrms, chPrmsNew;
    struct axonCurrVoltPrms chComps;

    // update somatic components
    // SCHWEIGHOFER:

    chPrms.v = prevAxon.V_axon;
    chPrms.prevComp1 =prevAxon.Sodium_h_a;

    //Sodium current calculation
    chPrmsNew = AxonSodium(chPrms);

    a_output.Sodium_h_a =  chPrmsNew.newComp1;
    a_output.Sodium_m_a =  chPrmsNew.newComp2;

    chPrms.v = prevAxon.V_axon;
    chPrms.prevComp1 = prevAxon.Potassium_x_a;

    //Potassium current calculation
    chPrmsNew.newComp1 = AxonPotassium(chPrms);

    a_output.Potassium_x_a =  chPrmsNew.newComp1;

    chComps.vSoma = prevSoma;
    chComps.vAxon = prevAxon.V_axon;
    //Compartment output calculation
    chComps.m_a = a_output.Sodium_m_a;
    chComps.h_a = a_output.Sodium_h_a;
    chComps.x_a = a_output.Potassium_x_a;
    chComps.newVAxon = AxonCurrVolt(chComps);

    a_output.V_axon =  chComps.newVAxon;

    return a_output;
}

channelParams AxonSodium(struct channelParams chPrms){

    mod_prec m_inf_a, h_inf_a, tau_h_a, dh_dt_a, m_a_local, h_a_local;

    //Get inputs
    mod_prec prevV_axon = chPrms.v;
    mod_prec prevSodium_h_a = chPrms.prevComp1;

    //opt for division by constant
    mod_prec const five_five = 1/5.5; //no change in area by synthesizer on V7
    mod_prec const five_eight = -1/5.8; //no change in area by synthesizer on V7
    mod_prec const thirty_three = 1/33.0;  //no change in area by synthesizer on V7

    // Update axonal Na components
    // NOTE: current has shortened inactivation to account for high
    // firing frequencies in axon hillock


    m_inf_a   = 1 / (1 + (expf((-30 - prevV_axon) * five_five)));
    h_inf_a   = 1 / (1 + (expf((-60 - prevV_axon) * five_eight)));
    tau_h_a   =     1.5 * expf((-40 - prevV_axon) * thirty_three);
    dh_dt_a   = (h_inf_a - prevSodium_h_a)/tau_h_a;
    m_a_local = m_inf_a;
    h_a_local = prevSodium_h_a + DELTA * dh_dt_a;

    //Put result
    chPrms.newComp1 = h_a_local;
    chPrms.newComp2 = m_a_local;
    return chPrms;
}

mod_prec AxonPotassium(struct channelParams chPrms){

    mod_prec alpha_x_a, beta_x_a, x_inf_a, tau_x_a, dx_dt_a, x_a_local;

    //Get inputs
    mod_prec prevV_axon = chPrms.v;
    mod_prec prevPotassium_x_a = chPrms.prevComp1;

    //opt for division by constant
    mod_prec const ten = 0.1; //no change in area by synthesizer on V7

    // D'ANGELO 2001 -- Voltage-dependent potassium
    alpha_x_a = 0.13 * (prevV_axon + 25) / (1 - expf(-(prevV_axon + 25) * ten));
    beta_x_a  = 1.69 * expf(-0.0125 * (prevV_axon + 35));
    x_inf_a   = alpha_x_a / (alpha_x_a + beta_x_a);
    tau_x_a   =         1 / (alpha_x_a + beta_x_a);
    dx_dt_a   = (x_inf_a - prevPotassium_x_a) / tau_x_a;
    x_a_local = 0.05 * dx_dt_a + prevPotassium_x_a;

    //Put result
    chPrms.newComp1 = x_a_local;
    return chPrms.newComp1;
}

mod_prec AxonCurrVolt(struct axonCurrVoltPrms chComps){

    //Local variable
    mod_prec I_Na_a, I_la, I_sa, I_K_a, dVa_dt;

    //Get inputs
    mod_prec prevV_soma = chComps.vSoma;
    mod_prec prevV_axon = chComps.vAxon;
    mod_prec m_a = chComps.m_a;
    mod_prec h_a = chComps.h_a;
    mod_prec x_a = chComps.x_a;

    // AXONAL CURRENTS
    // Sodium
    I_Na_a  = G_NA_A  * m_a * m_a * m_a * h_a * (prevV_axon - V_NA);
    // Leak

    I_la    = G_LA * (prevV_axon - V_L);
    // Soma-axon interaction current I_sa
    I_sa    = (G_INT / P2) * (prevV_axon - prevV_soma);
    // Potassium (transient)
    I_K_a   = G_K_A * x_a *x_a *x_a *x_a * (prevV_axon - V_K);
    dVa_dt = (-(I_K_a + I_sa + I_la + I_Na_a)) / C_M;
    chComps.newVAxon = DELTA * dVa_dt + prevV_axon;

    return chComps.newVAxon;
}

inline mod_prec min(mod_prec a, mod_prec b){
    return (a < b) ? a : b;
}



/*
#/******************************************************************************
#* Vivado_ZedBoard/hls/infoli.cpp
#*
#* Written by: George Smaragdos.
#* Modified by : Carlos Salazar-García, 2017
#* This source code is the kernel of the algorithm
#* 
#*
#******************************************************************************/
