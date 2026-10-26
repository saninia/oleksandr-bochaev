#include "DisplayProtocol.h"

int16_t DisplayProtocolParser::ReadInt16(
    const std::vector<uint8_t>& data,
    size_t pos)
{
    return static_cast<int16_t>(
        data[pos] |
        (data[pos + 1] << 8));
}

uint32_t DisplayProtocolParser::ReadUInt32(
    const std::vector<uint8_t>& data,
    size_t pos)
{
    return static_cast<uint32_t>(
        data[pos] |
        (data[pos + 1] << 8) |
        (data[pos + 2] << 16) |
        (data[pos + 3] << 24));
}

Command DisplayProtocolParser::Parse(
    const std::vector<uint8_t>& packet)
{
    if (packet.size() < 3)
    {
        throw std::runtime_error("Packet too short");
    }

    CommandType command =
        static_cast<CommandType>(packet[0]);

    uint16_t length =
        packet[1] |
        (packet[2] << 8);

    if (length != packet.size() - 3)
    {
        throw std::runtime_error("Invalid length");
    }

    Command result;
    result.type = command;

    switch (command)
    {
    case CommandType::ClearDisplay:
    {
        if (length != 4)
            throw std::runtime_error("Invalid packet");

        result.params.push_back(
            ReadUInt32(packet, 3));

        break;
    }

    case CommandType::DrawPixel:
    {
        if (length != 8)
            throw std::runtime_error("Invalid packet");

        result.params =
        {
            ReadInt16(packet,3),
            ReadInt16(packet,5),
            (int32_t)ReadUInt32(packet,7)
        };

        break;
    }

    case CommandType::DrawLine:
    {
        if (length != 12)
            throw std::runtime_error("Invalid packet");

        result.params =
        {
            ReadInt16(packet,3),
            ReadInt16(packet,5),
            ReadInt16(packet,7),
            ReadInt16(packet,9),
            (int32_t)ReadUInt32(packet,11)
        };

        break;
    }

    case CommandType::DrawRectangle:
    case CommandType::FillRectangle:
    case CommandType::DrawEllipse:
    case CommandType::FillEllipse:
    {
        if (length != 12)
            throw std::runtime_error("Invalid packet");

        result.params =
        {
            ReadInt16(packet,3),
            ReadInt16(packet,5),
            ReadInt16(packet,7),
            ReadInt16(packet,9),
            (int32_t)ReadUInt32(packet,11)
        };

        break;
    }

    case CommandType::DrawCircle:
    case CommandType::FillCircle:
    {
        if (length != 10)
            throw std::runtime_error("Invalid packet");

        result.params =
        {
            ReadInt16(packet,3),
            ReadInt16(packet,5),
            ReadInt16(packet,7),
            (int32_t)ReadUInt32(packet,9)
        };

        break;
    }

    case CommandType::DrawRoundedRectangle:
    case CommandType::FillRoundedRectangle:
    {
        if (length != 14)
            throw std::runtime_error("Invalid packet");

        result.params =
        {
            ReadInt16(packet,3),
            ReadInt16(packet,5),
            ReadInt16(packet,7),
            ReadInt16(packet,9),
            ReadInt16(packet,11),
            (int32_t)ReadUInt32(packet,13)
        };

        break;
    }

    case CommandType::SetOrientation:
    {
        if (length != 1)
            throw std::runtime_error("Invalid packet");

        uint8_t orientation = packet[3];

        if (orientation > 3)
        {
            throw std::runtime_error("Invalid orientation");
        }

        result.params.push_back(orientation);

        break;
    }

    case CommandType::GetWidth:
    case CommandType::GetHeight:
    {
        if (length != 0)
            throw std::runtime_error("Invalid packet");

        break;
    }

    default:
    {
        throw std::runtime_error("Unknown command");
    }
    }

    return result;
}
