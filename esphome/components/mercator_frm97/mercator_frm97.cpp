#include "mercator_frm97.h"
#include "esphome/core/log.h"
#include "esphome/core/helpers.h"

namespace esphome {
namespace mercator_frm97 {

static const char *TAG = "mercator_frm97";

// MercatorFRM97Light

light::LightTraits MercatorFRM97Light::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supports_brightness(false);
  return traits;
}

void MercatorFRM97Light::setup_state(light::LightState *state) { state_ = state; }

void MercatorFRM97Light::update_state(bool state) {
  auto call = state_->make_call();
  call.set_state(state);
  call.set_output(false);
  call.perform();
};

void MercatorFRM97Light::write_state(light::LightState *state) {
  bool binary;
  state->current_values_as_binary(&binary);
  this->parent_->set_lamp(binary);
};

// MercatorFRM97Fan

void MercatorFRM97Fan::dump_config() { ESP_LOGCONFIG(TAG, "Fan '%s':", this->fan_->get_name().c_str()); }
void MercatorFRM97Fan::setup() {
  auto traits = fan::FanTraits(false, true, false);
  this->fan_->set_traits(traits);
  this->fan_->add_on_state_callback([this]() { this->next_update_ = true; });
}
void MercatorFRM97Fan::loop() {
  if (!this->next_update_) {
    return;
  }
  this->next_update_ = false;

  FanState fan_state = FAN_OFF;
  if (this->fan_->state) {
    if (this->fan_->speed == fan::FAN_SPEED_LOW)
      fan_state = FAN_LOW;
    else if (this->fan_->speed == fan::FAN_SPEED_MEDIUM)
      fan_state = FAN_MEDIUM;
    else if (this->fan_->speed == fan::FAN_SPEED_HIGH)
      fan_state = FAN_HIGH;
  }
  this->parent_->set_fan(fan_state);
}
float MercatorFRM97Fan::get_setup_priority() const { return setup_priority::DATA; }

// MercatorFRM97Output

void MercatorFRM97Output::set_lamp(bool state) {
  if (state) {
    send_command(0x9F);
  } else {
    send_command(0x8C);
  }
};

void MercatorFRM97Output::set_fan(FanState state) {
  switch (state) {
    case FAN_LOW:
      send_command(0x18);
      break;
    case FAN_MEDIUM:
      send_command(0x08);
      break;
    case FAN_HIGH:
      send_command(0x88);
      break;
    default:
    case FAN_OFF:
      send_command(0x28);
      break;
  }
}

void MercatorFRM97Output::send_command(uint8_t cmd) {
  auto transmit = this->transmitter_->transmit();
  transmit.set_send_times(14);
  transmit.set_send_wait(2025);
  auto data = transmit.get_data();

  uint32_t value = 0xC5689000;

  value |= (address_ & 0xF) << 8;
  value |= cmd;

  data->item(1635, 400);
  for (uint32_t mask = 1UL << (8 * sizeof(uint32_t) - 1); mask != 0; mask >>= 1) {
    if (value & mask)
      data->item(820, 400);
    else
      data->item(410, 400);
  }

  transmit.perform();
}

}  // namespace mercator_frm97
}  // namespace esphome
