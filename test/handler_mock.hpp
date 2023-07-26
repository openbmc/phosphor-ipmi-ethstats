#pragma once
#include "handler.hpp"

#include <gmock/gmock.h>

namespace ethstats
{

class HandlerMock : public EthStatsInterface
{
  public:
    MOCK_METHOD(std::uint64_t, readStatistic, (stdplus::const_zstring),
                (const));
};

} // namespace ethstats
