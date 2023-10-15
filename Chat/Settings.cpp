#pragma once

#include "Settings.h"

#include <cereal/archives/binary.hpp>
#include <fstream>

Settings::Settings(std::string fileName) : m_historyFileName(fileName) {
    loadSettings(fileName);
}

void Settings::loadSettings(const std::string& fileName)
{
    try{
        std::ifstream ios(fileName, std::ios::binary);
        cereal::BinaryInputArchive archive( ios );
        archive(*this);
    }
    catch(...){

    }
}

void Settings::saveSettings(const std::string& fileName)
{
    std::ofstream os(fileName, std::ios::binary);
    cereal::BinaryOutputArchive archive( os );
    archive(*this);
}
