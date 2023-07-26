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

#include "handler.hpp"

#include <stdplus/fd/create.hpp>
#include <stdplus/fd/line.hpp>
#include <stdplus/numeric/str.hpp>

namespace ethstats
{

std::uint64_t EthStats::readStatistic(stdplus::const_zstring path) const
{
    auto fd = stdplus::fd::open(path, stdplus::fd::OpenAccess::ReadOnly);
    auto liner = stdplus::fd::LineReader(fd);
    auto line = liner.readLine();
    if (line == nullptr)
    {
        throw std::runtime_error("Missing stats data");
    }
    auto ret = stdplus::StrToInt<10, std::uint64_t>{}(std::string_view(*line));
    line = liner.readLine();
    if ((line != nullptr && !line->empty()) || liner.readLine() != nullptr)
    {
        throw std::runtime_error("Invalid stats data");
    }
    return ret;
}

} // namespace ethstats
