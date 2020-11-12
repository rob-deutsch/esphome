import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import fan, output
from esphome.const import CONF_ID
from . import MercatorFRM97Output, mercator_frm97_ns

mercator_frm97_ns = cg.esphome_ns.namespace('mercator_frm97')
MercatorFRM97Fan = mercator_frm97_ns.class_('MercatorFRM97Fan', fan.Fan, cg.Component)

CONF_MERCATOR_FRM97_ID = 'mercator_frm97_id'

CONFIG_SCHEMA = fan.FAN_RAW_SCHEMA.extend({
    cv.GenerateID(CONF_ID): cv.declare_id(MercatorFRM97Fan),
    cv.GenerateID(CONF_MERCATOR_FRM97_ID): cv.use_id(MercatorFRM97Output),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    paren = yield cg.get_variable(config[CONF_MERCATOR_FRM97_ID])
    var = cg.new_Pvariable(config[CONF_ID], paren)
    yield fan.register_fan(var, config)
    cg.add(paren.set_fan(var))
