import esphome.codegen as cg
from esphome.components import i2c, sensor
import esphome.config_validation as cv
from esphome.const import (
    CONF_HUMIDITY,
    CONF_ID,
    CONF_TEMPERATURE,
    DEVICE_CLASS_HUMIDITY,
    DEVICE_CLASS_TEMPERATURE,
    STATE_CLASS_MEASUREMENT,
    UNIT_CELSIUS,
    UNIT_PERCENT,
)

DEPENDENCIES = ["i2c"]

dfrobot_sen0590_sensor_ns = cg.esphome_ns.namespace("dfrobot_sen0590_sensor")
DFRobotSen0590 = dfrobot_sen0590_sensor_ns.class_(
    "DFRobotSen0590", cg.PollingComponent
)

CONFIG_SCHEMA =  sensor.sensor_schema(
                unit_of_measurement=UNIT_MILLIMETER,
                accuracy_decimals=1,
                device_class=DEVICE_CLASS_DISTANCE,
                state_class=STATE_CLASS_MEASUREMENT,
            )
    .extend(cv.polling_component_schema("60s"))
    .extend(i2c.i2c_device_schema(0x74))


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    sens = await sensor.new_sensor(config[CONF_DISTANCE])
