#pragma once

#include "esphome/components/heatpumpir/heatpumpir.h"

namespace esphome {
namespace mitsubishi_heavy {

// Simple enum to represent models.
enum Model {
  MODEL_ZJ = 0,
  MODEL_ZM = 1,
  MODEL_ZMP = 2,
};

class MitsubishiHeavyClimate : public heatpumpir::HeatpumpIRClimate {
 public:
  void set_model(Model model) { this->model_ = model; }
 protected:
  void transmit_state() override;
  Model model_;
};

}  // namespace mitsubishi_heavy
}  // namespace esphome
