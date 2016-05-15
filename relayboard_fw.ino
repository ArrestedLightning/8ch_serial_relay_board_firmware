#include <Metro.h>
#include <avr/wdt.h>
#define RUN_LED 5
#define RELAY_1 A0
#define RELAY_2 A1
#define RELAY_3 A2
#define RELAY_4 A3
#define RELAY_5 A4
#define RELAY_6 2
#define RELAY_7 3
#define RELAY_8 4
#define HEARTBEAT_MS 500

int8_t data_position=-1;
//no doubt there's a more elegant way to accomplish the same thing

int8_t relay_num_1, relay_num_2, relay_num_3;
int8_t relay_state_1, relay_state_2, relay_state_3;

Metro heartbeat = Metro(HEARTBEAT_MS);
int8_t ledState = HIGH;

void setup() {
  wdt_disable();//disable the watchdog initially
  //Relays are active low
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);
  digitalWrite(RELAY_5, HIGH);
  digitalWrite(RELAY_6, HIGH);
  digitalWrite(RELAY_7, HIGH);
  digitalWrite(RELAY_8, HIGH);
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  pinMode(RELAY_5, OUTPUT);
  pinMode(RELAY_6, OUTPUT);
  pinMode(RELAY_7, OUTPUT);
  pinMode(RELAY_8, OUTPUT);
  pinMode(RUN_LED, OUTPUT);


  Serial.begin(19200);//Clock rate is wrong, so actual external connect rate is 9600
  wdt_enable(WDTO_250MS); //enable the watchdog with a 250MS (actually 500MS) timeout
}

void loop() {
  if (Serial.available() > 0){
    //use the RUN LED as a serial comms indicator
    if (ledState == HIGH) {
      ledState = LOW; 
    } 
    else {
      ledState = HIGH;
    }
    digitalWrite(RUN_LED, ledState);
    parse_packet(Serial.read());

  }
  //toggle the RUN LED as a heartbeat
  if (heartbeat.check() == 1) {
    if (ledState == HIGH) {
      ledState = LOW; 
    } 
    else {
      ledState = HIGH;
    }
    heartbeat.interval(HEARTBEAT_MS);
    digitalWrite(RUN_LED, ledState);
  }
  wdt_reset();
}

/**
 * Packet format is:
 * '<', relay number, relay number, relay number, \
 * relay state, relay state, relay state, '>'
 * Relay number is 1-8
 * Relay state is 0 or 1
 * Both are repeated 3x as a simple form of error detection
 * If they don't match, nothing is changed.
 */
void parse_packet(int data){
  switch (data_position){
  case -1: //waiting for start
    if (data == '<'){ //check for proper packet start
      data_position = 0;//go to first byte state
    }
    break;
  case 0:
    relay_num_1 = data;
    data_position = 1;
    break;
  case 1:
    relay_num_2 = data;
    data_position = 2;
    break;
  case 2:
    relay_num_3 = data;
    data_position = 3;
    break;
  case 3:
    relay_state_1 = data;
    data_position = 4;
    break;
  case 4:
    relay_state_2 = data;
    data_position = 5;
    break;
  case 5:
    relay_state_3 = data;
    data_position = 6;
    break;
  case 6:
    //check for proper packet ending
    if (data == '>'){
      //check data
      if ((relay_num_1 == relay_num_2) && (relay_num_2 == relay_num_3)){
        if ((relay_state_1 == relay_state_2) && (relay_state_2 == relay_state_3)) {
          switch_relay(relay_num_1, relay_state_1);
          Serial.write(1); //success status
          data_position = -1;//get ready to receive next packet
          return;
        }
      }
    }
    Serial.write(0); //failure status
    data_position = -1;
    break;
  default:
    //how did we get here?
    data_position = -1;
    break;
  }
}

void switch_relay(byte relay_num, byte state){
  if (state != 1) { //no other value can turn on a relay
    state = HIGH; //off
  } 
  else{
    state = LOW; //on [active low]
  }
  byte the_relay = 0;
  switch (relay_num){
  case 1:
    the_relay = RELAY_1;
    break;
  case 2:
    the_relay = RELAY_2;
    break;
  case 3:
    the_relay = RELAY_3;
    break;
  case 4:
    the_relay = RELAY_4;
    break;
  case 5:
    the_relay = RELAY_5;
    break;
  case 6:
    the_relay = RELAY_6;
    break;
  case 7:
    the_relay = RELAY_7;
    break;
  case 8:
    the_relay = RELAY_8;
    break;
  default:
    // invalid relay number
    return;
    break;
  }
  digitalWrite(the_relay, state);
}









