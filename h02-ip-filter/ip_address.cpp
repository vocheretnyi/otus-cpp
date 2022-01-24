#include "ip_address.hpp"

#include <sstream>
#include <tuple>

IPAddress::IPAddress() : bytes() {
}

IPAddress::IPAddress(const std::string& ipAddressString) {
    *this = ParseString(ipAddressString);
}

bool IPAddress::operator<(const IPAddress& other) const {
    return bytes < other.bytes;
}

uint8_t& IPAddress::operator[](size_t index) {
    return bytes[index];
}

const uint8_t& IPAddress::operator[](size_t index) const {
    return bytes[index];
}

IPAddress IPAddress::ParseString(const std::string& ipAddressString) {
    IPAddress ipAddress;
    std::stringstream ipStringStream(ipAddressString);

    for (size_t i = 0; i < kIP_BYTES_COUNT; ++i) {
        // Streams wrongly works with (u)int8_t types, they're assuming that they are chars.
        // So we need to use uint16_t here.
        //
        uint16_t byte;
        ipStringStream >> byte;
        ipAddress[i] = static_cast<uint8_t>(byte);

        // Read '.'.
        //
        std::ignore = ipStringStream.get();
    }

    return ipAddress;
}

IPAddress::IPAddress(std::array<uint8_t, kIP_BYTES_COUNT> bytesIn) : bytes(bytesIn) {
}

std::ostream& operator<<(std::ostream& out, const IPAddress& ip) {
    for (size_t i = 0; i < IPAddress::kIP_BYTES_COUNT; ++i) {
        // Streams wrongly works with (u)int8_t types, they're assuming that they are chars.
        // So we need to use uint16_t here.
        //
        out << static_cast<uint16_t>(ip[i]);
        if (i < IPAddress::kIP_BYTES_COUNT - 1) {
            out << '.';
        }
    }
    return out;
}
