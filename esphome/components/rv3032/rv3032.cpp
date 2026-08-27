#include "rv3032.h"
#include "esphome/core/hal.h"
#include "esphome/core/log.h"

// Application manual:
// - https://www.microcrystal.com/fileadmin/Media/Products/RTC/App.Manual/RV-3032-C7_App-Manual.pdf

namespace esphome::rv3032 {

static const char *const TAG = "rv3032";

static const uint8_t REG_CTRL1 = 0x10;
static const uint8_t REG_EECMD = 0x3F;
static const uint8_t REG_EEPROM_PMU = 0xC0;
static const uint8_t REG_EEPROM_OFFSET = 0xC1;
static const uint8_t REG_EEPROM_CLKOUT1 = 0xC2;
static const uint8_t REG_EEPROM_CLKOUT2 = 0xC3;

static const uint8_t CTRL1_X_AND_EERD = 0x24;  // X must be 1; EERD disables auto-refresh
static const uint8_t CTRL1_X = 0x20;
static const uint8_t EECMD_UPDATE = 0x11;  // Copy configuration RAM -> EEPROM

void RV3032Component::setup() {
  // C0h PMU: NCLKE=0 so CLKOUT is enabled. Leave backup/trickle charger off.
  // C1h Offset: 6-bit two's complement aging correction.
  // C2h/C3h CLKOUT: XTAL mode (OS=0), FD selects 32768/1024/64/1 Hz.
  const uint8_t offset_bits = static_cast<uint8_t>(this->offset_) & 0x3F;
  const uint8_t clkout2 = static_cast<uint8_t>(this->clkout_fd_ << 5);
  if (!this->write_reg_(REG_EEPROM_PMU, 0x00) || !this->write_reg_(REG_EEPROM_OFFSET, offset_bits) ||
      !this->write_reg_(REG_EEPROM_CLKOUT1, 0x00) || !this->write_reg_(REG_EEPROM_CLKOUT2, clkout2)) {
    this->mark_failed();
    return;
  }

  // Store RAM mirror into EEPROM so the setting survives power-off.
  if (!this->write_reg_(REG_CTRL1, CTRL1_X_AND_EERD) || !this->write_reg_(REG_EECMD, EECMD_UPDATE)) {
    this->mark_failed();
    return;
  }
  delay(50);  // NOLINT  tUPDATE is ~46 ms
  if (!this->write_reg_(REG_CTRL1, CTRL1_X)) {
    this->mark_failed();
  }
}

void RV3032Component::dump_config() {
  static const uint32_t FREQ_HZ[] = {32768, 1024, 64, 1};
  ESP_LOGCONFIG(TAG, "RV3032:");
  LOG_I2C_DEVICE(this);
  if (this->is_failed()) {
    ESP_LOGE(TAG, ESP_LOG_MSG_COMM_FAIL);
  }
  ESP_LOGCONFIG(TAG, "  CLKOUT: %u Hz", FREQ_HZ[this->clkout_fd_ & 0x03]);
  ESP_LOGCONFIG(TAG, "  Offset: %d", this->offset_);
}

bool RV3032Component::write_reg_(uint8_t reg, uint8_t value) {
  return this->write_register(reg, &value, 1) == i2c::ERROR_OK;
}

}  // namespace esphome::rv3032
