#pragma once

#include <cstdint>

namespace ethstats
{

/**
 * @brief Ethstat Request structure.
 */
struct EthStatRequest
{
    uint8_t statId;
    uint8_t if_name_len;
} __attribute__((packed));

/**
 * @brief Ethstat Reply structure.
 */
struct EthStatReply
{
    uint8_t statId;
    uint64_t value;
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

} // namespace ethstats
