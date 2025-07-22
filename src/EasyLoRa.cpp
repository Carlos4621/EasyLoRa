#include "EasyLoRa.hpp"

EasyLoRa::EasyLoRa(std::string_view port) 
: serialPort_m{ port.data(), Default_BaudRate, serial::Timeout::simpleTimeout(Default_Timeout_In_Ms) }
{
    if (!serialPort_m.isOpen()) {
        throw PortDontOpen{ port };
    }

    actualConfiguration_m = getConfigurationFromModule();
    syncBaudRate();
}

void EasyLoRa::setConfiguration(const ModuleConfig &config) {

    if (actualConfiguration_m == config) {
        return;
    }
    
    Envelope packageToSend;
    *packageToSend.mutable_configuration() = config.toProtobuf();
    
    sendPackage(packageToSend);
    const auto succesData{ getSuccesStatus() };
    throwIfSuccesStatusError(succesData);
    
    actualConfiguration_m = config;
    syncBaudRate();
}

ModuleConfig EasyLoRa::getConfiguration() const noexcept {
    return actualConfiguration_m;
}

void EasyLoRa::sendPackage(const Envelope& package) {
    const auto serializedPackage{ serializePackage(package) };
    writeToSerial(serializedPackage);
}

std::string EasyLoRa::serializePackage(const Envelope& package) {
    std::string serializedPackage;
    if (!package.SerializeToString(&serializedPackage)) {
        throw SerializeError{};
    }
    
    serializedPackage.insert(serializedPackage.cbegin(), static_cast<uint8_t>(serializedPackage.size()));
    return serializedPackage;
}

void EasyLoRa::writeToSerial(const std::string& data) {
    const auto bytesWritten{ serialPort_m.write(data) };
    if (bytesWritten != data.size()) {
        throw WriteError{ bytesWritten, data.size() };
    }
}

std::string EasyLoRa::getResponseData() {
    const auto success{ getSuccesStatus() };
    
    throwIfSuccesStatusError(success);

    return success.data();
}

SuccessStatus EasyLoRa::getSuccesStatus() {
    const auto rawData{ readFromSerial() };

    return deserializeSuccessStatus(rawData);
}

std::string EasyLoRa::readFromSerial() {
    const auto messageLength{ static_cast<uint8_t>(serialPort_m.read(Size_Byte_Length)[0]) };

    const auto serializedResponse{ serialPort_m.read(messageLength) };

    if (serializedResponse.empty()) {
        throw SuccessDontReceived{};
    }
    return serializedResponse;
}

SuccessStatus EasyLoRa::deserializeSuccessStatus(const std::string& data) {
    SuccessStatus succesStatus;
    
    if (!succesStatus.ParseFromArray(data.data(), data.size())) {
        throw DeserializeError{};
    }
    
    return succesStatus;
}

void EasyLoRa::throwIfSuccesStatusError(const SuccessStatus &status) {
    if (status.PossibleData_case() == SuccessStatus::kError) {
        throw ModuleError{ status.error() };
    }
}

ModuleConfig EasyLoRa::getConfigurationFromModule() {
    Envelope packageToSend;
    packageToSend.set_requestconfiguration(true);

    sendPackage(packageToSend);
    const auto data{ getResponseData() };

    ModuleConfiguration protobuffData;
    if (!protobuffData.ParseFromArray(data.data(), data.size())) {
        throw DeserializeError{};
    }

    return ModuleConfig::fromProtobuf(protobuffData);
}

void EasyLoRa::syncBaudRate() {
    switch (actualConfiguration_m.getUartBaudRate()) {
    case UART_1200_BPS:
        serialPort_m.setBaudrate(1200);
        break;
    
    case UART_2400_BPS:
        serialPort_m.setBaudrate(2400);
        break;

    case UART_4800_BPS:
        serialPort_m.setBaudrate(4800);
        break;
    
    case UART_9600_BPS:
        serialPort_m.setBaudrate(9600);
        break;

    case UART_19200_BPS:
        serialPort_m.setBaudrate(19200);
        break;

    case UART_38400_BPS:
        serialPort_m.setBaudrate(38400);
        break;

    case UART_57600_BPS:
        serialPort_m.setBaudrate(57600);
        break;

    case UART_115200_BPS:
        serialPort_m.setBaudrate(115200);
        break;

    default:
        std::unreachable();
        break;
    }
}
