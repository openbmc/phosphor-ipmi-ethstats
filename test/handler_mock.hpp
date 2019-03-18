#pragma once

#include "handler.hpp"

#include <cstdint>

#include <gmock/gmock.h>

namespace ethstats
{

class HandlerMock
{
  public:
    ~HandlerMock() = default;

    MOCK_CONST_METHOD1(validIfNameAndField, bool(const StatRequest&));
    MOCK_CONST_METHOD1(readStatistic, std::uint64_t(const StatRequest&));
};

} // namespace ethstats
