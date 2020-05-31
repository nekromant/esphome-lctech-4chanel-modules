#include "esphome.h"
using namespace esphome;

/* 
 The protocol according to the docs:

 Open relay  1:A0 01 01 A2
 Close relay 1:A0 01 00 A1
 Open relay  2:A0 02 01 A3
 Close relay 2:A0 02 00 A2
 Open relay  3:A0 03 01 A4
 Close relay 3:A0 03 00 A3
 Open relay  4:A0 04 01 A5
 Close relay 4:A0 04 00 A4 

 - First  byte, always A0 start of the packet packet
 - Second byte, relay number 1-4
 - Third  byte, state 0/1 
 - Forth  byte, the sum of three previous bytes

  Alternative firmware also add RGB float outputs and button states

*/

class LCRelay : public Component, public UARTDevice, public switch_::Switch {

 public:
    int ctlId;
    LCRelay(UARTComponent *parent, int control_id): UARTDevice(parent) {
        ctlId = control_id;
    }

  void setup() override {
    /* Make sure we're synchronized initially and all relays are off */
    delay(300);
    int j; 
    for (j=0; j<3; j++) {
        this->write_state(false);
    }
  }

  static void send_packet(UARTDevice *dev, uint8_t id, uint8_t state)
  {
    uint8_t msg[4];
    msg[0] = 0xA0;
    msg[1] = id;
    msg[2] = state;
    msg[3] = msg[0] + msg[1] + msg[2];
    /* There are no ack/nack packets. 
       Let's send the payload twice to be sure */
    dev->write_array(msg, sizeof(msg));
  }

  void write_state(bool state) override {
    this->send_packet(this, ctlId, !state);
    this->publish_state(state);    
  }
};