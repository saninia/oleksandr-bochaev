#include "CppUnitTest.h"
#include "../Project1/DisplayProtocol.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace DisplayProtocolTests
{
    TEST_CLASS(ProtocolTests)
    {
        TEST_METHOD(PacketTooShort)
        {
            DisplayProtocolParser parser;

            std::vector<uint8_t> packet =
            {
                0x01
            };

            Assert::ExpectException<std::runtime_error>(
                [&]()
                {
                    parser.Parse(packet);
                });
        }

        TEST_METHOD(UnknownCommand)
        {
            DisplayProtocolParser parser;

            std::vector<uint8_t> packet =
            {
                0x99,
                0x00,
                0x00
            };

            Assert::ExpectException<std::runtime_error>(
                [&]()
                {
                    parser.Parse(packet);
                });
        }
    public:

        TEST_METHOD(ParseGetWidth)
        {
            DisplayProtocolParser parser;

            std::vector<uint8_t> packet =
            {
                0x0F,
                0x00,
                0x00
            };

            Command cmd = parser.Parse(packet);

            Assert::AreEqual(
                (int)CommandType::GetWidth,
                (int)cmd.type);
        }

        TEST_METHOD(ParseGetHeight)
        {
            DisplayProtocolParser parser;

            std::vector<uint8_t> packet =
            {
                0x10,
                0x00,
                0x00
            };

            Command cmd = parser.Parse(packet);

            Assert::AreEqual(
                (int)CommandType::GetHeight,
                (int)cmd.type);
        }
    };
}