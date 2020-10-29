#pragma once

#include "esphome/components/climate_ir/climate_ir.h"

namespace esphome {
namespace mitsubishi_heavy {

// Simple enum to represent models.
enum Model {
  MODEL_ZJ = 0,
  MODEL_ZM = 1,
  MODEL_ZMP = 2,
};

// Temperature
const uint8_t MITSUBISHI_TEMP_MIN = 18;  // Celsius
const uint8_t MITSUBISHI_TEMP_MAX = 30;  // Celsius

class MitsubishiHeavyClimate : public climate_ir::ClimateIR {
 public:
  MitsubishiHeavyClimate()
      : climate_ir::ClimateIR(
            MITSUBISHI_TEMP_MIN, MITSUBISHI_TEMP_MAX, 1.0f, true, true,
            std::vector<climate::ClimateFanMode>{climate::CLIMATE_FAN_AUTO, climate::CLIMATE_FAN_LOW,
                                                 climate::CLIMATE_FAN_MEDIUM, climate::CLIMATE_FAN_HIGH}) {}
  void set_model(Model model) { this->model_ = model; }

 protected:
  /// Transmit via IR the state of this climate controller.
  void transmit_state() override;
  Model model_;
};

}  // namespace mitsubishi_heavy
}  // namespace esphome
