#pragma once

#include "handler.hpp"

#include <cstdint>
#include <string>

#include <gmock/gmock.h>

namespace ethstats
{

class HandlerMock : public EthStatsInterface
{
  public:
    ~HandlerMock() = default;

    MOCK_METHOD(bool, validIfNameAndField, (const std::string&), (const));
    MOCK_METHOD(std::uint64_t, readStatistic, (const std::string&), (const));
};

} // namespace ethstats
