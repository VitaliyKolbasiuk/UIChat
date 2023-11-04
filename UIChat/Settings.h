#pragma once

#include <cereal/types/string.hpp>
#include <array>

class Settings{
    std::string m_historyFileName;

public:
    int m_version = 0;
    std::array<uint8_t, 32> m_userKey;      // private key
    std::array<uint8_t, 32> m_userPublicKey;
    std::array<uint8_t, 32> m_deviceKey;
    std::string m_username;

    Settings(std::string fileName);

    void loadSettings(const std::string& fileName);

    void saveSettings(const std::string& fileName);

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar( m_version );
        ar( m_userKey, m_deviceKey );
        ar( m_username, m_historyFileName );
    }
};
