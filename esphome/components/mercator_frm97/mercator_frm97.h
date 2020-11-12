#pragma once

#include "esphome/core/component.h"
#include "esphome/components/remote_base/remote_base.h"
#include "esphome/components/remote_base/raw_protocol.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"
#include "esphome/components/light/light_output.h"
#include "esphome/core/log.h"
#include "esphome/components/fan/fan.h"

namespace esphome {
namespace mercator_frm97 {

const uint8_t COMMAND_FRAME_SIZE = 4;

// Simple enum to represent the speed of a fan.
enum FanState {
  FAN_OFF = 0,
  FAN_LOW = 1,
  FAN_MEDIUM = 2,
  FAN_HIGH = 3,
};

class MercatorFRM97Output;

class MercatorFRM97Light : public light::LightOutput {
 public:
  void setup_state(light::LightState *state) override;
  void write_state(light::LightState *state) override;
  void update_state(bool state);
  void set_parent(MercatorFRM97Output *parent) { parent_ = parent; }
  light::LightTraits get_traits() override;

 protected:
  MercatorFRM97Output *parent_;
  light::LightState *state_{nullptr};
};

class MercatorFRM97Fan : public Component, public fan::Fan {
 public:
  MercatorFRM97Fan(MercatorFRM97Output *parent) : parent_(parent) {}
  void setup() override;
  void dump_config() override;
  float get_setup_priority() const override;

 protected:
  MercatorFRM97Output *parent_;
  void control() override;
};

class MercatorFRM97Output : public Component, public remote_base::RemoteReceiverListener {
 public:
  void set_address(uint8_t adress) { address_ = adress; }
  void set_transmitter(remote_transmitter::RemoteTransmitterComponent *transmitter) {
    this->transmitter_ = transmitter;
  }

  void set_lamp(bool state);
  void set_fan(FanState state);

  void set_light(MercatorFRM97Light *light) { light_ = light; }

  bool on_receive(remote_base::RemoteReceiveData data) override { 
    if (!data.expect_item(1635, 400))
      return false;

    // auto rd = new remote_base::RawDumper;
    // rd->dump(data);

    // auto raw_data = data.get_raw_data();
    // for (uint16_t i = 50; (i < raw_data->size()) && (i < 72); i += 2) {
    //   if (i+2 < raw_data->size()) {
    //     ESP_LOGD("mercator_frm97", "s %i %i", (*raw_data)[i], (*raw_data)[i+1]);
    //   } else {
    //     ESP_LOGD("mercator_frm97", "s %i", (*raw_data)[i]);
    //   }
    // }

    uint8_t command_frame[COMMAND_FRAME_SIZE] = {};

    for (uint8_t i = 0; i < COMMAND_FRAME_SIZE; i++) {
      if (!this->read_byte(data, command_frame+i)) {
        LOG_V("mercator_frm97", "Invalid byte %i", i);
        return false;
      }
    }

    if (command_frame[0] != 0xC5 || command_frame[1] != 0x68 || (command_frame[2] >> 4) != 0x9) {
      LOG_V("mercator_frm97", "Invalid header 0x%02X%02X%X", command_frame[0], command_frame[1], command_frame[2] >> 4);
      return false;
    }

    if ((command_frame[2] & 0xF) != this->address_) {
      LOG_V("mercator_frm97", "Irrelevant address 0x%X", command_frame[2] & 0xF);
      return false;
    }

    switch (command_frame[3]) {
      case 0x08:
        // Fan medium
        break;
      case 0x18:
        // Fan low
        break;
      case 0x28:
        // Fan off
        break;
      case 0x88:
        // Fan high
        break;
      case 0x8C:
        // Light off
        this->light_->update_state(false);
        break;
      case 0x9F:
        // Light on
        this->light_->update_state(true);
        break;
      default:
        ESP_LOGV("mercator_frm97", "Invalid command 0x%02X", command_frame[3]);
        break;
    }

    ESP_LOGD("mercator_frm97", "Valid request to address 0x%02X command 0x%02X", this->address_, command_frame[3]);
    return true;
  }
 protected:
  bool read_byte(remote_base::RemoteReceiveData &data, uint8_t *b) {
    uint8_t temp =0;
    for (uint8_t i = 0; i < 8; i++) {
      if (data.expect_mark(750)) {
        // Found 1. Set appropriate bit.
        temp |= (1 << (7-i));
      } else if (data.expect_mark(350)) {
        // Found 0. No action needed.        
      } else {
        return false;
      }
      data.advance();
    }
    *b = temp;
    return true;
  } 
  uint8_t address_;
  remote_transmitter::RemoteTransmitterComponent *transmitter_;
  MercatorFRM97Light *light_;
  void send_command(uint8_t cmd);
};

}  // namespace mercator_frm97
}  // namespace esphome
