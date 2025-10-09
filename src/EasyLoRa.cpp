#include "EasyLoRa.hpp"

EasyLoRa::EasyLoRa(std::string_view port) 
: serialPort_m{ port.data(), Default_BaudRate, serial_cpp::Timeout::simpleTimeout(Default_Timeout_In_Ms) }
{
    if (!serialPort_m.isOpen()) {
        throw PortDontOpen{ port };
    }

    actualConfiguration_m = getConfigurationFromModule();
    serialPort_m.setBaudrate(ModuleConfig::toBaudRateValue(actualConfiguration_m.getUartBaudRate()));
}

void EasyLoRa::setConfiguration(const ModuleConfig &config, bool syncWithReceiver) {
    if (actualConfiguration_m == config) {
        return;
    }
    
    serialPort_m.setTimeout(serial_cpp::Timeout::simpleTimeout(Default_Configuration_Sync_Timeout_In_Ms));

    Envelope packageToSend;
    *packageToSend.mutable_configuration() = config.toProtobuf();
    packageToSend.mutable_configuration()->set_syncwithreceiver(syncWithReceiver);
    
    if (syncWithReceiver) {
        packageToSend.mutable_ackrequirements()->set_timeout(Default_Configuration_Sync_Timeout_In_Ms);
    }
    
    const auto ack{ requestInformation(packageToSend) };
    if (!ack.ack()) {
        throw SuccessDontReceived{};
    }

    if (syncWithReceiver) {
        const auto response{ deserializeEnvelope(readFromSerial()) };
        if (!response.ack()) {
            throw SuccessDontReceived{};
        }
    }

    serialPort_m.setTimeout(serial_cpp::Timeout::simpleTimeout(Default_Timeout_In_Ms));
    
    actualConfiguration_m = config;
    serialPort_m.setBaudrate(ModuleConfig::toBaudRateValue(actualConfiguration_m.getUartBaudRate()));
}

ModuleConfig EasyLoRa::getConfigurationFromModule() {
    Envelope packageToSend;
    packageToSend.set_requestconfiguration(true);

    const auto response{ requestInformation(packageToSend) };
    if (!response.has_configuration()) {
        throw SuccessDontReceived{};
    }

    return ModuleConfig::fromProtobuf(response.configuration());
}

ModuleConfig EasyLoRa::getConfiguration() const noexcept {
    return actualConfiguration_m;
}

void EasyLoRa::sendData(std::string_view message) {
    Envelope packageToSend;
    *packageToSend.mutable_data() = message;
    sendEnvelope(packageToSend);
}

// TODO: Implementar timeout, Si un paquete que no es data o error es recibido, se producirá violación de segmento
std::string EasyLoRa::receiveData() {
    const auto response{ deserializeEnvelope(readFromSerial()) };
    throwIfEnvelopeError(response);

    return response.data();
}

void EasyLoRa::sendEnvelope(const Envelope& package) {
    const auto serializedPackage{ serializePackage(package) };
    writeToSerial(serializedPackage);
}

Envelope EasyLoRa::requestInformation(const Envelope &package) {
    sendEnvelope(package);

    serialPort_m.flush();

    const auto response{ deserializeEnvelope(readFromSerial()) };
    throwIfEnvelopeError(response);

    return response;
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

std::string EasyLoRa::readFromSerial() {
    const auto messageLength{ static_cast<uint8_t>(serialPort_m.read(Size_Byte_Length)[0]) };
    
    return serialPort_m.read(messageLength);
}

Envelope EasyLoRa::deserializeEnvelope(const std::string& data) {
    Envelope envelope;
    
    if (!envelope.ParseFromArray(data.data(), data.size())) {
        throw DeserializeError{};
    }
    
    return envelope;
}

void EasyLoRa::throwIfEnvelopeError(const Envelope& status) {
    if (status.PosibleData_case() == Envelope::kError) {
        throw ModuleError{ status.error() };
    }
}
