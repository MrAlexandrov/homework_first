#pragma once

#include "types.hpp"

#include <vector>

namespace NAnalitycal {

using namespace NTypes;

class TAnalyticalSolution final {
public:
    TAnalyticalSolution(const TMatrix&);
    std::vector<Type> CalculateAndGetDistribution();
    std::vector<Type> GetDistribution() const;

private:
    void CalculateDistribution();
    TVector CalculateDistributionImpl() const;

private:
    const TMatrix P_;
    std::vector<Type> Distribution_;
};

} // namespace NAnalitycal
