#pragma once

#include "Settings.h"
#include "Utils.h"

#define FIXEDINT_H_INCLUDED

#include <cereal/archives/binary.hpp>
#include <cereal/types/array.hpp>
#include <fstream>
#include <qDebug>

#include "ed25519/src/ed25519.h"

Settings::Settings(std::string fileName) : m_historyFileName(fileName) {
    loadSettings(fileName);

}

void Settings::loadSettings(const std::string& fileName)
{
    try{
        std::ifstream ios(fileName + "_keys", std::ios::binary);
        cereal::BinaryInputArchive archive( ios );
        archive(m_keyPair);
    }
    catch(...){
        //generateRandomKey(m_userKey);
        unsigned char scalar[32];
        ed25519_create_seed(scalar);
        ed25519_create_keypair(&m_keyPair.m_publicKey[0], &m_keyPair.m_privateKey[0], scalar);

        qDebug() << "!!!!!Keypair generated";

        std::ofstream os(fileName + "_keys", std::ios::binary);
        cereal::BinaryOutputArchive archive( os );
        archive(m_keyPair);
    }

    // TEST CASE
    unsigned char signature[64];
    ed25519_sign(signature, (unsigned char*)"message", 7, &m_keyPair.m_publicKey[0], &m_keyPair.m_privateKey[0]);

    /* verify the signature */
    if (ed25519_verify(signature, (unsigned char*)"message", 7, &m_keyPair.m_publicKey[0])) {
        qDebug() << "valid signature";
    } else {
        qDebug() << "invalid signature";
    }

    try{
        std::ifstream ios(fileName, std::ios::binary);
        cereal::BinaryInputArchive archive( ios );
        archive(*this);
    }
    catch(...){
        //generateRandomKey(m_userKey);
        unsigned char scalar[32];
        ed25519_create_seed(scalar);
        ed25519_create_keypair(&m_keyPair.m_publicKey[0], &m_keyPair.m_privateKey[0], scalar);

        qDebug() << "!!!!!Keypair generated";

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
        saveSettings(fileName);
    }

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
