#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"
#include "esphome/components/sensor/sensor.h"

#define wait_period \
  300  // the time in ms to wait to read the data after requesting a new reading
       // - this is stated by the docs as 100ms, but in the code it's either
       // 300ms or 400ms. 300ms seems to work.

// Defines copied from Arduino Library

#define REG_READ_ST 0x01
#define REG_TEMP 0x04
#define REG_WET 0x05

#define REG_AIR 0x06
#define REG_WATER 0x07

#define REG_CAP 0x0A
#define REG_RES 0x0B
#define REG_RT 0x0D

#define REG_ADDR 0x08
#define REG_DATA 0x09

namespace esphome {
namespace tinovi_leaf_sensor {

// The various states the component can be in
enum TinvoiLeafSensorState {
  REQUEST,  // Request a new measurement
  WAITING,  // Waiting for the measurement
  READY,    // Ready to request the measurement value
  READ,     // Requesting the measurement value
  IDLE      // There is no request in progress
};

/*
 * An ESPHome component for the I2C leaf sensor made by Tinovi.
 * It's based on their Arduino example code which is in LeadArduioI2C but
 * replaces the various delays they use with a very basic state machine, and
 * uses millis() to work out when the sensor is ready to avoid blocking the
 * loop.
 *
 * It publishes both the temperature reading in (degrees celsius) and the
 * wetness reading (%).
 *
 * To use it, enable the I2C bus:
 *
 * ```
 * i2c:
 *     scan: true
 *     sda: GPIO32
 *     scl: GPIO33
 * ```
 *
 * Add the includes under `esphome:`
 *
 * ```
 * includes:
 *   - custom_components/tinovi-leaf-sensor/tinovi_leaf_wetness.h
 *   - custom_components/tinovi-leaf-sensor/LeafArduinoI2c/LeafSens.h
 * ```
 *
 * and the custom component:
 *
 * ```
 * sensor:
 *   - platform: custom
 *     lambda: |-
 *       auto sensor = new LeafWetness(5000);
 *       App.register_component(sensor);
 *       return {sensor->temperature_sensor, sensor->wetness_sensor};
 *
 *     sensors:
 *       - name: "Temperature Sensor"
 *         unit_of_measurement: °C
 *         accuracy_decimals: 1
 *       - name: "Wetness Sensor"
 *         unit_of_measurement: "%"
 *         accuracy_decimals: 1
 */
class TinoviLeafSensorComponent : public i2c::I2CDevice, public PollingComponent, sensor::Sensor {
 public:
  void set_temperature_sensor(sensor::Sensor *temperature_sensor) { temperature_sensor_ = temperature_sensor; }
  void set_humidity_sensor(sensor::Sensor *humidity_sensor) { humidity_sensor_ = humidity_sensor; }
  void setup() override;
  void loop() override;
  void update() override;
  void dump_config() override;

 protected:
  unsigned long startRequest = 0UL;    // The time the REQUEST state is entered
  TinvoiLeafSensorState state = IDLE;  // The sensor state machine
  Sensor *temperature_sensor_;  // The ESPHome temperature sensor
  Sensor *humidity_sensor_;      // The ESPHome wetness sensor
};

}  // namespace tinovi_leaf_sensor
}  // namespace esphome
