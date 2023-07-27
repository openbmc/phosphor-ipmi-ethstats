/*
 * Copyright 2019 Google Inc.
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

#include "ethstats.hpp"

#include "handler.hpp"

#include <endian.h>
#include <ipmid/api.h>

#include <stdplus/raw.hpp>
#include <stdplus/str/cat.hpp>

#include <cstdint>
#include <span>
#include <string_view>
#include <unordered_map>

namespace ethstats
{

using std::literals::operator""sv;

// If this changes in the future, there should be some alternative
// source for the information if possible to provide continuined functionality.
static const std::unordered_map<std::uint8_t, std::string_view> statLookup = {
    {RX_BYTES, "rx_bytes"sv},
    {RX_COMPRESSED, "rx_compressed"sv},
    {RX_CRC_ERRORS, "rx_crc_errors"sv},
    {RX_DROPPED, "rx_dropped"sv},
    {RX_ERRORS, "rx_errors"sv},
    {RX_FIFO_ERRORS, "rx_fifo_errors"sv},
    {RX_FRAME_ERRORS, "rx_frame_errors"sv},
    {RX_LENGTH_ERRORS, "rx_length_errors"sv},
    {RX_MISSED_ERRORS, "rx_missed_errors"sv},
    {RX_NOHANDLER, "rx_nohandler"sv},
    {RX_OVER_ERRORS, "rx_over_errors"sv},
    {RX_PACKETS, "rx_packets"sv},
    {TX_ABORTED_ERRORS, "tx_aborted_errors"sv},
    {TX_BYTES, "tx_bytes"sv},
    {TX_CARRIER_ERRORS, "tx_carrier_errors"sv},
    {TX_COMPRESSED, "tx_compressed"sv},
    {TX_DROPPED, "tx_dropped"sv},
    {TX_ERRORS, "tx_errors"sv},
    {TX_FIFO_ERRORS, "tx_fifo_errors"sv},
    {TX_HEARTBEAT_ERRORS, "tx_heartbeat_errors"sv},
    {TX_PACKETS, "tx_packets"sv},
    {TX_WINDOW_ERRORS, "tx_window_errors"sv},
};

ipmi_ret_t handleEthStatCommand(const std::uint8_t* reqBuf,
                                std::uint8_t* replyCmdBuf, size_t* dataLen,
                                const EthStatsInterface* handler)
{
    static_assert(alignof(EthStatReply) == 1);
    auto& reply = *reinterpret_cast<EthStatReply*>(replyCmdBuf);
    std::string_view stat, name;
    try
    {
        std::string_view req(reinterpret_cast<const char*>(reqBuf), *dataLen);
        const auto& hdr = stdplus::raw::extractRef<EthStatRequest>(req);
        if (hdr.if_name_len > req.size())
        {
            return IPMI_CC_REQ_DATA_LEN_INVALID;
        }
        auto it = statLookup.find(hdr.statId);
        if (it == statLookup.end())
        {
            return IPMI_CC_INVALID_FIELD_REQUEST;
        }
        reply.statId = hdr.statId;
        stat = it->second;
        name = req.substr(0, hdr.if_name_len);
    }
    catch (...)
    {
        return IPMI_CC_REQ_DATA_LEN_INVALID;
    }

    // Minor sanity & security check. (of course, I'm less certain if unicode
    // comes into play here.
    //
    // Basically you can't easily inject ../ or /../ into the path below.
    // Decided to make this more robust, although since it appends to the path
    // it would limit any exposure.
    if (name.find('/') != std::string::npos)
    {
        return IPMI_CC_INVALID_FIELD_REQUEST;
    }

    try
    {
        reply.value = htole64(handler->readStatistic(stdplus::strCat(
            "/sys/class/net/"sv, name, "/statistics/"sv, stat)));
    }
    catch (const std::system_error& e)
    {
        if (e.code() == std::errc::no_such_file_or_directory)
        {
            return IPMI_CC_INVALID_FIELD_REQUEST;
        }
        throw;
    }

    (*dataLen) = sizeof(reply);
    return IPMI_CC_OK;
}

} // namespace ethstats
