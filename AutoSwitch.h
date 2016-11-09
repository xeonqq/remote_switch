#ifndef AUTOSWITCH_H
#define AUTOSWITCH_H 

#define USER 0 
#define ACTUATOR 1


/****************** User Config ***************************/
#define WHOIAM USER

enum ACTUATOR_CMD 
{
	TURN_FORWARD = 0xD1,
	TURN_BACKWARD = 0xE2,
	TURN_STOP = 0xF3
};
void button_task();
void rf_task();
void actuator_task();
#endif /* AUTOSWITCH_H */
