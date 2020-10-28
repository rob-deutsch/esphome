#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

namespace esphome {
namespace mitsubishi_heavy {

// Temperature
const uint8_t MITSUBISHI_TEMP_MIN = 18;  // Celsius
const uint8_t MITSUBISHI_TEMP_MAX = 30;  // Celsius

class MitsubishiHeavyClimate : public climate_ir::ClimateIR {
 public:
  MitsubishiHeavyClimate() : climate_ir::ClimateIR(MITSUBISHI_TEMP_MIN, MITSUBISHI_TEMP_MAX) {}

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
};

}  // namespace mitsubishi
}  // namespace esphome
