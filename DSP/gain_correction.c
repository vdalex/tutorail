/* Gain correction algorithm used in m0nka DSP radio project */


//*----------------------------------------------------------------------------
//* Function Name       : UiCalcTxIqGainAdj
//* Object              : Calculate TX IQ Gain adjustments
//* Input Parameters    :
//* Output Parameters   :
//* Functions called    :
//*----------------------------------------------------------------------------
void UiCalcTxIqGainAdj(void)
{
	// Note:  There is a fixed amount of offset due to the fact that the phase-added Hilbert (e.g. 0, 90) transforms are
	// slightly asymmetric that is added so that "zero" is closer to being the proper phase balance.
	//
	if(ts.dmod_mode == DEMOD_LSB)
		ts.tx_adj_gain_var_i = (float)ts.tx_iq_lsb_gain_balance;		// get current gain adjustment setting for LSB
	else
		ts.tx_adj_gain_var_i = (float)ts.tx_iq_usb_gain_balance;		// get current gain adjustment setting for USB and other modes
	//
	ts.tx_adj_gain_var_i /= 1024;		// fractionalize it
	ts.tx_adj_gain_var_q = -ts.tx_adj_gain_var_i;				// get "invert" of it
	ts.tx_adj_gain_var_i += 1;		// offset it by one (e.g. 0 = unity)
	ts.tx_adj_gain_var_q += 1;
}

//*----------------------------------------------------------------------------
//* Function Name       : UiCalcRxIqGainAdj
//* Object              : Calculate RX IQ Gain adjustments
//* Input Parameters    :
//* Output Parameters   :
//* Functions called    :
//*----------------------------------------------------------------------------
void UiCalcRxIqGainAdj(void)
{
	if(ts.dmod_mode == DEMOD_AM)
		ts.rx_adj_gain_var_i = (float)ts.rx_iq_am_gain_balance;			// get current gain adjustment for AM
	else if(ts.dmod_mode == DEMOD_LSB)
		ts.rx_adj_gain_var_i = (float)ts.rx_iq_lsb_gain_balance;		// get current gain adjustment setting for LSB
	else
		ts.rx_adj_gain_var_i = (float)ts.rx_iq_usb_gain_balance;		// get current gain adjustment setting	USB and other modes
	//
	ts.rx_adj_gain_var_i /= 1024;	// fractionalize it
	ts.rx_adj_gain_var_q = -ts.rx_adj_gain_var_i;				// get "invert" of it
	ts.rx_adj_gain_var_i += 1;		// offset it by one (e.g. 0 = unity)
	ts.rx_adj_gain_var_q += 1;
}
