#pragma once

#include "handler.hpp"

#include <gmock/gmock.h>

#include <cstdint>

namespace ethstats
{

class HandlerMock {
 public:
  ~HandlerMock() = default;

  MOCK_CONST_METHOD1(validIfNameAndField, bool(const StatRequest&));
  MOCK_CONST_METHOD1(readStatistic, std::uint64_t(const StatRequest&));
};

} // namespace ethstats
