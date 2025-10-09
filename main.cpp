// Este archivo es temporal, no está planeado dejarlo en el repositorio.
#include "serial_cpp/serial.h"
#include "EasyLoRa.hpp"
#include "ModuleConfig.hpp"
#include <thread>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std;

void sendAlive() {
    EasyLoRa lora{ "/dev/ttyACM0" };

    for (size_t i = 0; ; ++i) {
        lora.sendData("Alive");

        std::this_thread::sleep_for(std::chrono::milliseconds{40});
    }
}

[[noreturn]]
void getAlive() {
    EasyLoRa lora{ "/dev/ttyACM1" };

    while (true) {
        const auto message{ lora.receiveData() };

        cout << message.size() << message << '\n';
    }
}

int main() {
    EasyLoRa lora{ "/dev/ttyACM0" };

    std::this_thread::sleep_for(std::chrono::milliseconds{1000});

    ModuleConfig conf;

    conf.setAddressHighByte(0x19);

    lora.setConfiguration(conf, true);
}
