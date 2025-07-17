#include "EasyLoRa.hpp"

EasyLoRa::EasyLoRa(std::string_view port, uint32_t baudRate) 
: serialPort_m{ port.data(), baudRate, serial::Timeout::simpleTimeout(Default_Timeout_In_Ms) }
{
    if (!serialPort_m.isOpen()) {
        throw PortDontOpen{ port };
    }
}

void EasyLoRa::setConfiguration(const ModuleConfig &config) {
    Envelope packageToSend;
    *packageToSend.mutable_configuration() = config.toProtobuf();
    
    sendPackage(packageToSend);
    const auto succesData{ getSuccesStatus() };
    throwIfSuccesStatusError(succesData);
}

ModuleConfig EasyLoRa::getConfiguration() {
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
