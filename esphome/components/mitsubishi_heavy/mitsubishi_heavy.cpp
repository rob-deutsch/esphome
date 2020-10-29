#include "mitsubishi_heavy.h"
#include "esphome/core/log.h"
#include "ir_sender_esphome.h"
#include <MitsubishiHeavyHeatpumpIR.h>

namespace esphome {
namespace mitsubishi_heavy {

static const char *TAG = "mitsubishi_heavy.climate";

void MitsubishiHeavyClimate::transmit_state() {
  uint8_t powerModeCmd;
  uint8_t operatingModeCmd;
  uint8_t temperatureCmd;
  uint8_t fanSpeedCmd;

  uint8_t swingVCmd = VDIR_SWING;
  uint8_t swingHCmd = HDIR_SWING;

  switch (this->fan_mode) {
    case climate::CLIMATE_FAN_LOW:
      fanSpeedCmd = FAN_2;
      break;
    case climate::CLIMATE_FAN_MEDIUM:
      fanSpeedCmd = FAN_3;
      break;
    case climate::CLIMATE_FAN_HIGH:
      fanSpeedCmd = FAN_4;
      break;
    case climate::CLIMATE_FAN_AUTO:
    default:
      fanSpeedCmd = FAN_AUTO;
      break;
  }

 switch (this->mode) {
    case climate::CLIMATE_MODE_COOL:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_COOL;
      break;
    case climate::CLIMATE_MODE_HEAT:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_HEAT;
      break;
    case climate::CLIMATE_MODE_AUTO:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_AUTO;
      break;
    case climate::CLIMATE_MODE_FAN_ONLY:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_FAN;
      break;
    case climate::CLIMATE_ACTION_DRYING:
      powerModeCmd = POWER_ON;
      operatingModeCmd = MODE_DRY;
      break;
    case climate::CLIMATE_MODE_OFF:
    default:
      powerModeCmd = POWER_OFF;
      operatingModeCmd = MODE_AUTO;
      break;
  }

  temperatureCmd = (uint8_t) clamp(this->target_temperature, MITSUBISHI_TEMP_MIN, MITSUBISHI_TEMP_MAX);

  IRSenderESPHome espSender(0, this->transmitter_);

  auto heatpumpIR = new MitsubishiHeavyZMHeatpumpIR();
  heatpumpIR->send(espSender, powerModeCmd, operatingModeCmd, fanSpeedCmd, temperatureCmd, swingVCmd, swingHCmd, false, false, false);
}

}  // namespace mitsubishi
}  // namespace esphome
