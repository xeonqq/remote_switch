#ifndef RF_MANAGER_H
#define RF_MANAGER_H 

#include <stdint.h>
#include <SPI.h>
#include "RF24.h"
#include "AutoSwitch.h"
#include <Arduino.h>

class RF_Manager
{
	public:
		enum RF_User_State {
			WaitAcK = 0,
			TimeOut,
			Success,
			Max_User_State
		};

		RF_Manager(RF24& radio, uint8_t radio_num);
		uint8_t radio_number() const {return _radio_num;}
		void update_user_state();
		void update_actuator_state();
		void update_state();
		ACTUATOR_CMD recved_actuator_cmd() const {return recieved_actuator_cmd;}
		static const byte addresses[][6];
		void send_actuator_command(ACTUATOR_CMD cmd);
		void send_ack(ACTUATOR_CMD cmd);
	private:
		RF24& _radio; 
		uint8_t _radio_num;
		ACTUATOR_CMD sent_cmd;
		uint32_t sent_time;
		RF_User_State rfUserState;

		ACTUATOR_CMD recieved_actuator_cmd;
		
};

#endif /* RF_MANAGER_H */
