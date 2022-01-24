#pragma once

#include <string>
#include <array>

struct IPAddress {
    static constexpr size_t kIP_BYTES_COUNT = 4;

    IPAddress();

    explicit IPAddress(std::array<uint8_t, kIP_BYTES_COUNT> bytesIn);

    explicit IPAddress(const std::string& ipAddressString);

    static IPAddress ParseString(const std::string& ipAddressString);

    // C++20 features (default comparisons operators).
    //
    bool operator==(const IPAddress&) const = default;

    bool operator!=(const IPAddress&) const = default;

    bool operator<(const IPAddress& other) const;

    uint8_t& operator[](size_t index);

    const uint8_t& operator[](size_t index) const;

    std::array<uint8_t, kIP_BYTES_COUNT> bytes;
};

std::ostream& operator<<(std::ostream& out, const IPAddress& ip);
