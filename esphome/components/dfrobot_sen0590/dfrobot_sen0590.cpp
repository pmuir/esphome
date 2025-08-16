#include "esphome/core/log.h"
#include "dfrobot_sen0590.h"
#include "esp_timer.h"

namespace esphome {
namespace dfrobot_sen0590 {

static const char *TAG = "dfrobot_sen0590.sensor";

void DFRobotSen0590::setup() {
  ESP_LOGCONFIG(TAG, "Setting up DFRobot SEN0590...");
}

void DFRobotSen0590::loop() {
  // The state machine
  ESP_LOGVV(TAG, "STATE: %d", sensor_state_);
  switch(sensor_state_) {
    // Request a measurement is made
    case REQUEST: {
      uint8_t data[2] = {0x10, 0xB0};
      this->write(data, 2);
      sensor_state_ = READY;
      startRequest = this->millis();
      break;
    }
    case READY:
      // Wait for the measurement to be ready
      if (request_wait_period > this->millis() - startRequest) {
        break;
      }
      // Tell the sensor to send the measurement
      {
        uint8_t cmd = 0x02;
        if (this->write(&cmd, 1) != i2c::ERROR_OK) {
          ESP_LOGW(TAG, "Failed to request measurement");
          sensor_state_ = IDLE;
          return;
        }
      }
      sensor_state_ = READ;
      startRead = this->millis();
      break;
    case READ:
      // Wait for the measurement to be ready to read
      if (read_wait_period > this->millis() - startRead) {
        break;
      }
      // Read the measurement and publish it
      {
        uint8_t buf[2] = {0};
        if (this->read(buf, 2) != i2c::ERROR_OK) {
          ESP_LOGW(TAG, "Failed to read measurement");
          sensor_state_ = IDLE;
          return;
        }
        uint16_t distance = (buf[0] << 8) | buf[1];
        distance += 10; // Offset correction as per original code
        ESP_LOGD(TAG, "Distance: %u mm", distance);
        this->publish_state(distance);
      }
      sensor_state_ = IDLE;
      break;
    case IDLE:
      // Do nothing, waiting for next update cycle
      break;
  }
}

void DFRobotSen0590::update() {
  // The work is done in loop()
  if (sensor_state_ == IDLE) {
    sensor_state_ = REQUEST;  // Put the sensor into the REQUEST state to start a measurement
  }
}

void DFRobotSen0590::dump_config() { 
  ESP_LOGCONFIG(TAG, "DFRobot SEN0590:");
  LOG_I2C_DEVICE(this);
  LOG_UPDATE_INTERVAL(this);
  LOG_SENSOR("  ", "Distance", this);
}

uint32_t DFRobotSen0590::millis() {
  return (uint32_t) (esp_timer_get_time() / 1000ULL);
}

}  // namespace dfrobot_sen0590
}  // namespace esphome
