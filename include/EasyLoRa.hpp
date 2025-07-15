#ifndef EASY_LORA_HPP
#define EASY_LORA_HPP

#include "serial/serial.h"
#include <string_view>

class EasyLoRa {
public:
    class PortDontOpen;

    EasyLoRa(std::string_view port, uint32_t baudRate = 115200);

private:
    static constexpr uint16_t Default_Timeout_In_Ms = 1000;

    serial::Serial serialPort_m;
};

#endif // EASY_LORA_HPP