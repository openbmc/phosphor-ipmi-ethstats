/*
 * Copyright 2018 Google Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "config.h"

#include <host-ipmid/ipmid-api.h>

#include <cstdint>
#include <cstring>
#include <experimental/filesystem>
#include <fstream>
#include <host-ipmid/iana.hpp>
#include <host-ipmid/oemrouter.hpp>
#include <map>
#include <sstream>
#include <string>

namespace oem
{
constexpr auto ethStatsCmd = 48;
} // namespace oem

namespace ethstats
{
namespace fs = std::experimental::filesystem;

struct EthStatRequest
{
    uint8_t stat_id;
    uint8_t if_name_len;
    uint8_t if_name[0];
} __attribute__((packed));

struct EthStatReply
{
    uint8_t stat_id;
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

// This is a mapping of the statistics ID to the filename used by this version
// of gBMC.  If this changes in the future, there should be some alternative
// source for the information if possible to provide continuined functionality.
static const std::map<uint8_t, std::string> statLookup = {
    {RX_BYTES, "rx_bytes"},
    {RX_COMPRESSED, "rx_compressed"},
    {RX_CRC_ERRORS, "rx_crc_errors"},
    {RX_DROPPED, "rx_dropped"},
    {RX_ERRORS, "rx_errors"},
    {RX_FIFO_ERRORS, "rx_fifo_errors"},
    {RX_FRAME_ERRORS, "rx_frame_errors"},
    {RX_LENGTH_ERRORS, "rx_length_errors"},
    {RX_MISSED_ERRORS, "rx_missed_errors"},
    {RX_NOHANDLER, "rx_nohandler"},
    {RX_OVER_ERRORS, "rx_over_errors"},
    {RX_PACKETS, "rx_packets"},
    {TX_ABORTED_ERRORS, "tx_aborted_errors"},
    {TX_BYTES, "tx_bytes"},
    {TX_CARRIER_ERRORS, "tx_carrier_errors"},
    {TX_COMPRESSED, "tx_compressed"},
    {TX_DROPPED, "tx_dropped"},
    {TX_ERRORS, "tx_errors"},
    {TX_FIFO_ERRORS, "tx_fifo_errors"},
    {TX_HEARTBEAT_ERRORS, "tx_heartbeat_errors"},
    {TX_PACKETS, "tx_packets"},
    {TX_WINDOW_ERRORS, "tx_window_errors"},
};

static ipmi_ret_t HandleEthStatCommand(ipmi_cmd_t cmd, const uint8_t* reqBuf,
                                       uint8_t* replyCmdBuf, size_t* dataLen)
{
    // Verify the reqBuf is the minimum length.
    // [0] == statistics id
    // [1] == if_name_length
    // [2..N] == if_name
    // In theory the smallest can be a one-letter name. (3 bytes).
    if ((*dataLen) < sizeof(struct EthStatRequest) + sizeof(uint8_t))
    {
        std::fprintf(stderr, "*dataLen too small: %u\n",
                     static_cast<uint32_t>(*dataLen));
        return IPMI_CC_INVALID;
    }

    const auto request =
        reinterpret_cast<const struct EthStatRequest*>(&reqBuf[0]);

    if ((*dataLen) < (sizeof(struct EthStatRequest) + request->if_name_len))
    {
        std::fprintf(stderr, "*dataLen too small: %u\n",
                     static_cast<uint32_t>(*dataLen));
        return IPMI_CC_INVALID;
    }

    // Check the statistic to see if we recognize it.
    auto stat = statLookup.find(request->stat_id);
    if (stat == statLookup.end())
    {
        std::fprintf(stderr, "stat not known: 0x%x\n", request->stat_id);
        return IPMI_CC_INVALID_FIELD_REQUEST;
    }

    // The if_name handling plus a few other things was taken from the
    // CableCheck command implementation.
    //
    // Ok, so we know what statistic they want.  Let's validate their
    // if_name.  The string is length delimited (like dns).

    // Maximum length one can specify, plus null terminator.
    char nameBuf[256] = {};

    // Copy the string out of the request buffer.
    std::memcpy(&nameBuf[0], request->if_name, request->if_name_len);
    std::string name = nameBuf;

    // Minor sanity & security check (of course, I'm less certain if unicode
    // comes into play here.
    //
    // Basically you can't easily inject ../ or /../ into the path below.
    // Decided to make this more robust, although since it appends to the path
    // it would limit any exposure.
    if (name.find("/") != std::string::npos)
    {
        std::fprintf(stderr, "Invalid or illegal name: '%s'\n", nameBuf);
        return IPMI_CC_INVALID;
    }

    std::ostringstream opath;
    opath << "/sys/class/net/" << name << "/statistics/" << stat->second;
    std::string path = opath.str();

    std::error_code ec;
    if (!fs::exists(path, ec))
    {
        std::fprintf(stderr, "Path: '%s' doesn't exist.\n", path.c_str());
        return IPMI_CC_INVALID;
    }
    // We're uninterested in the state of ec.

    // Read the file and check the result.
    // We read the number as int64, then check to make sure it's positive
    // before casting to uint64.
    uint64_t value = 0;
    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit);

    try
    {
        ifs.open(path);
        ifs >> value;
    }
    catch (std::ios_base::failure& fail)
    {
        return IPMI_CC_INVALID;
    }

    struct EthStatReply reply;
    reply.stat_id = request->stat_id;
    reply.value = value;

    // Store the result.
    std::memcpy(&replyCmdBuf[0], &reply, sizeof(struct EthStatReply));
    (*dataLen) = sizeof(struct EthStatReply);

    return IPMI_CC_OK;
}

void setupGlobalOemCableCheck() __attribute__((constructor));

void setupGlobalOemCableCheck()
{
    oem::Router* oemRouter = oem::mutableRouter();

#ifdef ENABLE_GOOGLE
    /* Install in Google OEM Namespace when enabled. */
    std::fprintf(stderr,
                 "Registering OEM:[%#08X], Cmd:[%#04X] for Ethstats Commands\n",
                 oem::googOemNumber, oem::ethStatsCmd);

    oemRouter->registerHandler(oem::googOemNumber, oem::ethStatsCmd,
                               HandleEthStatCommand);
#endif

    std::fprintf(stderr,
                 "Registering OEM:[%#08X], Cmd:[%#04X] for Ethstats Commands\n",
                 oem::obmcOemNumber, oem::ethStatsCmd);

    oemRouter->registerHandler(oem::obmcOemNumber, oem::ethStatsCmd,
                               HandleEthStatCommand);
}

} // namespace ethstats
