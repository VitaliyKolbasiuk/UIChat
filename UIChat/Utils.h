#pragma once

#include <iostream>
#include <random>
#include <string>
#include <array>
#include <sstream>
#include <iomanip>

template<size_t N>
void generateRandomKey(std::array<uint8_t, N>& key)
{
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_int_distribution<int> distribution(65, 90);  // ASCII range for uppercase letters

    for (size_t i = 0; i < N; ++i) {
        key[i] = static_cast<uint8_t>(distribution(generator));
    }
}

template<size_t N>
std::string arrayToHexString(const std::array<uint8_t, N>& arr)
{
    std::stringstream ss;
    ss << std::hex << std::setfill('0');
    for (const auto& element : arr) {
        ss << std::setw(2) << static_cast<int>(element);
    }
    return ss.str();
}

