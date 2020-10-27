#pragma once

#include "esphome/components/climate_ir/climate_ir.h"
#include <MitsubishiHeavyHeatpumpIR.h>

namespace esphome {
namespace mitsubishi_heavy {

// Temperature
const uint8_t MITSUBISHI_TEMP_MIN = 16;  // Celsius
const uint8_t MITSUBISHI_TEMP_MAX = 31;  // Celsius

class MitsubishiHeavyClimate : public climate_ir::ClimateIR {
 public:
  MitsubishiHeavyClimate() : climate_ir::ClimateIR(MITSUBISHI_TEMP_MIN, MITSUBISHI_TEMP_MAX) {}
  void setup() override;

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
  MitsubishiHeavyHeatpumpIR *heatpumpIR;
};

}  // namespace mitsubishi
}  // namespace esphome
