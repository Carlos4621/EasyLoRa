#ifndef EASY_LORA_HPP
#define EASY_LORA_HPP

#include "serial_cpp/serial.h"
#include <string_view>
#include "Envelope.pb.h"
#include "ModuleConfig.hpp"
#include "EasyLoRaExceptions.hpp"
#include "IOCommons.hpp"

/*
    TODO:

        - Seguir con las mejoras en los métodos

    - Hay un error raro que produce que no se pueda enviar datos a transmitir, requiere más investigación
        * pistas:
            - sucede al hacer ctrl + c en medio de una transmisión y querer volver a iniciar un programa
        * Posible problema:
            - El dispositivo sigue enviando mensajes debido al cierre incorrecto de la aplicación, provocando que al momento de llamar al constructor se
              no se reciba el mensaje con la configuración actual del módulo
*/

/// @brief Clase para la comunicación con el módulo EasyLoRa
class EasyLoRa : public IOCommons {
public:

    /// @brief Constructor base
    /// @param port Puerto USB en donde está conectado el módulo EasyloRa
    /// @throw PortDontOpen en caso de no poder abrir el puerto
    explicit EasyLoRa(std::string_view port);

    // TODO: Añadir la excepción lanzada si no se sincroniza

    /// @brief Establece la configuración enviada
    /// @param config Configuración a enviar
    /// @param syncWithReceiver Indica si se quiere sincronizar la configuración con el módulo oyente
    /// @throw SerializeError en caso de no poder serializar la configuración
    /// @throw WriteError en caso de no poder escribir en el puerto
    /// @throw SuccessDontReceived en caso de no recibir mensaje de confirmación de aplicación de cambios
    /// @throw DeserializeError en caso de no poder desearilzar el mensaje de confirmación
    /// @throw ModuleError en caso de error en el módulo
    void setConfiguration(const ModuleConfig &config, bool syncWithReceiver = false);

    /// @brief Obtiene la configuración actual del módulo
    /// @return ModuleConfig con la configuración del módulo   
    /// @throw SerializeError en caso de no poder serializar la solicitud de configuración
    /// @throw WriteError en caso de no poder escribir en el puerto
    /// @throw SuccessDontReceived en caso de no recibir información
    /// @throw DeserializeError en caso de no poder desearizar el mensaje de confirmación o configuración
    /// @throw ModuleError en caso de error en el módulo
    [[nodiscard]]
    ModuleConfig getConfiguration() const noexcept;

    /// @brief Envía datos para su transmisión mediante el módulo EasyLoRa
    /// @param message Datos a enviar
    void sendData(std::string_view message) override;

    /// @brief Recibe datos trasmitidos por otro dispositivo EasyLoRa
    /// @return Datos recibidos, puede estar vacío
    [[nodiscard]]
    std::string receiveData() override;

private:
    static constexpr uint16_t Default_Timeout_In_Ms{ 100 };
    static constexpr uint32_t Default_Configuration_Sync_Timeout_In_Ms{ Default_Timeout_In_Ms * 10 };

    static constexpr uint8_t Size_Byte_Length{ 1 };
    static constexpr uint32_t Default_BaudRate{ 115200 };

    serial_cpp::Serial serialPort_m;
    ModuleConfig actualConfiguration_m;

    void sendEnvelope(const Envelope& package);

    Envelope requestInformation(const Envelope& package);

    [[nodiscard]]
    std::string serializePackage(const Envelope& package);

    void writeToSerial(const std::string& data);

    std::string readFromSerial();

    [[nodiscard]]
    Envelope deserializeEnvelope(const std::string& data);

    void throwIfEnvelopeError(const Envelope& status);

    ModuleConfig getConfigurationFromModule();
};

#endif // EASY_LORA_HPP