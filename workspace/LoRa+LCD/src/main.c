/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/

#include "stm32f7xx.h"
#include "stm32746g_discovery.h"
#include "stm32746g_discovery_lcd.h"
#include "arm_const_structs.h"
#include "fonts.h"
#include "LoRa.h"
			
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);

extern float out_preamble_I[];
extern float out_preamble_Q[];

float tmp_buf_I[1024];
float tmp_buf_Q[1024];

float out_buf_I[1024];
float out_buf_Q[1024];


#define LCD_FRAME_BUFFER	SDRAM_DEVICE_ADDR
#define RGB565_BYTE_PER_PIXEL	2
#define	ARBG_BYTE_PER_PIXEL 4


int main(void)
{

  uint16_t x_start,x_end,y_start,y_end;
  uint16_t i;

  MPU_Config();
  CPU_CACHE_Enable();
  HAL_Init();
  SystemClock_Config();

  BSP_LCD_Init();
  BSP_LCD_LayerDefaultInit(LTDC_ACTIVE_LAYER, LCD_FRAME_BUFFER);
  BSP_LCD_SetLayerVisible(LTDC_ACTIVE_LAYER, ENABLE);
  BSP_LCD_SelectLayer(LTDC_ACTIVE_LAYER);
  BSP_LCD_Clear(LCD_COLOR_BLACK);
  BSP_LCD_SetBackColor(LCD_COLOR_BLACK);
  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
  BSP_LCD_DisplayOn();

  LoRa_Modulation(100,0); //prepare LoRa symbol 0
  //copy symbol in temporary buffer

  for (i=0; i<1024; i++)
  {
	  tmp_buf_I[i] = out_preamble_I[i];
	  tmp_buf_Q[i] = out_preamble_Q[i];
  }

  LoRa_Modulation(0,1); //prepare LoRa reverse chirp
  //do multiplication end select lower spectrum part
  //de_chirped = x.*conj(upChirp);
  for (i=0; i<1024; i++)
  {
//  	  out_preamble_I[i] = -1.0*out_preamble_I[i];;
//  	  out_preamble_Q[i] = -1.0*out_preamble_Q[i];
  }


  //complex multiply symbol and inverse chirp
  for (i=0; i<1024; i++)
  {
	  out_buf_I[i] = tmp_buf_I[i]*out_preamble_I[i];
	  out_buf_Q[i] = tmp_buf_Q[i]*out_preamble_Q[i];
//	  out_buf_I[i] = tmp_buf_I[i];
//	  out_buf_Q[i] = tmp_buf_Q[i];
  }

  //display LoRa symbol
//  BSP_LCD_DrawPixel(0, 136+(int16_t)(136.0*out_preamble_I[0]) , 0xff00ff00);
//  BSP_LCD_DrawPixel(0, 136+(int16_t)(136.0*out_preamble_Q[0]) , 0xffff0000);

  y_start=136+(int16_t)(136.0*out_buf_I[1023]);
  x_start=0;

  for (i=0; i<480; i++)
  {
	  x_end=i;
	  y_end=136+(int16_t)(136.0*out_buf_I[1023-i]);

	  BSP_LCD_DrawLine(x_start, y_start, x_end, y_end);

	  x_start=x_end;
	  y_start=y_end;

  }

  //display LoRa symbol in reverse manner
  //  BSP_LCD_DrawPixel(0, 136+(int16_t)(136.0*out_preamble_I[0]) , 0xff00ff00);
  //  BSP_LCD_DrawPixel(0, 136+(int16_t)(136.0*out_preamble_Q[0]) , 0xffff0000);

    y_start=136+(int16_t)(136.0*out_buf_Q[1023]);
    x_start=0;


    BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);

    for (i=0; i<480; i++)
    {
  	  x_end=i;
  	  y_end=136+(int16_t)(136.0*out_buf_Q[1023-i]);

  	  BSP_LCD_DrawLine(x_start, y_start, x_end, y_end);

  	  x_start=x_end;
  	  y_start=y_end;

    }

//add FFT and IFFT functions and compile
//problem can exist during bitrevers procedure
//
//
//

//  arm_status status;
//  status = ARM_MATH_SUCCESS;

//  arm_cfft_f32(&arm_cfft_sR_f32_len1024,out_buf_I,1,0); //do IFFT
//  arm_cfft_f32(&arm_cfft_sR_f32_len1024,out_buf_I,0,0); //do FFT

//add window function






  while (1)
  {
//	  char buffer[30];
//	  sprintf(buffer, "HELLO WORLD!!!");
//	  BSP_LCD_DisplayStringAtLine(1, (uint8_t *)buffer);
//	  BSP_LCD_DrawVLine(0, 0, 250);
//	  BSP_LCD_DrawVLine(10, 0, 250);
//	  BSP_LCD_DrawVLine(20, 0, 250);
//	  BSP_LCD_DrawVLine(30, 0, 250);

//	  BSP_LCD_DrawVLine();
//	  BSP_LCD_DrawHLine();
//	  BSP_LCD_SetTextColor(LCD_COLOR_YELLOW);
//	  BSP_LCD_FillRect(50, 50, 100, 100);
//	  BSP_LCD_SetTextColor(LCD_COLOR_WHITE);
//	  BSP_LCD_DrawPixel(200, 200, 0xff00ff00);

  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void)
{
  MPU_Region_InitTypeDef MPU_InitStruct;

  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void)
{
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

