#ifndef EASY_LORA_HPP
#define EASY_LORA_HPP

#include "serial/serial.h"
#include <string_view>
#include "Envelope.pb.h"
#include "ModuleConfig.hpp"
#include "EasyLoRaExceptions.hpp"
#include "SuccessStatus.pb.h"
#include "IOCommons.hpp"

/// @brief Clase para la comunicación con el módulo EasyLoRa
class EasyLoRa : public IOCommons {
public:

    /// @brief Constructor base
    /// @param port Puerto USB en donde está conectado el módulo EasyloRa
    /// @throw PortDontOpen en caso de no poder abrir el puerto
    explicit EasyLoRa(std::string_view port);

    /// @brief Establece la configuración enviada
    /// @param config Configuración a enviar
    /// @throw SerializeError en caso de no poder serializar la configuración
    /// @throw WriteError en caso de no poder escribir en el puerto
    /// @throw SuccessDontReceived en caso de no recibir mensaje de confirmación de aplicación de cambios
    /// @throw DeserializeError en caso de no poder desearilzar el mensaje de confirmación
    /// @throw ModuleError en caso de error en el módulo
    void setConfiguration(const ModuleConfig &config);

    /// @brief Obtiene la configuración actual del módulo
    /// @return ModuleConfig con la configuración del módulo   
    /// @throw SerializeError en caso de no poder serializar la solicitud de configuración
    /// @throw WriteError en caso de no poder escribir en el puerto
    /// @throw SuccessDontReceived en caso de no recibir información
    /// @throw DeserializeError en caso de no poder desearizar el mensaje de confirmación o configuración
    /// @throw ModuleError en caso de error en el módulo
    [[nodiscard]]
    ModuleConfig getConfiguration() const noexcept;

    void sendMessage(std::string_view message) override;

    [[nodiscard]]
    std::string receiveMessage() override;

private:
    static constexpr uint16_t Default_Timeout_In_Ms{ 2000 };
    static constexpr uint8_t Size_Byte_Length{ 1 };
    static constexpr uint16_t Default_BaudRate{ 9600 };

    serial::Serial serialPort_m;
    ModuleConfig actualConfiguration_m;

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

    ModuleConfig getConfigurationFromModule();

    void syncBaudRate();
};

#endif // EASY_LORA_HPP