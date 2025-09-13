// Este archivo es temporal, no está planeado dejarlo en el repositorio.
#include "serial/serial.h"
#include "EasyLoRa.hpp"
#include "ModuleConfig.hpp"
#include <thread>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

[[noreturn]]
void sendAlive() {
    EasyLoRa lora{ "/dev/ttyACM0" };

    while (true) {
        lora.sendData("Alive");

        std::this_thread::sleep_for(std::chrono::milliseconds{1});
    }
}

[[noreturn]]
void getAlive() {
    EasyLoRa lora{ "/dev/ttyACM1" };

    while (true) {
        const auto message{ lora.receiveData() };

        cout << message.size() << message << '\n';

        std::this_thread::sleep_for(std::chrono::seconds{1});
    }
}

int main() {
    EasyLoRa test{ "/dev/ttyACM0" };

    ModuleConfig config;
    config.setAddressHighByte(0x99)
          .setAddressLowByte(0x98)
          .setChannel(69);

    test.setConfiguration(config);

    cout << test.getConfiguration().toLegibleString();
}
