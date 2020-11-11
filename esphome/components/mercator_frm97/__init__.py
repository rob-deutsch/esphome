import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import remote_receiver, remote_transmitter
from esphome.components.remote_base import CONF_RECEIVER_ID, CONF_TRANSMITTER_ID
from esphome.const import CONF_ADDRESS, CONF_ID

DEPENDENCIES = ['remote_transmitter']
MULTI_CONF = True

mercator_frm97_ns = cg.esphome_ns.namespace('mercator_frm97')
MercatorFRM97Output = mercator_frm97_ns.class_('MercatorFRM97Output', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_TRANSMITTER_ID): cv.use_id(remote_transmitter.RemoteTransmitterComponent),
    cv.GenerateID(): cv.declare_id(MercatorFRM97Output),
    cv.Required(CONF_ADDRESS): cv.int_range(min=0, max=255),
    cv.Optional(CONF_RECEIVER_ID): cv.use_id(remote_receiver.RemoteReceiverComponent),
}).extend(cv.COMPONENT_SCHEMA)


def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    yield cg.register_component(var, config)

    cg.add(var.set_address(config[CONF_ADDRESS]))

    transmitter = yield cg.get_variable(config[CONF_TRANSMITTER_ID])
    cg.add(var.set_transmitter(transmitter))

    if CONF_RECEIVER_ID in config:
        receiver = yield cg.get_variable(config[CONF_RECEIVER_ID])
        cg.add(receiver.register_listener(var))
    