#include "ModuleConfig.hpp"

class ModuleConfig::InvalidChannel : public std::exception {
public:
    explicit InvalidChannel(uint8_t channel)
    : message_m{ std::string{"Channel must be lower than "} + std::to_string(Max_Channel) + ". Actual: " + std::to_string(channel) }
    {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return message_m.data();
    }

private:
    std::string message_m;
};

ModuleConfig& ModuleConfig::setAddressHighByte(uint8_t highAddressByte) noexcept {
    addressHighByte_m = highAddressByte;
    return *this;
}

ModuleConfig& ModuleConfig::setAddressLowByte(uint8_t lowAddressByte) noexcept {
    addressLowByte_m = lowAddressByte;
    return *this;
}

ModuleConfig& ModuleConfig::setNetId(uint8_t NETID) noexcept {
    netId_m = NETID;
    return *this;
}

ModuleConfig& ModuleConfig::setChannel(uint8_t channel) {
    if (channel > Max_Channel) {
        throw InvalidChannel{ channel };
    }

    channel_m = channel;
    return *this;
}

ModuleConfig& ModuleConfig::setEnableRSSI(bool enable) noexcept {
    enableRSSI_m = enable;
    return *this;
}

ModuleConfig& ModuleConfig::setRSSIByteEnabled(bool enable) noexcept {
    rssiByteEnabled_m = enable;
    return *this;
}

ModuleConfig& ModuleConfig::setEnableAbnormalLog(bool enable) noexcept {
    enableAbnormalLog_m = enable;
    return *this;
}

ModuleConfig& ModuleConfig::setEnableFixedTransmitionMode(bool enable) noexcept {
    enableFixedTransmitionMode_m = enable;
    return *this;
}

ModuleConfig& ModuleConfig::setEnableRepeaterMode(bool enable) noexcept {
    enableRepeaterMode_m = enable;
    return *this;
}

ModuleConfig& ModuleConfig::setEnableLBT(bool enable) noexcept {
    enableLBT_m = enable;
    return *this;
}

ModuleConfig& ModuleConfig::setEnableWORMode(bool enable) noexcept {
    enableWORMode_m = enable;
    return *this;
}

ModuleConfig& ModuleConfig::setAirDataRate(AirDataRate airDataRate) noexcept {
    airDataRate_m = airDataRate;
    return *this;
}

ModuleConfig& ModuleConfig::setSubpacketLength(SubpacketLenght subPacketLenght) noexcept {
    subpacketLength_m = subPacketLenght;
    return *this;
}

ModuleConfig& ModuleConfig::setSerialPortParityByte(SerialPortParityByte parityByte) noexcept {
    serialPortParityByte_m = parityByte;
    return *this;
}

ModuleConfig& ModuleConfig::setWorCycle(WORCycle cycle) noexcept {
    worCycle_m = cycle;
    return *this;
}

ModuleConfig& ModuleConfig::setUartBaudRate(UARTBaudRate baudRate) noexcept {
    uartBaudRate_m = baudRate;
    return *this;
}

uint8_t ModuleConfig::getAddressHighByte() const noexcept {
    return addressHighByte_m;
}

uint8_t ModuleConfig::getAddressLowByte() const noexcept {
    return addressLowByte_m;
}

uint8_t ModuleConfig::getNetId() const noexcept {
    return netId_m;
}

uint8_t ModuleConfig::getChannel() const noexcept {
    return channel_m;
}

bool ModuleConfig::isRSSIEnabled() const noexcept {
    return enableRSSI_m;
}

bool ModuleConfig::isRSSIByteEnabled() const noexcept {
    return rssiByteEnabled_m;
}

bool ModuleConfig::isAbnormalLogEnabled() const noexcept {
    return enableAbnormalLog_m;
}

bool ModuleConfig::isFixedTransmitionModeEnabled() const noexcept {
    return enableFixedTransmitionMode_m;
}

bool ModuleConfig::isRepeaterModeEnabled() const noexcept {
    return enableRepeaterMode_m;
}

bool ModuleConfig::isLBTEnabled() const noexcept {
    return enableLBT_m;
}

bool ModuleConfig::isWORModeEnabled() const noexcept {
    return enableWORMode_m;
}

AirDataRate ModuleConfig::getAirDataRate() const noexcept {
    return airDataRate_m;
}

