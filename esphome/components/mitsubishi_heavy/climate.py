import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import climate_ir
from esphome.components.heatpumpir import climate as heatpumpir
from esphome.const import CONF_ID, CONF_MAX_TEMPERATURE, CONF_MIN_TEMPERATURE, \
    CONF_MODEL, CONF_VISUAL

AUTO_LOAD = ['heatpumpir', 'climate_ir']

mitsubishi_heavy_ns = cg.esphome_ns.namespace('mitsubishi_heavy')
MitsubishiHeavyClimate = mitsubishi_heavy_ns.class_('MitsubishiHeavyClimate', heatpumpir.HeatpumpIRClimate)

Model = mitsubishi_heavy_ns.enum('Model')
MODELS = {
    'ZJ': Model.MODEL_ZJ,
    'ZM': Model.MODEL_ZM,
    'ZMP': Model.MODEL_ZMP,
}

CONFIG_SCHEMA = climate_ir.CLIMATE_IR_WITH_RECEIVER_SCHEMA.extend({
    cv.GenerateID(): cv.declare_id(MitsubishiHeavyClimate),
    cv.Required(CONF_MODEL): cv.enum(MODELS, upper=True),
    cv.Required(heatpumpir.CONF_HORIZONTAL_DEFAULT): cv.enum(heatpumpir.HORIZONTAL_DIRECTIONS),
    cv.Required(heatpumpir.CONF_VERTICAL_DEFAULT): cv.enum(heatpumpir.VERTICAL_DIRECTIONS),
})


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    visual = config[CONF_VISUAL]
    if CONF_MAX_TEMPERATURE not in visual:
        visual[CONF_MAX_TEMPERATURE] = 30
    if CONF_MIN_TEMPERATURE not in visual:
        visual[CONF_MIN_TEMPERATURE] = 18
    yield climate_ir.register_climate_ir(var, config)
    cg.add(var.set_model(config[CONF_MODEL]))
    cg.add(var.set_horizontal_default(config[heatpumpir.CONF_HORIZONTAL_DEFAULT]))
    cg.add(var.set_vertical_default(config[heatpumpir.CONF_VERTICAL_DEFAULT]))

