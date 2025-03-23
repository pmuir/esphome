#include "esphome/core/log.h"
#include "tinovi_leaf_sensor.h"
#include <Wire.h>

namespace esphome {
namespace tinovi_leaf_sensor {

static const char *TAG = "tinovi_leaf_sensor.component";

void TinoviLeafSensor::setup() {
  // no-op
}

void TinoviLeafSensor::loop() {
  // The state machine
  ESP_LOGVV("tinovi_leaf_wetness", "STATE: %d", state);
  switch (state) {
    case REQUEST:
      // Tell the sensor to start a measurement
      Wire.beginTransmission(this->address_);
      Wire.write(REG_READ_ST);
      Wire.endTransmission();
      state = WAITING;
      startRequest = millis();
      break;
    case WAITING:
      // Wait for the measurement to be complete
      if (wait_period < millis() - startRequest) {
        state = READY;
      }
      break;
    case READY:
      // Tell the sensor to send the measurement
      Wire.beginTransmission(this->address_);
      Wire.write(REG_DATA);
      Wire.endTransmission();
      Wire.requestFrom(this->address_, 4);
      state = READ;
    case READ:
      // Read the measurement and publish it
      if (Wire.available() == 4) {
        for (int k = 0; k < 2; k++) {
          int16_t ret;
          byte *pointer = (byte *) &ret;
          pointer[0] = Wire.read();
          pointer[1] = Wire.read();
          float value = ret / 100.0;
          switch (k) {
            case 0:
              wetness_sensor->publish_state(value);
              break;
            case 1:
              temperature_sensor->publish_state(value);
              break;
          }
        }
        state = IDLE;
      }
      break;
  }
}

void TinoviLeafSensor::update() {
  // The work is done in loop()
  state = REQUEST;  // Put the sensor into the REQUEST state to start a measurement
}

void TinoviLeafSensor::dump_config() { ESP_LOGCONFIG(TAG, "Tinovi Leaf Sensor"); }

}  // namespace tinovi_leaf_sensor
}  // namespace esphome
