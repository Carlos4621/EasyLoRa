#ifndef EASY_LORA_EXCEPTIONS_HPP
#define EASY_LORA_EXCEPTIONS_HPP

#include <exception>
#include <string>

class PortDontOpen : public std::exception {
public:
    explicit PortDontOpen(std::string_view port) 
    : message_m{ std::string{"The port: "} + port.data() + " is not open" }
    {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return message_m.data();
    }

private:
    std::string message_m;
};

class WriteError : public std::exception {
public:
    WriteError(size_t written, size_t expected)
    : message_m{ "Write incomplete: " + std::to_string(written) + 
                "/" + std::to_string(expected) + " bytes written" }
    {}
    
    [[nodiscard]]
    const char* what() const noexcept override {
        return message_m.data();
    }
    
private:
    std::string message_m;
};

class SerializeError : public std::exception {
public:
    [[nodiscard]]
    const char* what() const noexcept override {
        return "The package cant be serialized";
    }
};

class ModuleError : public std::exception {
public:
    explicit ModuleError(std::string_view message) 
    : message_m{ "Module sent error: " + std::string{message}}
    {}

    [[nodiscard]]
    const char* what() const noexcept override {
        return message_m.data();
    }

private:
    std::string message_m;

};

class DeserializeError : public std::exception {
public:
    [[nodiscard]]
    const char* what() const noexcept override {
        return "The package cant be deserialized";
    }
};

class SuccessDontReceived : public std::exception {
public:
    [[nodiscard]]
    const char* what() const noexcept override {
        return "Succes message dont received";
    }
};

#endif // !EASY_LORA_EXCEPTIONS_HPP