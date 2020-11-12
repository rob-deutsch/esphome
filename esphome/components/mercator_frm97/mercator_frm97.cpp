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
  this->parent_->set_lamp_state(binary);
};

// MercatorFRM97Fan

void MercatorFRM97Fan::dump_config() { ESP_LOGCONFIG(TAG, "Fan '%s':", this->get_name().c_str()); }
void MercatorFRM97Fan::setup() {
  auto traits = fan::FanTraits(false, true, false);
  this->set_traits(traits);

  this->restore_state_();
}
void MercatorFRM97Fan::control() {
  FanState fan_state = FAN_OFF;
  if (this->state) {
    if (this->speed == fan::FAN_SPEED_LOW)
      fan_state = FAN_LOW;
    else if (this->speed == fan::FAN_SPEED_MEDIUM)
      fan_state = FAN_MEDIUM;
    else if (this->speed == fan::FAN_SPEED_HIGH)
      fan_state = FAN_HIGH;
  }
  this->parent_->set_fan_state(fan_state);

  this->publish_state();
}
float MercatorFRM97Fan::get_setup_priority() const { return setup_priority::DATA; }

void MercatorFRM97Fan::update_state(FanState state) {
  this->state = true;
  switch(state) {
    case FAN_LOW:
      this->speed = fan::FAN_SPEED_LOW;
      break;
    case FAN_MEDIUM:
      this->speed = fan::FAN_SPEED_MEDIUM;
      break;
    case FAN_HIGH:
      this->speed = fan::FAN_SPEED_HIGH;
      break;
    default:
    case FAN_OFF:
      this->state = false;
  }
  this->publish_state();
};

// MercatorFRM97Output

void MercatorFRM97Output::set_lamp_state(bool state) {
  if (state) {
    send_command(0x9F);
  } else {
    send_command(0x8C);
  }
};

void MercatorFRM97Output::set_fan_state(FanState state) {
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
