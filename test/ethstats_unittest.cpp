#include "ethstats.hpp"
#include "handler_mock.hpp"

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
}

} // namespace ethstats
