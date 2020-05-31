/* Stuff in this file requires alternative firmware */
#include "esphome.h"
#include "esphome/log.h"

using namespace esphome;

class LCButtons: public UARTDevice, public PollingComponent {
 public:
  LCButtons(UARTComponent *parent, uint32_t update_interval = 300): 
    UARTDevice(parent), PollingComponent(update_interval)
  {

  }

  binary_sensor::BinarySensor buttons[2];

  void setup() override {
    setTimeout(150);
  }

  const char *TAG = "LCC";
  void update_button(uint8_t id) 
  {
    LCRelay::send_packet(this, id + 0x21, 0);
    while (!available());;
    char tmp[8]; 
    int button;
    int value;
    int ret = readBytesUntil('\n', tmp, 8);
    sscanf(tmp, "S%d:%d", &button, &value);
    if (button > 2 || button < 1)
        return;
    buttons[button - 1].publish_state(!value);
  }

  void update() override {
      update_button(0);
      update_button(1);
  }
};


class LCFloatOutput : public Component, public UARTDevice, public output::FloatOutput {
 public:
  char pre;
  static bool running;
  
  #define DIV_ROUND_DOWN(n, d)  (((n) / (d)) * (d))
  LCFloatOutput(UARTComponent *parent, const char prefix)
    :UARTDevice(parent)
  {
      pre = prefix;
  }
  void setup() override {
  }

  void write_state(float state) override {
    unsigned int v = (state * 255.0);
    LCRelay::send_packet(this, pre, v);
  }
};


class LCFanOutput : public Component, public UARTDevice, public output::FloatOutput {
 public:
  char fanlo, fanhi;
  static bool running;
  
  #define DIV_ROUND_DOWN(n, d)  (((n) / (d)) * (d))
  LCFanOutput(UARTComponent *parent, const char lo, const char hi)
    :UARTDevice(parent)
  {
    fanlo = lo; 
    fanhi = hi;
  }

  void setup() override {
  }

  void write_state(float state) override {
    if (state == 0.0) {
        LCRelay::send_packet(this, fanlo, 1);
        LCRelay::send_packet(this, fanhi, 1);
    } else if (state <= 0.34) {
        LCRelay::send_packet(this, fanlo, 0);
        LCRelay::send_packet(this, fanhi, 1);
    } else if (state <= 0.67) {
        LCRelay::send_packet(this, fanlo, 1);
        LCRelay::send_packet(this, fanhi, 0);
    } else {
        LCRelay::send_packet(this, fanlo, 0);
        LCRelay::send_packet(this, fanhi, 0);
    }
  }
};