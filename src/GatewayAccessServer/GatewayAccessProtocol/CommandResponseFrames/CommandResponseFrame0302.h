/**
 * \file      CommandResponseFrame0302.h
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

#ifndef COMMAND_RESPONSE_FRAME_0302_H
#define COMMAND_RESPONSE_FRAME_0302_H

#include "CommandResponseFrame.h"

class CommandResponseFrame0302: public CommandResponseFrame {
public:
    CommandResponseFrame0302(): CommandResponseFrame(0x0302) {}
    const std::vector<unsigned char> Serialize() const {
        std::vector<unsigned char> l_Buffer;
        l_Buffer.emplace_back(0x03);
        l_Buffer.emplace_back(0x02);
        l_Buffer.insert(l_Buffer.end(), m_Payload.begin(), m_Payload.end());
        return l_Buffer;
    }
    const std::vector<unsigned char> GetPayload() const { return m_Payload; }
    std::vector<unsigned char> m_Payload;
};


#endif // COMMAND_RESPONSE_FRAME_0302_H
