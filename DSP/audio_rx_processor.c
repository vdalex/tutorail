//
//*----------------------------------------------------------------------------
//* Function Name       : audio_rx_processor
//* Object              :
//* Object              : audio sample processor
//* Input Parameters    :
//* Output Parameters   :
//* Functions called    :
//*----------------------------------------------------------------------------
static void audio_rx_processor(int16_t *src, int16_t *dst, int16_t size)
{

	static ulong 		i;
	//
	int16_t				psize;		// processing size, with decimation
	//
	static ulong		lms1_inbuf = 0, lms1_outbuf = 0;
	static ulong		lms2_inbuf = 0, lms2_outbuf = 0;
	float				post_agc_gain_scaling;


	psize = size/(int16_t)ads.decimation_rate;	// rescale sample size inside decimated portion based on decimation factor
	//
	audio_rx_noise_blanker(src, size);		// do noise blanker function
	//
	//
	// ------------------------
	// Split stereo channels
	for(i = 0; i < size/2; i++)
	{
		//
		// Collect I/Q samples
		if(sd.state == 0)
		{
			sd.FFT_Samples[sd.samp_ptr] = (float32_t)(*(src + 1));	// get floating point data for FFT for spectrum scope/waterfall display
			sd.samp_ptr++;
			sd.FFT_Samples[sd.samp_ptr] = (float32_t)(*(src));
			sd.samp_ptr++;

			// On overload, update state machine,
			// reset pointer and wait
			if(sd.samp_ptr >= FFT_IQ_BUFF_LEN1*2)
			{
				sd.samp_ptr = 0;
				sd.state    = 1;
			}
		}
		//
		if(*src > ADC_CLIP_WARN_THRESHOLD/4)	{		// This is the release threshold for the auto RF gain
			ads.adc_quarter_clip = 1;
			if(*src > ADC_CLIP_WARN_THRESHOLD/2)	{		// This is the trigger threshold for the auto RF gain
					ads.adc_half_clip = 1;
					if(*src > ADC_CLIP_WARN_THRESHOLD)			// This is the threshold for the red clip indicator on S-meter
						ads.adc_clip = 1;
			}
		}
		//
		// 16 bit format - convert to float and increment
		ads.i_buffer[i] = (float32_t)*src++;
		ads.q_buffer[i] = (float32_t)*src++;
		//
	}
	//
	// Apply gain corrections for I/Q gain balancing
	//
	arm_scale_f32((float32_t *)ads.i_buffer, (float32_t)ts.rx_adj_gain_var_i, (float32_t *)ads.i_buffer, size/2);
	//
	arm_scale_f32((float32_t *)ads.q_buffer, (float32_t)ts.rx_adj_gain_var_q, (float32_t *)ads.q_buffer, size/2);
	//
	//
	if(ts.iq_freq_mode)	{		// is receive frequency conversion to be done?
		if(ts.iq_freq_mode == 1)			// Yes - "RX LO LOW" mode
			audio_rx_freq_conv(size, 1);
		else								// "RX LO HIGH" mode
			audio_rx_freq_conv(size, 0);
	}
	//
	// ------------------------
	// IQ SSB processing - Do 0-90 degree Phase-added Hilbert Transform
	// *** *EXCEPT* in AM mode - see the function "UiCalcRxPhaseAdj()"
	//    In AM, the FIR below does ONLY low-pass filtering appropriate for the filter bandwidth selected when in AM mode, in
	//	  which case there is ***NO*** audio phase shift applied to the I/Q channels.
	//
	arm_fir_f32((arm_fir_instance_f32 *)&FIR_I,(float32_t *)(ads.i_buffer),(float32_t *)(ads.i_buffer),size/2);	// shift 0 degree FIR
	arm_fir_f32((arm_fir_instance_f32 *)&FIR_Q,(float32_t *)(ads.q_buffer),(float32_t *)(ads.q_buffer),size/2);	// shift +90 degrees FIR (plus RX IQ phase adjustment)
	//
	//	Demodulation, optimized using fast ARM math functions as much as possible
	//
	switch(ts.dmod_mode)	{
		case DEMOD_LSB:
			arm_sub_f32((float32_t *)ads.i_buffer, (float32_t *)ads.q_buffer, (float32_t *)ads.a_buffer, size/2);	// difference of I and Q - LSB
			break;
		case DEMOD_CW:
			if(!ts.cw_lsb)	// is this USB RX mode?  (LSB of mode byte was zero)
				arm_add_f32((float32_t *)ads.i_buffer, (float32_t *)ads.q_buffer, (float32_t *)ads.a_buffer, size/2);	// sum of I and Q - USB
			else	// No, it is LSB RX mode
				arm_sub_f32((float32_t *)ads.i_buffer, (float32_t *)ads.q_buffer, (float32_t *)ads.a_buffer, size/2);	// difference of I and Q - LSB
			break;
		case DEMOD_AM:
			arm_mult_f32((float32_t *)ads.i_buffer, (float32_t *)ads.i_buffer, (float32_t *)ads.a_buffer, size/2);		// square I - store in buffer "a"
			arm_mult_f32((float32_t *)ads.q_buffer, (float32_t *)ads.q_buffer, (float32_t *)ads.b_buffer, size/2);		// square Q - store in buffer "b"
			arm_add_f32((float32_t *)ads.a_buffer, (float32_t *)ads.b_buffer, (float32_t *)ads.a_buffer, size/2);		// sum squares - store in buffer "a"
			for(i = 0; i < size/2; i++)	{										// square root of contents
				arm_sqrt_f32((float32_t)ads.a_buffer[i], (float32_t *)&ads.a_buffer[i]);
			}
			arm_mean_f32((float32_t *)ads.a_buffer, size/2, &ads.am_agc);	// get "average" value of "a" buffer - the recovered DC value - for the AGC
			ads.am_agc *= AM_SCALING;	// rescale AM AGC to match SSB scaling so that AGC comes out the same
			break;
		case DEMOD_USB:
		case DEMOD_DIGI:
		default:
			arm_add_f32((float32_t *)ads.i_buffer, (float32_t *)ads.q_buffer, (float32_t *)ads.a_buffer, size/2);	// sum of I and Q - USB
			break;
	}
	//
	// Do decimation down to lower rate for heavy-duty processing to reduce processor load
	//
	arm_fir_decimate_f32(&DECIMATE_RX, (float32_t *)ads.a_buffer, (float32_t *)ads.a_buffer, size/2);		// LPF built into decimation (Yes, you can decimate-in-place!)
	//
	// DSP Automatic Notch Filter using LMS (Least Mean Squared) algorithm
	//
	if((!ads.af_dissabled) && (ts.dsp_active & 4) && (ts.dmod_mode != DEMOD_CW) && (!ts.dsp_inhibit))	{	// No notch in CW mode
		arm_copy_f32((float32_t *)ads.a_buffer, (float32_t *)&lms2_nr_delay[lms2_inbuf], psize/2);	// put new data into the delay buffer
		//
		arm_lms_norm_f32(&lms2Norm_instance, (float32_t *)ads.a_buffer, (float32_t *)&lms2_nr_delay[lms2_outbuf], (float32_t *)errsig2, (float32_t *)ads.a_buffer, psize/2);	// do automatic notch
		//
		lms2_inbuf += psize/2;				// update circular de-correlation delay buffer
		lms2_outbuf = lms2_inbuf + psize/2;
		lms2_inbuf %= ts.dsp_notch_delaybuf_len;
		lms2_outbuf %= ts.dsp_notch_delaybuf_len;
	}
	//
	// DSP noise reduction using LMS (Least Mean Squared) algorithm
	// This is the pre-filter/AGC instance
	//
	if((ts.dsp_active & 1) && (!(ts.dsp_active & 2)) && (!ads.af_dissabled) && (!ts.dsp_inhibit))	{	// Do this if enabled and "Pre-AGC" DSP NR enabled
		arm_copy_f32((float32_t *)ads.a_buffer, (float32_t *)&lms1_nr_delay[lms1_inbuf], psize/2);	// put new data into the delay buffer
		//
		arm_lms_norm_f32(&lms1Norm_instance, (float32_t *)ads.a_buffer, (float32_t *)&lms1_nr_delay[lms1_outbuf], (float32_t *)ads.a_buffer, (float32_t *)errsig1 ,psize/2);	// do noise reduction
		//
		// Detect if the DSP output has gone to (near) zero output - a sign of it crashing!
		//
		if((((ulong)fabs(ads.a_buffer[0])) * DSP_ZERO_DET_MULT_FACTOR) < DSP_OUTPUT_MINVAL)	{	// is DSP level too low?
			// For some stupid reason we can't just compare above to a small fractional value  (e.g. "x < 0.001") so we must multiply it first!
			if(ads.dsp_zero_count < MAX_DSP_ZERO_COUNT)	{
				ads.dsp_zero_count++;
			}
		}
		else
			ads.dsp_zero_count = 0;
		//
		ads.dsp_nr_sample = ads.a_buffer[0];		// provide a sample of the DSP output for crash detection
		//
		lms1_inbuf += psize/2;	// bump input to the next location in our de-correlation buffer
		lms1_outbuf = lms1_inbuf + psize/2;	// advance output to same distance ahead of input
		lms1_inbuf %= ts.dsp_nr_delaybuf_len;
		lms1_outbuf %= ts.dsp_nr_delaybuf_len;
	}
	//
	// ------------------------
	// Apply audio filter
	if((!ads.af_dissabled)	&& (ts.filter_id != AUDIO_WIDE))	{	// we don't need to filter if running in "wide" mode (Hilbert/FIR does the job!)
		// IIR ARMA-type lattice filter
		arm_iir_lattice_f32(&IIR_PreFilter, (float32_t *)ads.a_buffer, (float32_t *)ads.a_buffer, psize/2);
	}

	//
	// now process the samples and perform the receiver AGC function
	//
	audio_rx_agc_processor(psize);

	//
	// DSP noise reduction using LMS (Least Mean Squared) algorithm
	// This is the post-filter, post-AGC instance
	//
	if((ts.dsp_active & 1) && (ts.dsp_active & 2) && (!ads.af_dissabled) && (!ts.dsp_inhibit))	{	// Do DSP NR if enabled and if post-DSP NR enabled
		arm_copy_f32((float32_t *)ads.a_buffer, (float32_t *)&lms1_nr_delay[lms1_inbuf], psize/2);	// put new data into the delay buffer
		//
		arm_lms_norm_f32(&lms1Norm_instance, (float32_t *)ads.a_buffer, (float32_t *)&lms1_nr_delay[lms1_outbuf], (float32_t *)ads.a_buffer, (float32_t *)errsig1, psize/2);	// Do noise reduction
		//
		// "DC" bias detection to determine if the DSP has crashed
		//
		// Detect if the DSP output has gone to (near) zero output - a sign of it crashing!
		//
		if((((ulong)fabs(ads.a_buffer[0])) * DSP_ZERO_DET_MULT_FACTOR) < DSP_OUTPUT_MINVAL)	{	// is DSP level too low?
			// For some stupid reason we can't just compare it to a small fractional value  (e.g. "x < 0.001") so we must multiply it first!
			if(ads.dsp_zero_count < MAX_DSP_ZERO_COUNT)	{
				ads.dsp_zero_count++;
			}
		}
		else
			ads.dsp_zero_count = 0;
		//
		ads.dsp_nr_sample = ads.a_buffer[0];		// provide a sample of the DSP output for additional crash detection
		//
		lms1_inbuf += psize/2;					// update circular de-correlation delay buffer
		lms1_outbuf = lms1_inbuf + psize/2;
		lms1_inbuf %= ts.dsp_nr_delaybuf_len;
		lms1_outbuf %= ts.dsp_nr_delaybuf_len;
	}
	//
	// Calculate scaling based on decimation rate since this affects the audio gain
	//
	if(ts.filter_id != AUDIO_WIDE)
		post_agc_gain_scaling = POST_AGC_GAIN_SCALING_DECIMATE_4;
	else
		post_agc_gain_scaling = POST_AGC_GAIN_SCALING_DECIMATE_2;
	//
	// Scale audio to according to AGC setting, demodulation mode and required fixed levels
	//
	if(ts.dmod_mode == DEMOD_AM)
		arm_scale_f32((float32_t *)ads.a_buffer,(float32_t)(ads.post_agc_gain * post_agc_gain_scaling * (AM_SCALING * AM_AUDIO_SCALING)), (float32_t *)ads.a_buffer, psize/2);	// apply fixed amount of audio gain 		scaling to make the audio levels correct along with AGC
	else
		arm_scale_f32((float32_t *)ads.a_buffer,(float32_t)(ads.post_agc_gain * post_agc_gain_scaling), (float32_t *)ads.a_buffer, psize/2);	// apply fixed amount of audio gain scaling to make the audio levels 			correct along with AGC
	//
	// resample back to original sample rate while doing low-pass filtering to minimize aliasing effects
	//
	arm_fir_interpolate_f32(&INTERPOLATE_RX, (float32_t *)ads.a_buffer,(float32_t *) ads.b_buffer, psize/2);
	//
	if(ts.rx_muting)	{
		arm_fill_f32(0, (float32_t *)ads.a_buffer, size/2);
		arm_fill_f32(0, (float32_t *)ads.b_buffer, size/2);
	}
	else	{
		arm_scale_f32((float32_t *)ads.b_buffer, (float32_t)LINE_OUT_SCALING_FACTOR, (float32_t *)ads.a_buffer, size/2);		// Do fixed scaling of audio for LINE OUT and copy to "a" buffer in one operation
		//
		// AF gain in "ts.audio_gain-active"
		//  0 - 16: via codec command
		// 17 - 20: soft gain after decoder
		//
		if(ts.audio_gain > 16)	// is volume control above highest hardware setting?
			arm_scale_f32((float32_t *)ads.b_buffer, (float32_t)ts.audio_gain_active, (float32_t *)ads.b_buffer, size/2);	// yes, do software volume control adjust on "b" buffer
	}
	//
	// Transfer processed audio to DMA buffer
	//
	i = 0;			// init sample transfer counter
	while(i < size/2)	{						// transfer to DMA buffer and do conversion to INT - Unrolled to speed it up
		*dst++ = (int16_t)ads.b_buffer[i];		// Speaker channel (variable level)
		*dst++ = (int16_t)ads.a_buffer[i++];		// LINE OUT (constant level)

		*dst++ = (int16_t)ads.b_buffer[i];		// Speaker channel (variable level)
		*dst++ = (int16_t)ads.a_buffer[i++];		// LINE OUT (constant level)

		*dst++ = (int16_t)ads.b_buffer[i];		// Speaker channel (variable level)
		*dst++ = (int16_t)ads.a_buffer[i++];		// LINE OUT (constant level)

		*dst++ = (int16_t)ads.b_buffer[i];		// Speaker channel (variable level)
		*dst++ = (int16_t)ads.a_buffer[i++];		// LINE OUT (constant level)
	}
	//
}

