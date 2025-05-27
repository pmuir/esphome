#include "esphome/core/log.h"
#include "dfrobot_sen0590.h"
#include <Wire.h>

namespace esphome {
namespace dfrobot_sen0590 {

static const char *TAG = "dfrobot_sen0590.component";

void DFRobotSen0590Component::setup() {
  // no-op
}

void DFRobotSen0590Component::loop() {
  // The state machine
  ESP_LOGVV("dfrobot_sen0590_wetness", "STATE: %d", state);
  switch(state) {
    // Request a measurement is made
    case REQUEST:
      Wire.beginTransmission(address);
      Wire.write(0x10);
      Wire.write(0xB0);
      Wire.endTransmission();
      state = READY;
      startRequest = millis();
      break;
    case READY:
      // Wait for the measurement to be ready
      if (request_wait_period > millis() - startRequest) {
        break;
      }
      // Tell the sensor to send the measurement
      Wire.beginTransmission(address);
      Wire.write(0x02);
      if (Wire.endTransmission() != 0) {
        return;
      }
      state = READ;
      startRead = millis();
      break;
    case READ:
      // Wait for the measurement to be ready to read
      if (read_wait_period > millis() - startRead) {
        break;
      }
      // Read the measurement and publish it
      Wire.requestFrom(address, 2);
      int buf[2] = { 0 };
      for (int i = 0; i < 2; i++) {
        buf[i] = Wire.read();
      }
      int distance = (buf[0] * 0x100 + buf[1] + 10);
      publish_state(distance);
      state = IDLE;
      break;
  }
}

void DFRobotSen0590Component::update() {
  // The work is done in loop()
  state = REQUEST;  // Put the sensor into the REQUEST state to start a measurement
}

void DFRobotSen0590Component::dump_config() { ESP_LOGCONFIG(TAG, "DFRobot Sen0590"); }

}  // namespace dfrobot_sen0590
}  // namespace esphome
