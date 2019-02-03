/*
 * LoRa.c
 *
 *  Created on: Jun 7, 2018
 *      Author: oleksandr
 *      Based on MATLAB from http://www.sghoslya.com/p/in-previous-post-you-read-theory-about.html
 */

#include "LoRa.h"
#include "math.h"
#include "arm_math.h"


float32_t out_preamble_I[SAMPLE_NUMBER];
float32_t out_preamble_Q[SAMPLE_NUMBER];

//LoRa_Modulation(0,0); //build main CHIRP

/*
* Function builds LoRa symbol
* inverse = 1 for inverse chirps, inverse = 0 for normal chirps
* symbol - symbol number
* ToDo: Replase cos and sin functions with CORDIC
*
*/


void LoRa_Modulation(uint8_t symbol,uint8_t inverse)
{
    uint16_t k = 0;
    float32_t phase = 0;
    float32_t f = 0;
    float32_t shift = symbol;

    for (k=0; k<SAMPLE_NUMBER; k++)
    {
        //output the complex signal
        out_preamble_I[k] = arm_cos_f32(phase);
        out_preamble_Q[k] = arm_sin_f32(phase);

        f = BW*shift/((float32_t)(2^SF)); //Frequency from cyclic shift

        if (inverse == 1)
        {
        	f = BW - f;
        }

        f = f + FREQ_OFFCET; //apply Frequency offset away from DC

        phase = phase + 2*PI*f/Fs; //Increase the phase according to frequency

        if (phase > PI)
        {
            phase = phase - 2*PI;
        }


        shift = shift + BW/Fs; //update cyclic shift
        if (shift >= (float32_t)(2^SF))
        {
            shift = shift - (float32_t)(2^SF);
        }
    }
}


