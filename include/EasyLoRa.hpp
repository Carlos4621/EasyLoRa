#ifndef EASY_LORA_HPP
#define EASY_LORA_HPP

#include "serial/serial.h"
#include <string_view>
#include "Envelope.pb.h"
#include "ModuleConfig.hpp"
#include "EasyLoRaExceptions.hpp"
#include "SuccessStatus.pb.h"

class EasyLoRa {
public:
    explicit EasyLoRa(std::string_view port, uint32_t baudRate = 115200);

    void setConfiguration(const ModuleConfig &config);

    [[nodiscard]]
    ModuleConfig getConfiguration();

    void sendMessage(const std::string &message);

    std::string receiveMessage();

private:
    static constexpr uint16_t Default_Timeout_In_Ms{ 2000 };

    serial::Serial serialPort_m;

    void sendPackage(const Envelope& package);

    [[nodiscard]]
    std::string serializePackage(const Envelope& package);

    void writeToSerial(const std::string& data);

    [[nodiscard]]
    std::string getResponseData();

    [[nodiscard]]
    SuccessStatus getSuccesStatus();

    [[nodiscard]]
    std::string readFromSerial();

    [[nodiscard]]
    SuccessStatus deserializeSuccessStatus(const std::string& data);

    void throwIfSuccesStatusError(const SuccessStatus& status);
};

#endif // EASY_LORA_HPP