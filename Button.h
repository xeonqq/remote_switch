#ifndef BUTTON_H
#define BUTTON_H 
#include <stdint.h>
#include <Arduino.h>

#define MAX_NUM_OBSERVER 3
#define VOLTAGE_LEVEL_PRESSED 0
class Button
{
	public: 
		enum Event {
			Pressed = 0,
			Released
		};
		
		Button(uint8_t pin);
		void start_listening();
		bool register_cb(void (*cb) (Event event));
		Event state() {return last_button_state;}
	private:
		uint8_t _pin;
		Event last_button_state;
		uint8_t observer_num;	
		void (*observer_cb[MAX_NUM_OBSERVER]) (Event event);
};
#endif /* BUTTON_H */
