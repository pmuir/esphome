#include "tinovi_leaf_sensor.h"

namespace esphome {
namespace tinovi_leaf_sensor {

void EmptyI2CComponent::setup() {}

void EmptyI2CComponent::loop() {}

void EmptyI2CComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "Empty I2C component");
}

} // namespace empty_i2c_component
} // namespace esphome