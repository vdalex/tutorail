/*
 * LoRa.h
 *
 *  Created on: Jun 7, 2018
 *      Author: oleksandr
 */
#include "stdint.h"

#ifndef LORA_H_
#define LORA_H_

#endif /* LORA_H_ */


#define BW 125000.0 // Bandwidth in Hz
#define Fs 1000000.0   // Sampling Frequency in Hz
#define FREQ_OFFCET (Fs/2 - BW/2)
#define SF	7 //Spread factor
#define SAMPLE_NUMBER	(uint16_t)(Fs*(2^SF)/BW) // Number of samples. Fs/BW must be integer value!!! 1e6 / 125e3 = 8; SAMPLE_NUMBER = 8*2^7=8*128 = 1024
#define PI 3.1415926

void LoRa_Modulation(uint8_t symbol, uint8_t inverse);
