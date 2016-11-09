#include "Motor.h"

Motor::Motor(uint8_t pin_in1, uint8_t pin_in2, uint8_t pin_pwm) : _pin_in1(pin_in1), _pin_in2(pin_in2), _pin_pwm(pin_pwm)
{

}

void Motor::setup()
{
	pinMode(_pin_in1, OUTPUT); 
	pinMode(_pin_in2, OUTPUT);
	analogWrite(_pin_pwm, 0);   //0~255
	stop();
}

void Motor::forward(uint8_t pwm)
{
	
	digitalWrite(_pin_in1,HIGH);	
	digitalWrite(_pin_in2,LOW);	
	analogWrite(_pin_pwm, pwm);
}


void Motor::backward(uint8_t pwm)
{
	digitalWrite(_pin_in2,HIGH);	
	digitalWrite(_pin_in1,LOW);	
	analogWrite(_pin_pwm, pwm);
}



void Motor::stop()
{
	digitalWrite(_pin_in2,LOW);	
	digitalWrite(_pin_in1,LOW);	
	analogWrite(_pin_pwm, 0);
}
