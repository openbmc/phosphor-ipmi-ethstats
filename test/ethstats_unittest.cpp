#include "ethstats.hpp"
#include "handler_mock.hpp"

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include <gtest/gtest.h>

#define MAX_IPMI_BUFFER 64

using ::testing::Return;
using ::testing::StrEq;
using ::testing::StrictMock;

namespace ethstats
{

TEST(EthStatsTest, InvalidStatReturnsFailure)
{
    // Verify that the enum of valid statistic IDs is checked.

    std::string ifName = "eth0";
    struct EthStatRequest requestStruct;
    requestStruct.statId = EthernetStatisticsIds::TX_WINDOW_ERRORS + 1;
    requestStruct.if_name_len = ifName.length();

    // Using StrictMock to ensure it isn't called.
    StrictMock<HandlerMock> hMock;

    EXPECT_EQ(ipmi::responseInvalidFieldRequest(),
              handleEthStatCommand(
                  nullptr, requestStruct.statId, requestStruct.if_name_len,
                  std::vector<uint8_t>(ifName.begin(), ifName.end()), &hMock));
}

TEST(EthStatsTest, InvalidIpmiPacketSize)
{
    // An IPMI packet for this command has a minimum length.

    std::string ifName = "e";
    struct EthStatRequest requestStruct;
    requestStruct.statId = EthernetStatisticsIds::RX_BYTES;
    requestStruct.if_name_len = ifName.length();

    // The minimum length is a 1-byte ifname - this gives one, but dataLen is
    // set to smaller.
    ifName.pop_back();

    // Using StrictMock to ensure it isn't called.
    StrictMock<HandlerMock> hMock;

    EXPECT_EQ(ipmi::responseReqDataLenInvalid(),
              handleEthStatCommand(
                  nullptr, requestStruct.statId, requestStruct.if_name_len,
                  std::vector<uint8_t>(ifName.begin(), ifName.end()), &hMock));
}

TEST(EthStatsTest, InvalidIpmiPacketContents)
{
    // The packet has a name length and name contents, if the name length is
    // longer than the packet size, it should fail.

    std::string ifName = "eth0";
    struct EthStatRequest requestStruct;
    requestStruct.statId = EthernetStatisticsIds::RX_BYTES;
    requestStruct.if_name_len = ifName.length() + 1;

    // Using StrictMock to ensure it isn't called.
    StrictMock<HandlerMock> hMock;

    EXPECT_EQ(ipmi::responseReqDataLenInvalid(),
              handleEthStatCommand(
                  nullptr, requestStruct.statId, requestStruct.if_name_len,
                  std::vector<uint8_t>(ifName.begin(), ifName.end()), &hMock));
}

TEST(EthStatsTest, NameHasIllegalCharacters)
{
    // The interface name cannot have slashes.
    std::string ifName = "et/h0";
    struct EthStatRequest requestStruct;
    requestStruct.statId = EthernetStatisticsIds::RX_BYTES;
    requestStruct.if_name_len = ifName.length();

    // Using StrictMock to ensure it isn't called.
    StrictMock<HandlerMock> hMock;

    EXPECT_EQ(ipmi::responseInvalidFieldRequest(),
              handleEthStatCommand(
                  nullptr, requestStruct.statId, requestStruct.if_name_len,
                  std::vector<uint8_t>(ifName.begin(), ifName.end()), &hMock));
}

TEST(EthStatsTest, InvalidNameOrField)
{
    // The handler returns failure on the input validity check.
    std::string ifName = "eth0";
    struct EthStatRequest requestStruct;
    requestStruct.statId = EthernetStatisticsIds::RX_BYTES;
    requestStruct.if_name_len = ifName.length();

    std::string expectedPath = buildPath(ifName, "rx_bytes");

    HandlerMock hMock;
    EXPECT_CALL(hMock, validIfNameAndField(StrEq(expectedPath)))
        .WillOnce(Return(false));

    EXPECT_EQ(ipmi::responseInvalidFieldRequest(),
              handleEthStatCommand(
                  nullptr, requestStruct.statId, requestStruct.if_name_len,
                  std::vector<uint8_t>(ifName.begin(), ifName.end()), &hMock));
}

TEST(EthStatsTest, EverythingHappy)
{
    std::string ifName = "eth0";
    struct EthStatRequest requestStruct;
    requestStruct.statId = EthernetStatisticsIds::RX_BYTES;
    requestStruct.if_name_len = ifName.length();

    std::string expectedPath = buildPath(ifName, "rx_bytes");

    HandlerMock hMock;
    EXPECT_CALL(hMock, validIfNameAndField(StrEq(expectedPath)))
        .WillOnce(Return(true));
    EXPECT_CALL(hMock, readStatistic(StrEq(expectedPath))).WillOnce(Return(1));

    struct EthStatReply expectedReply;
    expectedReply.statId = EthernetStatisticsIds::RX_BYTES;
    expectedReply.value = 1;

    auto response = handleEthStatCommand(
        nullptr, requestStruct.statId, requestStruct.if_name_len,
        std::vector<uint8_t>(ifName.begin(), ifName.end()), &hMock);

    EXPECT_EQ(::ipmi::ccSuccess, std::get<0>(response));

    auto data = std::get<1>(response);
    EXPECT_TRUE(data.has_value());

    auto actualStatId = std::get<0>(*data);
    auto actualValue = std::get<1>(*data);
    EXPECT_EQ(expectedReply.statId, actualStatId);
    EXPECT_EQ(expectedReply.value, actualValue);
}

} // namespace ethstats
