#include "mitsubishi_heavy.h"
#include "esphome/core/log.h"

namespace esphome {
namespace mitsubishi_heavy {

IRSenderESP32 irSender(2, 0);     // IR led on ESP22 digital pin 2, 0 - ESP32 LEDC channel. 

static const char *TAG = "mitsubishi_heavy.climate";

void MitsubishiHeavyClimate::setup() {
  heatpumpIR = new MitsubishiHeavyZMHeatpumpIR();
}

void MitsubishiHeavyClimate::transmit_state() {
  uint8_t powerModeCmd;
  uint8_t operatingModeCmd;
  uint8_t temperatureCmd;

  uint8_t fanSpeedCmd = FAN_AUTO;
  uint8_t swingVCmd = VDIR_AUTO;
  uint8_t swingHCmd = HDIR_AUTO;

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
    case climate::CLIMATE_MODE_OFF:
    default:
      powerModeCmd = POWER_OFF;
      operatingModeCmd = MODE_AUTO;
      break;
  }

  temperatureCmd = (uint8_t) clamp(this->target_temperature, MITSUBISHI_TEMP_MIN, MITSUBISHI_TEMP_MAX);
}

}  // namespace mitsubishi
}  // namespace esphome
