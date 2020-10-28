#pragma once

#include "esphome/components/remote_base/remote_base.h"
#include "esphome/components/remote_transmitter/remote_transmitter.h"
#include <IRSender.h> // arduino-heatpump library


namespace esphome {
namespace mitsubishi_heavy {

class IRSenderESPHome : public IRSender
{
  public:
    IRSenderESPHome(uint8_t pin);
    void setFrequency(int frequency);
    void space(int spaceLength);
    void mark(int markLength);

  protected:
    // remote_base::RemoteTransmitterBase::TransmitCall _transmit;
};


}  // namespace mitsubishi_heavy
}  // namespace esphome

