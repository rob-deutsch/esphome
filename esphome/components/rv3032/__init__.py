import esphome.codegen as cg
from esphome.components import i2c
import esphome.config_validation as cv
from esphome.const import CONF_FREQUENCY, CONF_ID, CONF_OFFSET
from esphome.types import ConfigType

CODEOWNERS = ["@rob-deutsch"]
DEPENDENCIES = ["i2c"]

# XTAL CLKOUT frequencies (FD field in EEPROM C3h)
CLKOUT_FD = {
    32768: 0,
    1024: 1,
    64: 2,
    1: 3,
}

rv3032_ns = cg.esphome_ns.namespace("rv3032")
RV3032Component = rv3032_ns.class_("RV3032Component", cg.Component, i2c.I2CDevice)


def validate_clkout_frequency(value: object) -> int:
    freq = int(round(cv.frequency(value)))
    if freq not in CLKOUT_FD:
        raise cv.Invalid("CLKOUT frequency must be 1Hz, 64Hz, 1024Hz or 32768Hz")
    return freq


CONFIG_SCHEMA = (
    cv.Schema(
        {
            cv.GenerateID(): cv.declare_id(RV3032Component),
            cv.Optional(CONF_FREQUENCY, default="32768Hz"): validate_clkout_frequency,
            cv.Optional(CONF_OFFSET, default=0): cv.int_range(min=-32, max=31),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
    .extend(i2c.i2c_device_schema(0x51))
)


async def to_code(config: ConfigType) -> None:
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await i2c.register_i2c_device(var, config)
    cg.add(var.set_clkout_fd(CLKOUT_FD[config[CONF_FREQUENCY]]))
    cg.add(var.set_offset(config[CONF_OFFSET]))
