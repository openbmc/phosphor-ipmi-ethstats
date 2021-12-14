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

#include "ethstats.hpp"

#include <cstdio>
#include <ipmid/api-types.hpp>
#include <ipmid/handler.hpp>
#include <ipmid/iana.hpp>
#include <ipmid/message.hpp>
#include <ipmid/oemopenbmc.hpp>

namespace ethstats
{

EthStats handler;

Resp ethStatCommand(ipmi::Context::ptr ctx, uint8_t statId, uint8_t length,
                    const std::vector<uint8_t>& data)
{
    return handleEthStatCommand(ctx, statId, length, data, &handler);
}

} // namespace ethstats

void setupGlobalOemEthStats() __attribute__((constructor));

void setupGlobalOemEthStats()
{
#if ENABLE_GOOGLE
    /* Install in Google OEM Namespace when enabled. */
    std::fprintf(stderr,
                 "Registering OEM:[%#08X], Cmd:[%#04X] for Ethstats Commands\n",
                 oem::googOemNumber, oem::Cmd::ethStatsCmd);
    ipmi::registerOemHandler(::ipmi::prioOemBase, oem::googOemNumber,
                             oem::Cmd::ethStatsCmd, ::ipmi::Privilege::User,
                             ethstats::ethStatCommand);
#endif

    std::fprintf(stderr,
                 "Registering OEM:[%#08X], Cmd:[%#04X] for Ethstats Commands\n",
                 oem::obmcOemNumber, oem::Cmd::ethStatsCmd);
    ipmi::registerOemHandler(::ipmi::prioOemBase, oem::obmcOemNumber,
                             oem::Cmd::ethStatsCmd, ::ipmi::Privilege::User,
                             ethstats::ethStatCommand);
}
