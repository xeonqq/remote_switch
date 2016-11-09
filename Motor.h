#ifndef MOTOR_H
#define MOTOR_H 
#include <Arduino.h>

class Motor
{
	public:
		Motor(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm);
		void setup();
		void forward(uint8_t pwm=255);
		void backward(uint8_t pwm=255);
		void stop();
	private:
		uint8_t _pin_pwm;
		uint8_t _pin_in1;
		uint8_t _pin_in2;

};
#endif /* MOTOR_H */
