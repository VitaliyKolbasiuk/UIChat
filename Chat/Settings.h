#pragma once

#include <cereal/types/string.hpp>

class Settings{
    std::string m_historyFileName;

public:
    std::string m_username;

    Settings(std::string fileName);

    void loadSettings(const std::string& fileName);

    void saveSettings(const std::string& fileName);

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar( m_username, m_historyFileName );
    }
};
