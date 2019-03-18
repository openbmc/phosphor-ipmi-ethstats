#include "ethstats.hpp"
#include "handler_mock.hpp"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#define MAX_IPMI_BUFFER 64

using ::testing::Return;

namespace ethstats
{

TEST(EthStatsTest, InvalidStatReturnsFailure)
{
    // Verify that the enum of valid statistic IDs is checked.
}

TEST(EthStatsTest, InvalidIpmiPacketSize)
{
    // An IPMI packet for this command has a minimum length.
}

TEST(EthStatsTest, InvalidIpmiPacketContents)
{
    // The packet has a name length and name contents, if the name length is
    // invalid, it should fail.
}

TEST(EthStatsTest, NameHasIllegalCharacters)
{
    // The interface name cannot have slashes.
}

TEST(EthStatsTest, InavlidNameOrField)
{
    // The handler returns failure on the input validity check.
}

TEST(EthStatsTest, EverythingHappy)
{
    std::string ifName = "eth0";
    struct EthStatRequest requestStruct;
    requestStruct.statId = EthernetStatisticsIds::RX_BYTES;
    requestStruct.if_name_len = ifName.length();

    std::vector<std::uint8_t> request(sizeof(requestStruct) + ifName.length());
    std::memcpy(request.data(), &requestStruct, sizeof(requestStruct));
    std::memcpy(&request[sizeof(requestStruct)], ifName.c_str(),
                ifName.length());

    size_t dataLen = request.size();
    std::uint8_t reply[MAX_IPMI_BUFFER];

    StatRequest expectedStructure(ifName, "rx_bytes");

    HandlerMock hMock;
    EXPECT_CALL(hMock, validIfNameAndField(expectedStructure))
        .WillOnce(Return(true));
    EXPECT_CALL(hMock, readStatistic(expectedStructure)).WillOnce(Return(1));

    EXPECT_EQ(IPMI_CC_OK,
              handleEthStatCommand(request.data(), reply, &dataLen, &hMock));

    struct EthStatReply expectedReply, realReply;
    expectedReply.statId = EthernetStatisticsIds::RX_BYTES;
    expectedReply.value = 1;

    std::memcpy(&realReply, reply, sizeof(realReply));
    EXPECT_EQ(0, std::memcmp(&expectedReply, &realReply, sizeof(realReply)));
}

} // namespace ethstats
