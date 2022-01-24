#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <iostream>

#include "ip_address.hpp"
#include "ip_filter.hpp"

std::vector<IPAddress> ReadIPsFromInput(std::istream& in) {
    std::vector<IPAddress> ipAddresses;
    std::string line;
    while (getline(in, line)) {
        std::string part;
        std::stringstream lineStream(line);
        std::vector<std::string> parts;
        while (getline(lineStream, part, '\t')) {
            parts.push_back(part);
        }
        ipAddresses.emplace_back(parts[0]);
    }
    return ipAddresses;
}

void PrintIpAddressesList(std::ostream& out, const std::vector<IPAddress>& ipAddresses) {
    for (auto& ip : ipAddresses) {
        out << ip << "\n";
    }
}

void Solve(std::istream& in, std::ostream& out) {
    auto ipAddresses = ReadIPsFromInput(in);

    sort(ipAddresses.rbegin(), ipAddresses.rend());

    auto filterAndPrint = [&ipAddresses, &out](auto predicate) {
        std::vector<IPAddress> buf;
        std::copy_if(ipAddresses.cbegin(), ipAddresses.cend(), std::back_inserter(buf), predicate);
        PrintIpAddressesList(out, buf);
    };

    PrintIpAddressesList(out, ipAddresses);

    filterAndPrint([](const IPAddress& ip) {
        return ip.bytes[0] == 1;
    });

    filterAndPrint([](const IPAddress& ip) {
        return ip.bytes[0] == 46 && ip.bytes[1] == 70;
    });

    filterAndPrint([](const IPAddress& ip) {
        return std::any_of(ip.bytes.cbegin(), ip.bytes.cend(), [](auto byte) {
            return byte == 46;
        });
    });
}
