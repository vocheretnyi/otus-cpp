#include <gtest/gtest.h>
#include <openssl/md5.h>

#include <iomanip>
#include <fstream>
#include <sstream>

#include "ip_address.hpp"
#include "ip_filter.hpp"

TEST(IPAddress, ParseString) {
    ASSERT_EQ(IPAddress::ParseString("192.168.0.1"),
              (IPAddress{std::array<uint8_t, IPAddress::kIP_BYTES_COUNT>{192, 168, 0, 1}}));
    ASSERT_EQ(IPAddress::ParseString("255.255.255.255"),
              (IPAddress{std::array<uint8_t, IPAddress::kIP_BYTES_COUNT>{255, 255, 255, 255}}));
    ASSERT_EQ(IPAddress::ParseString("0.0.0.0"),
              (IPAddress{std::array<uint8_t, IPAddress::kIP_BYTES_COUNT>{0, 0, 0, 0}}));
}

TEST(IPFilter, Solve) {
    std::fstream input(std::string(RESOURCES_PATH) + "/ip_filter.tsv");
    std::ostringstream os;
    Solve(input, os);

    auto md5Hash = MD5(reinterpret_cast<const unsigned char*>(os.str().c_str()), os.str().size(), nullptr);

    // Converting byte array to hex string.
    //
    std::stringstream hashStrStream;
    hashStrStream << std::hex;
    for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        hashStrStream << std::setw(2) << std::setfill('0') << static_cast<int>(md5Hash[i]);
    }

    ASSERT_EQ(hashStrStream.str(), "24e7a7b2270daee89c64d3ca5fb3da1a");
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
