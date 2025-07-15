#include "EasyLoRa.hpp"

class EasyLoRa::PortDontOpen : public std::exception {
public:

    PortDontOpen(std::string_view port) 
    : message_m{ std::string("The port: ") + port.data() + " is not open" }
    {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return message_m.data();
    }

private:
    std::string message_m;
};

EasyLoRa::EasyLoRa(std::string_view port, uint32_t baudRate) 
: serialPort_m{ port.data(), baudRate, serial::Timeout::simpleTimeout(Default_Timeout_In_Ms) }
{
    if (!serialPort_m.isOpen()) {
        throw PortDontOpen{ port };
    }
}