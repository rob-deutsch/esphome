#include "IRSenderESPHome.h"

namespace esphome {
namespace mitsubishi_heavy {

IRSenderESPHome::IRSenderESPHome(uint8_t pin) : IRSender(pin)
{
  // _transmit = transmitter->transmit();
}

void IRSenderESPHome::setFrequency(int frequency)
{
  // auto data = _transmit.get_data();

  // data->set_carrier_frequency(frequency);
}

// Send an IR 'mark' symbol, i.e. transmitter ON
void IRSenderESPHome::mark(int markLength)
{
  // auto data = _transmit.get_data();
  // data->mark(markLength);
}

// Send an IR 'space' symbol, i.e. transmitter OFF
void IRSenderESPHome::space(int spaceLength)
{
  if (spaceLength) {
    // auto data = _transmit.get_data();
    // data->space(spaceLength);
  } else {
    // _transmit.perform();
  }
}

}  // namespace mitsubishi_heavy
}  // namespace esphome
