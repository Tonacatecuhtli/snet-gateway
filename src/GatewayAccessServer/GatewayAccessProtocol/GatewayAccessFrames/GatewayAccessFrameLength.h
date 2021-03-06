/**
 * \file      GatewayAccessFrameLength.h
 * \brief     
 * \author    Florian Evers, florian-evers@gmx.de
 * \copyright GNU Public License version 3.
 *
 * The HDLC Deamon implements the HDLC protocol to easily talk to devices connected via serial communications.
 * Copyright (C) 2016  Florian Evers, florian-evers@gmx.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GATEWAY_ACCESS_FRAME_LENGTH_H
#define GATEWAY_ACCESS_FRAME_LENGTH_H

#include "GatewayAccessFrame.h"
#include <memory>
#include <assert.h>

class GatewayAccessFrameLength: public GatewayAccessFrame {
public:
    static GatewayAccessFrameLength Create(const std::vector<unsigned char> &a_Payload) {
        assert(a_Payload.size() < 4096);
        GatewayAccessFrameLength l_GatewayAccessFrameLength;
        l_GatewayAccessFrameLength.m_Buffer = a_Payload;
        return l_GatewayAccessFrameLength;
    }

    static std::shared_ptr<GatewayAccessFrameLength> CreateDeserializedFrame() {
        auto l_GatewayAccessFrameLength(std::shared_ptr<GatewayAccessFrameLength>(new GatewayAccessFrameLength));
        l_GatewayAccessFrameLength->m_eDeserialize = DESERIALIZE_HEADER;
        l_GatewayAccessFrameLength->m_BytesRemaining = 2; // Next: consume the frame identifier and the length field
        return l_GatewayAccessFrameLength;
    }

    const std::vector<unsigned char>& GetPayload() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        return m_Buffer;
    }

private:
    // Private CTOR
    GatewayAccessFrameLength(): m_eDeserialize(DESERIALIZE_FULL) {
    }

    // Methods
    E_GATEWAY_ACCESS_FRAME GetGatewayAccessFrameType() const { return GATEWAY_ACCESS_FRAME_LENGTH; }

    // Serializer
    const std::vector<unsigned char> Serialize() const {
        assert(m_eDeserialize == DESERIALIZE_FULL);
        std::vector<unsigned char> l_Buffer;
        uint16_t l_NbrOfBytes = m_Buffer.size();
        l_Buffer.emplace_back(uint8_t(GATEWAY_ACCESS_FRAME_LENGTH) + ((l_NbrOfBytes >> 8) & 0x0F));
        l_Buffer.emplace_back(l_NbrOfBytes & 0xFF);
        l_Buffer.insert(l_Buffer.end(), m_Buffer.begin(), m_Buffer.end());
        return l_Buffer;
    }

    // Deserializer
    bool Deserialize() {
        // All requested bytes are available
        switch (m_eDeserialize) {
        case DESERIALIZE_HEADER: {
            // Deserialize the header
            assert(m_Buffer.size() == 2);
            if ((m_Buffer[0] & GATEWAY_ACCESS_FRAME_MASK) != GATEWAY_ACCESS_FRAME_LENGTH) {
                // Error!
                m_eDeserialize = DESERIALIZE_ERROR;
                return false;
            } else {
                m_BytesRemaining = ((uint16_t(m_Buffer[0] & 0x0F) << 8) + m_Buffer[1]);
                m_Buffer.clear();
                m_eDeserialize = DESERIALIZE_DATA;
                if (m_BytesRemaining == 0) {
                    // An empty data frame?!... Ok...
                    m_eDeserialize = DESERIALIZE_FULL;
                } // if
            } // else

            break;
        }
        case DESERIALIZE_DATA: {
            // Read of payload completed
            m_eDeserialize = DESERIALIZE_FULL;
            break;
        }
        case DESERIALIZE_ERROR:
        case DESERIALIZE_FULL:
        default:
            assert(false);
        } // switch

        // No error
        return true;
    }

    // Members
    typedef enum {
        DESERIALIZE_ERROR  = 0,
        DESERIALIZE_HEADER = 1,
        DESERIALIZE_DATA   = 2,
        DESERIALIZE_FULL   = 3
    } E_DESERIALIZE;
    E_DESERIALIZE m_eDeserialize;
};

#endif // GATEWAY_ACCESS_FRAME_LENGTH_H
