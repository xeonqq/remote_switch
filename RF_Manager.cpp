#include "RF_Manager.h"
const uint8_t RF_Manager::addresses[][6] = {"1Node","2Node"};

RF_Manager::RF_Manager(RF24& radio, uint8_t radio_num): _radio(radio), _radio_num(radio_num)
{
	sent_cmd = TURN_STOP;

	recieved_actuator_cmd = TURN_STOP;
}

void RF_Manager::send_actuator_command(ACTUATOR_CMD cmd)
{
	_radio.stopListening();
	if (!_radio.write( &cmd, sizeof(ACTUATOR_CMD) )){
		Serial.println(F("failed"));
	}
	sent_cmd = cmd;
	sent_time = micros();
	_radio.startListening();
	rfUserState = WaitAcK;
	Serial.print(F("Send Cmd "));
	Serial.println(cmd);
}

void RF_Manager::send_ack(ACTUATOR_CMD cmd)
{
	_radio.stopListening();                                        // First, stop listening so we can talk   
	_radio.write(&cmd, sizeof(ACTUATOR_CMD));
	_radio.startListening();                                       // Now, resume listening so we catch the next packets.     
	Serial.println(F("Ack "));
	Serial.println(cmd);
}

void RF_Manager::update_state()
{
	if (_radio_num == USER)
		update_user_state();
	if (_radio_num == ACTUATOR)
		update_actuator_state();
}

void RF_Manager::update_user_state()
{
	ACTUATOR_CMD got_cmd;
	while (_radio.available()) {                                   // While there is data ready
		_radio.read( &got_cmd, sizeof(ACTUATOR_CMD));             // Get the payload
	}
	if (got_cmd == sent_cmd){
		rfUserState = Success;
		Serial.println(F("Cmd Success"));
	}

	switch(rfUserState) {
		case WaitAcK:
			{
				if (micros() - sent_time > 200000)  // If waited longer than 200ms, indicate timeout 
					rfUserState= TimeOut;
			}
			break;
		case TimeOut:
			//todo: limit re-try times
			send_actuator_command(sent_cmd);
			Serial.println(F("Timeout"));
			break;
		case Success:
			break;
		default:
			break;
	}

}

void RF_Manager::update_actuator_state()
{
	if( _radio.available()){

		while (_radio.available()) {                                   // While there is data ready
			_radio.read( &recieved_actuator_cmd, sizeof(ACTUATOR_CMD));             // Get the payload
		}
		send_ack(recieved_actuator_cmd);
	}
}
