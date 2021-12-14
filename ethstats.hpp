#pragma once

#include "handler.hpp"

#include <ipmid/api.h>

#include <ipmid/api-types.hpp>
#include <ipmid/message.hpp>

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace ethstats
{

using Resp = ::ipmi::RspType<std::uint8_t, std::uint64_t>;

/**
 * @brief Ethstat Request structure.
 */
struct EthStatRequest
{
    std::uint8_t statId;
    std::uint8_t if_name_len;
} __attribute__((packed));

/**
 * @brief Ethstat Reply structure.
 */
struct EthStatReply
{
    std::uint8_t statId;
    std::uint64_t value;
} __attribute__((packed));

enum EthernetStatisticsIds
{
    RX_BYTES = 0,
    RX_COMPRESSED = 1,
    RX_CRC_ERRORS = 2,
    RX_DROPPED = 3,
    RX_ERRORS = 4,
    RX_FIFO_ERRORS = 5,
    RX_FRAME_ERRORS = 6,
    RX_LENGTH_ERRORS = 7,
    RX_MISSED_ERRORS = 8,
    RX_NOHANDLER = 9,
    RX_OVER_ERRORS = 10,
    RX_PACKETS = 11,
    TX_ABORTED_ERRORS = 12,
    TX_BYTES = 13,
    TX_CARRIER_ERRORS = 14,
    TX_COMPRESSED = 15,
    TX_DROPPED = 16,
    TX_ERRORS = 17,
    TX_FIFO_ERRORS = 18,
    TX_HEARTBEAT_ERRORS = 19,
    TX_PACKETS = 20,
    TX_WINDOW_ERRORS = 21,
};

/**
 * Handle the OEM IPMI EthStat Command.
 *
 * @param[in] statId - Stat Id of the request.
 * @param[in] data - Read of the ipmi payload.
 * @param[in] handler - pointer to ethstats implementation.
 * @return the IPMI result code.
 */
Resp handleEthStatCommand(uint8_t statId,
                          uint8_t length, std::span<const uint8_t> data,
                          const EthStatsInterface* handler);

} // namespace ethstats
