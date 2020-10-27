#include "ir_sender_esphome.h"

namespace esphome {
namespace mitsubishi_heavy {

IRSenderESPHome::IRSenderESPHome(uint8_t pin, remote_transmitter::RemoteTransmitterComponent transmitter) : IRSender(pin)
{
  auto transmit = transmitter.transmit();
  _transmit = &transmit;
  _data = _transmit->get_data();
}

void IRSenderESPHome::setFrequency(int frequency)
{
  _data->set_carrier_frequency(frequency);
}

// Send an IR 'mark' symbol, i.e. transmitter ON
void IRSenderESPHome::mark(int markLength)
{
  _data->mark(markLength);
}

// Send an IR 'space' symbol, i.e. transmitter OFF
void IRSenderESPHome::space(int spaceLength)
{
  if (spaceLength) {
    _data->space(spaceLength);
  } else {
    _transmit->perform();
  }
}

}  // namespace mitsubishi_heavy
}  // namespace esphome
