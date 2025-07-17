#ifndef MODULE_CONFIG_HPP
#define MODULE_CONFIG_HPP

#include "Envelope.pb.h"
#include <stdexcept>
#include <sstream>
#include <bitset>

class ModuleConfig {
public:

    class InvalidChannel;

    ModuleConfig() = default;

    ModuleConfig& setAddressHighByte(uint8_t highAddress) noexcept;
    ModuleConfig& setAddressLowByte(uint8_t lowAddress) noexcept;
    ModuleConfig& setNetId(uint8_t NETID) noexcept;
    ModuleConfig& setChannel(uint8_t channel);
    
    ModuleConfig& setEnableRSSI(bool enable) noexcept;
    ModuleConfig& setRSSIByteEnabled(bool enable) noexcept;
    ModuleConfig& setEnableAbnormalLog(bool enable) noexcept;
    ModuleConfig& setEnableFixedTransmitionMode(bool enable) noexcept;
    ModuleConfig& setEnableRepeaterMode(bool enable) noexcept;
    ModuleConfig& setEnableLBT(bool enable) noexcept;
    ModuleConfig& setEnableWORMode(bool enable) noexcept;
    
    ModuleConfig& setAirDataRate(AirDataRate airDataRate) noexcept;
    ModuleConfig& setSubpacketLength(SubpacketLenght subPacketLength) noexcept;
    ModuleConfig& setSerialPortParityByte(SerialPortParityByte parityByte) noexcept;
    ModuleConfig& setWorCycle(WORCycle WORCycle) noexcept;
    ModuleConfig& setUartBaudRate(UARTBaudRate baudRate) noexcept;

    uint8_t getAddressHighByte() const noexcept;
    uint8_t getAddressLowByte() const noexcept;
    uint8_t getNetId() const noexcept;
    uint8_t getChannel() const noexcept;
    
    bool isRSSIEnabled() const noexcept;
    bool isRSSIByteEnabled() const noexcept;
    bool isAbnormalLogEnabled() const noexcept;
    bool isFixedTransmitionModeEnabled() const noexcept;
    bool isRepeaterModeEnabled() const noexcept;
    bool isLBTEnabled() const noexcept;
    bool isWORModeEnabled() const noexcept;
    
    AirDataRate getAirDataRate() const noexcept;
    SubpacketLenght getSubpacketLength() const noexcept;
    SerialPortParityByte getSerialPortParityByte() const noexcept;
    WORCycle getWorCycle() const noexcept;
    UARTBaudRate getUartBaudRate() const noexcept;

    ModuleConfiguration toProtobuf() const noexcept;
    static ModuleConfig fromProtobuf(const ModuleConfiguration& pbConfig) noexcept;

    [[nodiscard]]
    std::string toLegibleString() const noexcept;

private:
    static constexpr uint8_t Max_Channel{ 83 };

    uint8_t addressHighByte_m{ 0 };
    uint8_t addressLowByte_m{ 0 };
    uint8_t netId_m{ 0 };
    uint8_t channel_m{ 0x17 };

    bool enableRSSI_m{ false };
    bool rssiByteEnabled_m{ false };
    bool enableAbnormalLog_m{ false };
    bool enableFixedTransmitionMode_m{ false };
    bool enableRepeaterMode_m{ false };
    bool enableLBT_m{ false };
    bool enableWORMode_m{ false };

    AirDataRate airDataRate_m{ AirDataRate::AirRate_2400_BPS };
    SubpacketLenght subpacketLength_m{ SubpacketLenght::Bytes_Lenght_240 };
    SerialPortParityByte serialPortParityByte_m{ SerialPortParityByte::Byte_8N1 };
    WORCycle worCycle_m{ WORCycle::WORCycle_500_ms };
    UARTBaudRate uartBaudRate_m{ UARTBaudRate::UART_9600_BPS };
};

#endif // MODULE_CONFIG_HPP