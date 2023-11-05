#pragma once

#include <stdint.h>
#include <cereal/archives/binary.hpp>
#include <cereal/types/array.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>

struct Seed{
    std::array<uint8_t, 64> m_data;
    template <class Archive>
    void serialize( Archive & ar )
    {
        ar( m_data );
    }
};

struct ConnectRequest{
    std::array<uint8_t, 32> m_publicKey;
    std::string m_nickname;
    std::array<uint8_t, 64> m_sign;

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar( m_publicKey, m_nickname, m_sign );
    }
};



