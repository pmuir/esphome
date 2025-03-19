import esphome.codegen as cg
from esphome.components import i2c, sensor
import esphome.config_validation as cv
from esphome.const import CONF_ID

DEPENDENCIES = ["i2c"]

tinovi_leaf_sensor_ns = cg.esphome_ns.namespace("tinovi_leaf_sensor")
TinoviLeafSensor = tinovi_leaf_sensor_ns.class_(
    "TinoviLeafSensor", cg.PollingComponent, i2c.I2CDevice, sensor.Sensor
)

CONFIG_SCHEMA = (
    cv.Schema({cv.GenerateID(): cv.declare_id(TinoviLeafSensor)})
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x01))
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)
    await i2c.register_i2c_device(var, config)
