#include "mitsubishi_heavy.h"
#include "esphome/core/log.h"
#include "esphome/components/heatpumpir/ir_sender_esphome.h"
#include <MitsubishiHeavyHeatpumpIR.h>
#include <HeatpumpIR.h>

namespace esphome {
namespace mitsubishi_heavy {

static const char *TAG = "mitsubishi_heavy.climate";

void MitsubishiHeavyClimate::transmit_state() {
 uint8_t power_mode_cmd;
  uint8_t operating_mode_cmd;
  uint8_t temperature_cmd;
  uint8_t fan_speed_cmd;

  uint8_t swing_v_cmd;
  switch (default_vertical_direction_) {
    case heatpumpir::VERTICAL_DIRECTION_AUTO:
      swing_v_cmd = VDIR_AUTO;
      break;
    case heatpumpir::VERTICAL_DIRECTION_UP:
      swing_v_cmd = VDIR_UP;
      break;
    case heatpumpir::VERTICAL_DIRECTION_MUP:
      swing_v_cmd = VDIR_MUP;
      break;
    case heatpumpir::VERTICAL_DIRECTION_MIDDLE:
      swing_v_cmd = VDIR_MIDDLE;
      break;
    case heatpumpir::VERTICAL_DIRECTION_MDOWN:
      swing_v_cmd = VDIR_MDOWN;
      break;
    case heatpumpir::VERTICAL_DIRECTION_DOWN:
      swing_v_cmd = VDIR_DOWN;
      break;
    default:
      ESP_LOGE(TAG, "Invalid default vertical direction");
      return;
  }
  if ((this->swing_mode == climate::CLIMATE_SWING_VERTICAL) || (this->swing_mode == climate::CLIMATE_SWING_BOTH)) {
    swing_v_cmd = VDIR_SWING;
  }

  uint8_t swing_h_cmd;
  switch (default_horizontal_direction_) {
    case heatpumpir::HORIZONTAL_DIRECTION_AUTO:
      swing_h_cmd = HDIR_AUTO;
      break;
    case heatpumpir::HORIZONTAL_DIRECTION_MIDDLE:
      swing_h_cmd = HDIR_MIDDLE;
      break;
    case heatpumpir::HORIZONTAL_DIRECTION_LEFT:
      swing_h_cmd = HDIR_LEFT;
      break;
    case heatpumpir::HORIZONTAL_DIRECTION_MLEFT:
      swing_h_cmd = HDIR_MLEFT;
      break;
    case heatpumpir::HORIZONTAL_DIRECTION_MRIGHT:
      swing_h_cmd = HDIR_MRIGHT;
      break;
    case heatpumpir::HORIZONTAL_DIRECTION_RIGHT:
      swing_h_cmd = HDIR_RIGHT;
      break;
    default:
      ESP_LOGE(TAG, "Invalid default horizontal direction");
      return;
  }
  if ((this->swing_mode == climate::CLIMATE_SWING_HORIZONTAL) || (this->swing_mode == climate::CLIMATE_SWING_BOTH)) {
    swing_h_cmd = HDIR_SWING;
  }

  switch (this->fan_mode) {
    case climate::CLIMATE_FAN_LOW:
      fan_speed_cmd = FAN_2;
      break;
    case climate::CLIMATE_FAN_MEDIUM:
      fan_speed_cmd = FAN_3;
      break;
    case climate::CLIMATE_FAN_HIGH:
      fan_speed_cmd = FAN_4;
      break;
    case climate::CLIMATE_FAN_AUTO:
    default:
      fan_speed_cmd = FAN_AUTO;
      break;
  }

  switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      power_mode_cmd = POWER_ON;
      operating_mode_cmd = MODE_COOL;
      break;
    case climate::CLIMATE_MODE_HEAT:
      power_mode_cmd = POWER_ON;
      operating_mode_cmd = MODE_HEAT;
      break;
    case climate::CLIMATE_MODE_AUTO:
      power_mode_cmd = POWER_ON;
      operating_mode_cmd = MODE_AUTO;
      break;
    case climate::CLIMATE_MODE_FAN_ONLY:
      power_mode_cmd = POWER_ON;
      operating_mode_cmd = MODE_FAN;
      break;
    case climate::CLIMATE_ACTION_DRYING:
      power_mode_cmd = POWER_ON;
      operating_mode_cmd = MODE_DRY;
      break;
    case climate::CLIMATE_MODE_OFF:
    default:
      power_mode_cmd = POWER_OFF;
      operating_mode_cmd = MODE_AUTO;
      break;
  }

  temperature_cmd = (uint8_t) clamp(this->target_temperature, 18, 30);

  // TODO: Are the following memory leaks?
  heatpumpir::IRSenderESPHome esp_sender(0, this->transmitter_);

  MitsubishiHeavyHeatpumpIR *heatpump_ir;
  switch(model_) {
    case MODEL_ZJ:
      heatpump_ir = new MitsubishiHeavyZJHeatpumpIR();
      break;
    case MODEL_ZM:
      heatpump_ir = new MitsubishiHeavyZMHeatpumpIR();
      break;
    case MODEL_ZMP:
      heatpump_ir = new MitsubishiHeavyZMPHeatpumpIR();
      break;
    default:
      ESP_LOGE(TAG, "Invalid model");
      return;
  }
  heatpump_ir->send(esp_sender, power_mode_cmd, operating_mode_cmd, fan_speed_cmd,
                    temperature_cmd, swing_v_cmd, swing_h_cmd, false, false, false);
}

}  // namespace mitsubishi_heavy
}  // namespace esphome