SubpacketLenght ModuleConfig::getSubpacketLength() const noexcept {
    return subpacketLength_m;
}

SerialPortParityByte ModuleConfig::getSerialPortParityByte() const noexcept {
    return serialPortParityByte_m;
}

WORCycle ModuleConfig::getWorCycle() const noexcept {
    return worCycle_m;
}

UARTBaudRate ModuleConfig::getUartBaudRate() const noexcept {
    return uartBaudRate_m;
}

ModuleConfiguration ModuleConfig::toProtobuf() const noexcept {
    ModuleConfiguration pbConfig;
    
    pbConfig.set_addresshighbyte(addressHighByte_m);
    pbConfig.set_addresslowbyte(addressLowByte_m);
    pbConfig.set_netid(netId_m);
    pbConfig.set_channel(channel_m);
    
    pbConfig.set_enablerssi(enableRSSI_m);
    pbConfig.set_rssibyte(rssiByteEnabled_m);
    pbConfig.set_enableabnormallog(enableAbnormalLog_m);
    pbConfig.set_enablefixedtransmitionmode(enableFixedTransmitionMode_m);
    pbConfig.set_enablerepeatermode(enableRepeaterMode_m);
    pbConfig.set_enablelbt(enableLBT_m);
    pbConfig.set_enablewormode(enableWORMode_m);
    
    pbConfig.set_airdatarate(airDataRate_m);
    pbConfig.set_subpacketlenght(subpacketLength_m);
    pbConfig.set_serialportparitybyte(serialPortParityByte_m);
    pbConfig.set_worcycle(worCycle_m);
    pbConfig.set_uartbaudrate(uartBaudRate_m);
    
    return pbConfig;
}

ModuleConfig ModuleConfig::fromProtobuf(const ModuleConfiguration& pbConfig) noexcept {
    ModuleConfig config;

    config.addressHighByte_m = pbConfig.addresshighbyte();
    config.addressLowByte_m = pbConfig.addresslowbyte();
    config.netId_m = pbConfig.netid();
    config.channel_m = pbConfig.channel();
    
    config.enableRSSI_m = pbConfig.enablerssi();
    config.rssiByteEnabled_m = pbConfig.rssibyte();
    config.enableAbnormalLog_m = pbConfig.enableabnormallog();
    config.enableFixedTransmitionMode_m = pbConfig.enablefixedtransmitionmode();
    config.enableRepeaterMode_m = pbConfig.enablerepeatermode();
    config.enableLBT_m = pbConfig.enablelbt();
    config.enableWORMode_m = pbConfig.enablewormode();
    
    config.airDataRate_m = pbConfig.airdatarate();
    config.subpacketLength_m = pbConfig.subpacketlenght();
    config.serialPortParityByte_m = pbConfig.serialportparitybyte();
    config.worCycle_m = pbConfig.worcycle();
    config.uartBaudRate_m = pbConfig.uartbaudrate();
    
    return config;
}

std::string ModuleConfig::toLegibleString() const noexcept {
    std::ostringstream output;

    output <<
    "High address: " << std::hex << static_cast<uint16_t>(addressHighByte_m) << '\n' <<
    "Low address: " << static_cast<uint16_t>(addressLowByte_m) << '\n' <<
    "NETID: " << std::dec << static_cast<uint16_t>(netId_m) << '\n' <<
    "UART bps: " << std::bitset<3>(uartBaudRate_m) << '\n' <<
    "Parity byte: " << std::bitset<2>(serialPortParityByte_m) << '\n' <<
    "Air rate: " << std::bitset<3>(airDataRate_m) << '\n' <<
    "Subpacket: " << std::bitset<2>(subpacketLength_m) << '\n' <<
    "RSSI noise: " << rssiByteEnabled_m << '\n' <<
    "Abnormal register: " << enableAbnormalLog_m << '\n' <<
    "Channel: " << std::hex << static_cast<uint16_t>(channel_m) << '\n' <<
    "Enable RSSI: " << enableRSSI_m << '\n' <<
    "Transmition method: " << enableFixedTransmitionMode_m << '\n' <<
    "Relay Mode: " << enableRepeaterMode_m << '\n' <<
    "LBT enabled: " << enableLBT_m << '\n' <<
    "WOR mode: " << enableWORMode_m << '\n' <<
    "WOR cycle: " << std::bitset<3>(worCycle_m);

    return output.str();
}
