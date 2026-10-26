#pragma once

#include <vector>
#include <cstdint>
#include <stdexcept>

enum class CommandType : uint8_t
{
    ClearDisplay = 0x01,
    DrawPixel = 0x02,
    DrawLine = 0x03,
    DrawRectangle = 0x04,
    FillRectangle = 0x05,
    DrawEllipse = 0x06,
    FillEllipse = 0x07,

    DrawCircle = 0x08,
    FillCircle = 0x09,

    DrawRoundedRectangle = 0x0A,
    FillRoundedRectangle = 0x0B,

    DrawText = 0x0C,
    DrawImage = 0x0D,

    SetOrientation = 0x0E,
    GetWidth = 0x0F,
    GetHeight = 0x10
};

struct Command
{
    CommandType type;
    std::vector<int32_t> params;
};

class DisplayProtocolParser
{
public:
    Command Parse(const std::vector<uint8_t>& packet);

private:
    int16_t ReadInt16(
        const std::vector<uint8_t>& data,
        size_t pos);

    uint32_t ReadUInt32(
        const std::vector<uint8_t>& data,
        size_t pos);
};