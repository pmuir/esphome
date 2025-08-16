#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

#define request_wait_period 50 // Time to wait for a measurement in ms
#define read_wait_period 20 // Time to wait for a measurement to be ready in ms

namespace esphome {
namespace dfrobot_sen0590 {

// The various states the component can be in
enum Sen0590SensorState {
    REQUEST, // Request a new measurement
    READY, // Ready to request the measurement value
    READ, // Requesting the measurement value
    IDLE // There is no request in progress
};

/*
 * An ESPHome component for the Laser Ranging Sensor (4m) which has the SKU SEN0590 made by DFRobot.
 * It's based on their Arduino example code which on their wiki
 * https://wiki.dfrobot.com/Laser_Ranging_Sensor_4m_SKU_SEN0590 but replaces the various delays
 * they use with a very basic state machine, and uses millis() to work out when the sensor is ready
 * to avoid blocking the loop.
 *
 *
 * To use it, enable the I2C bus:
 *
 * ```
 * i2c:
 *     scan: true
 *     sda: GPIO32
 *     scl: GPIO33
 * ```
 * ```
 * sensor:
 *   - dfrobot_sen0590:
 *       id: dfrobot_sen0590_1_1
 *       address: 0x74
 * ```
 *
 * The precision on this sensor is dependent on what you are measuring the distance towards (as it
 * depends what the laser can bounce off) so using some filters on the raw value is useful.
 */
class DFRobotSen0590 : public i2c::I2CDevice, public PollingComponent, public sensor::Sensor {
 public:
  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;

 protected:
    unsigned long startRequest = 0UL; // The time the REQUEST state is entered
    unsigned long startRead = 0UL; // The time the READ state is entered
    Sen0590SensorState sensor_state_ = IDLE; // The sensor state machine
};

}  // namespace dfrobot_sen0590
}  // namespace esphome
