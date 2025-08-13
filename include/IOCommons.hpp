#ifndef IO_COMMONS_HEADER
#define IO_COMMONS_HEADER

#include <span>
#include <cstdint>
#include <string>

class IOCommons {
public:
    virtual void sendData(std::string_view data) = 0;

    virtual std::string receiveData() = 0;
};

#endif //!IO_COMMONS_HEADER