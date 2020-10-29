import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate_ir
from esphome.const import CONF_ID, CONF_MODEL

AUTO_LOAD = ['climate_ir']

mitsubishi_heavy_ns = cg.esphome_ns.namespace('mitsubishi_heavy')
MitsubishiHeavyClimate = mitsubishi_heavy_ns.class_('MitsubishiHeavyClimate', climate_ir.ClimateIR)

Model = mitsubishi_heavy_ns.enum('Model')
MODELS = {
    'ZJ': Model.MODEL_ZJ,
    'ZM': Model.MODEL_ZM,
    'ZMP': Model.MODEL_ZMP,
}

CONFIG_SCHEMA = climate_ir.CLIMATE_IR_WITH_RECEIVER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(MitsubishiHeavyClimate),
    cv.Required(CONF_MODEL): cv.enum(MODELS, upper=True)
})


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield climate_ir.register_climate_ir(var, config)
    cg.add(var.set_model(config[CONF_MODEL]))

    cg.add_library('HeatpumpIR', None)
