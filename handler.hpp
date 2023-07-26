#pragma once
#include <stdplus/zstring.hpp>

#include <cstdint>

namespace ethstats
{

class EthStatsInterface
{
  public:
    virtual ~EthStatsInterface() = default;

    /** Given an ifname and a statistic, return the value.
     *
     * @param[in] path - the interface name and statistics field
     * @return the value of that statistic for that interface.
     */
    virtual std::uint64_t readStatistic(stdplus::const_zstring path) const = 0;
};

class EthStats : public EthStatsInterface
{
  public:
    std::uint64_t readStatistic(stdplus::const_zstring path) const override;
};

} // namespace ethstats
