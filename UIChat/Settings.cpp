#pragma once

#include "Settings.h"
#include "Utils.h"

#define FIXEDINT_H_INCLUDED


//#include "ed25519.h"
//#include "ge.h"
//#include "sc.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/array.hpp>
#include <fstream>
#include <qDebug>

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
        generateRandomKey(m_userKey);
        generateRandomKey(m_deviceKey);
        if (m_version == 0)
        {
            m_version = 1;
        }
        else
        {
            // TO DO
            qDebug() << "Settings corrupted";
            exit(-1);
        }
        saveSettings("setings.bin");
    }

//    unsigned char scalar[32];
//    ed25519_create_seed(scalar);
//    ed25519_create_keypair(&m_userKey[0], &m_userPublicKey[0], scalar);

//    // TEST CASE
//    unsigned char signature[64];
//    ed25519_sign(signature, (unsigned char*)"message", 7, &m_userPublicKey[0], &m_userKey[0]);

//    /* verify the signature */
//    if (ed25519_verify(signature, (unsigned char*)"message", 7, &m_userPublicKey[0])) {
//        qDebug() << "valid signature";
//    } else {
//        qDebug() << "invalid signature";
//    }
}

void Settings::saveSettings(const std::string& fileName)
{
    try{
        std::ofstream os(fileName, std::ios::binary);
        cereal::BinaryOutputArchive archive( os );
        archive(*this);
    }
    catch(std::exception& ex){
        qDebug() << "Save Setting exception: " << ex.what();
    }
}
