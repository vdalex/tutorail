/*
Phase correction algorithm used in m0nka DSP radio project
Filter coefficient calculation done with Iowa Hills Hilbert Filter Designer
*/

#include "filter.h"

void UiCalcRxPhaseAdj(void)
{
	float f_coeff, f_offset, var_norm, var_inv;
	ulong i;
	int phase;
	
	fc.rx_q_num_taps = Q_NUM_TAPS;
	fc.rx_i_num_taps = I_NUM_TAPS;
	
	fc.rx_q_block_size = Q_BLOCK_SIZE;
	fc.rx_i_block_size = I_BLOCK_SIZE;

	if(ts.dmod_mode == DEMOD_LSB)	// get phase setting appropriate to mode
	{
		phase = ts.rx_iq_lsb_phase_balance;	// yes, get current gain adjustment setting for LSB
	}
	else
	{
		phase = ts.rx_iq_usb_phase_balance; // yes, get current gain adjustment setting for USB and other mdoes
	}

	var_norm = (float)phase;
	var_norm = fabs(var_norm);		// get absolute value of this gain adjustment
	var_inv = 32 - var_norm;		// calculate "inverse" of number of steps
	var_norm /= 32;					// fractionalize by the number of steps
	var_inv /= 32;					// fractionalize this one, too

	if(phase != 0)
	{
		if(phase < 0)
		{ 
			for(i = 0; i < Q_NUM_TAPS; i++)	
			{
				f_coeff = var_inv * q_rx_3k6_coeffs[i];	// get fraction of 90 degree setting
				f_offset = var_norm * q_rx_3k6_coeffs_minus[i];	// get fraction of 90.5 degree setting
				fc.rx_filt_q[i] = f_coeff + f_offset;	// synthesize new coefficient
				fc.rx_filt_i[i] = i_rx_3k6_coeffs[i];
			}
		}
		else
		{
			for(i = 0; i < Q_NUM_TAPS; i++)	
			{
				f_coeff = var_inv * q_rx_3k6_coeffs[i];	// get fraction of 90 degree setting
				f_offset = var_norm * q_rx_3k6_coeffs_plus[i];	// get fraction of 90.5 degree setting
				fc.rx_filt_q[i] = f_coeff + f_offset;	// synthesize new coefficient
				fc.rx_filt_i[i] = i_rx_3k6_coeffs[i];
			}
		}	
	}
	// not in AM mode - use normal settings
	arm_fir_init_f32((arm_fir_instance_f32 *)&FIR_I, fc.rx_i_num_taps, (float32_t *)&fc.rx_filt_i[0], &FirState_I[0], fc.rx_i_block_size);
	arm_fir_init_f32((arm_fir_instance_f32 *)&FIR_Q, fc.rx_q_num_taps, (float32_t *)&fc.rx_filt_q[0], &FirState_Q[0], fc.rx_q_block_size);
}
