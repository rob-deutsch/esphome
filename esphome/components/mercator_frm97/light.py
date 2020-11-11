import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import CONF_OUTPUT_ID, CONF_OUTPUT
from . import mercator_frm97_ns, MercatorFRM97Output

MercatorFRM97Light = mercator_frm97_ns.class_('MercatorFRM97Light', light.LightOutput)

CONF_MERCATOR_FRM97_ID = 'mercator_frm97_id'

CONFIG_SCHEMA = light.BINARY_LIGHT_SCHEMA.extend({
    cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(MercatorFRM97Light),
    cv.GenerateID(CONF_MERCATOR_FRM97_ID): cv.use_id(MercatorFRM97Output),
})


def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    yield light.register_light(var, config)

    paren = yield cg.get_variable(config[CONF_MERCATOR_FRM97_ID])
    cg.add(var.set_parent(paren))
    cg.add(paren.set_light(var))
