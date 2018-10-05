#include "main.h"/ *Private variables ---------------------------------------------------------*/

static Button_press_type button_key_event = BUTTON_NONE_PRESS;

GPIO_InitTypeDef  buttonInitStruct;

void  button_init(void)
{
	BUTTON_GPIO_CLK_ENABLE();  
	buttonInitStruct.Pin = BUTTON_PIN;
	buttonInitStruct.Mode  = GPIO_MODE_INPUT;
	buttonInitStruct.Pull  = GPIO_PULLDOWN;
	buttonInitStruct.Speed = GPIO_SPEED_FREQ_HIGH;  
	HAL_GPIO_Init(BUTTON_GPIO_PORT, &buttonInitStruct);
}

Button_press_type button_get_key_event(void)
{
	Button_press_type e = button_key_event;
	button_key_event = BUTTON_NONE_PRESS;
	return e;
}

uint8_t button_get_state(void) // button state event generation
{
	static uint8_t button_state_debounced = 0;
	static uint16_t button_debounce_cntr = 0;
	static uint8_t state = 0;  
	uint8_t button_state = BUTTON_GPIO_READ_STATE(); // true corresponds to btn pressed  // Debounce logic

	if (button_state_debounced != button_state)
	{
		button_state_debounced = button_state;
		button_debounce_cntr = DEBOUNCE_DELAY;
	}  

	if (button_debounce_cntr > 0) button_debounce_cntr--;
	else state = button_state_debounced;  
	return state;
}

void button_tick(void) // generation btn press type events
{
	static uint32_t button_press_cntr = 0;
	static uint8_t button_state = 0;
	static uint8_t button_state_prev = 0;
	static _Bool longest_press_lock = 0;  

	button_state = button_get_state();  // event generation
	Button_Event event;
 
	if (button_state && !button_state_prev) event = BUTTON_EVENT_PRESSED;
	else if (!button_state && button_state_prev) event = BUTTON_EVENT_RELEASED;
	
	button_state_prev = button_state;  // press type generation

	if (button_press_cntr >= BUTTON_LONGEST_PRESS_TIME)
	{
		if (!longest_press_lock)
		{
			button_key_event = BUTTON_LONGEST_PRESS;
			longest_press_lock  = 1;
		}
	}
	else if (event == BUTTON_EVENT_RELEASED)
	{
		if (button_press_cntr >= BUTTON_LONG_PRESS_TIME) button_key_event = BUTTON_LONG_PRESS;
		else button_key_event = BUTTON_SHORT_PRESS;
	}  

	if (event == BUTTON_EVENT_PRESSED) button_press_cntr++;
	else
	{
		longest_press_lock = 0;
		button_press_cntr = 0;
	}
}

