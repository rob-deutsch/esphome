#pragma once

#include "esphome/core/component.h"
#include "esphome/components/i2c/i2c.h"

namespace esphome::rv3032 {

class RV3032Component final : public Component, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;

  void set_clkout_fd(uint8_t fd) { this->clkout_fd_ = fd; }
  void set_offset(int8_t offset) { this->offset_ = offset; }

 protected:
  bool write_reg_(uint8_t reg, uint8_t value);

  uint8_t clkout_fd_{0};
  int8_t offset_{0};
};

}  // namespace esphome::rv3032
