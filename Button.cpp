#include "Button.h"

Button::Button(uint8_t pin) : _pin(pin), last_button_state(Released), observer_num(0)
{
	pinMode(_pin, INPUT); 
}



void Button::start_listening() 
{
	uint8_t button_in = digitalRead(_pin);
	if (button_in == VOLTAGE_LEVEL_PRESSED) {
		if (last_button_state == Released)
		{
			//button press triggered
			for (uint8_t i=0;i<observer_num;++i) {
				observer_cb[i](Pressed);
			}
		}
		else if (last_button_state == Pressed)
		{
			//still pressed
		}
		last_button_state = Pressed;
	}
	else {
		if (last_button_state == Pressed) {
	       		//button released triggered
			for (uint8_t i=0;i<observer_num;++i) {
				observer_cb[i](Released);
			}
			
		}
		else if (last_button_state == Pressed)
		{
			//still released
		}
		last_button_state = Released;
	}


}

bool Button::register_cb(void (*cb) (Event event))
{
	if (observer_num < MAX_NUM_OBSERVER)
		observer_cb[observer_num++] = cb;
	else
		return false; //exceeding MAX_NUM_OBSERVER
	return true;
}
