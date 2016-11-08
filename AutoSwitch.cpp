
#include "Button.h"
#include <TaskScheduler.h>
#include "AutoSwitch.h"
#include <SPI.h>
#include "RF24.h"
#include "RF_Manager.h"

Button button = Button(4);
Scheduler runner;
/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);
RF_Manager rf_manager(radio, WHOIAM);

Task t1_button(10, TASK_FOREVER, &button_task);
Task t2_rf(20, TASK_FOREVER, &rf_task);

void on_button_callback(Button::Event e) {
	if (rf_manager.radio_number() == ACTUATOR)
		return;
	if (e == Button::Pressed)
	{
		Serial.println(F("Pressed"));
		rf_manager.send_actuator_command(TURN_FORWARD);
	}
	else if (e == Button::Released)
	{
		Serial.println(F("Released"));
		rf_manager.send_actuator_command(TURN_STOP);
	}
}


void button_task()
{
	button.start_listening();
}

void rf_task()
{
	rf_manager.update_state();
}

void setup() {
	Serial.begin(115200);
	Serial.println(F("Test"));

	radio.begin();

	// Set the PA Level low to prevent power supply related issues since this is a
	// getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
	radio.setPALevel(RF24_PA_LOW);

	// Open a writing and reading pipe on each radio, with opposite addresses
	if(rf_manager.radio_number() == USER){
		radio.openWritingPipe(rf_manager.addresses[1]);
		radio.openReadingPipe(1,rf_manager.addresses[0]);
		Serial.print(F("USER"));
	}else{
		radio.openWritingPipe(rf_manager.addresses[0]);
		radio.openReadingPipe(1,rf_manager.addresses[1]);
		Serial.print(F("ACTUATOR"));
	}

	// Start the radio listening for data
	radio.startListening();

	button.register_cb(on_button_callback);
	runner.addTask(t1_button);
	t1_button.enable();
	runner.addTask(t2_rf);
	t2_rf.enable();
}

void user_task()
{
	if (rf_manager.radio_number() == USER)
		return;
}

void actuator_task()
{
	if (rf_manager.radio_number() == USER)
		return;
	
}

void loop() {
	runner.execute();
}
#if 0



/**********************************************************/

void loop() {
  
  
/****************** Ping Out Role ***************************/  
if (role == 1)  {
    
    radio.stopListening();                                    // First, stop listening so we can talk.
    
    
    Serial.println(F("Now sending"));

    unsigned long start_time = micros();                             // Take the time, and send it.  This will block until complete
     if (!radio.write( &start_time, sizeof(unsigned long) )){
       Serial.println(F("failed"));
     }
        
    radio.startListening();                                    // Now, continue listening
    
    unsigned long started_waiting_at = micros();               // Set up a timeout period, get the current microseconds
    boolean timeout = false;                                   // Set up a variable to indicate if a response was received or not
    
    while ( ! radio.available() ){                             // While nothing is received
      if (micros() - started_waiting_at > 200000 ){            // If waited longer than 200ms, indicate timeout and exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                             // Describe the results
        Serial.println(F("Failed, response timed out."));
    }else{
        unsigned long got_time;                                 // Grab the response, compare, and send to debugging spew
        radio.read( &got_time, sizeof(unsigned long) );
        unsigned long end_time = micros();
        
        // Spew it
        Serial.print(F("Sent "));
        Serial.print(start_time);
        Serial.print(F(", Got response "));
        Serial.print(got_time);
        Serial.print(F(", Round-trip delay "));
        Serial.print(end_time-start_time);
        Serial.println(F(" microseconds"));
    }

    // Try again 1s later
    delay(1000);
  }



/****************** Pong Back Role ***************************/

  if ( role == 0 )
  {
    unsigned long got_time;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
      }
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Now, resume listening so we catch the next packets.     
      Serial.print(F("Sent response "));
      Serial.println(got_time);  
   }
 }




/****************** Change Roles via Serial Commands ***************************/

  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    
   }else
    if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
       
    }
  }


} // Loop
#endif
