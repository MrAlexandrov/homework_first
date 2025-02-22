#pragma once

#include <Eigen/Dense>
#include <vector>

namespace NAnalitycal {

using Type = double;
using TVector = Eigen::Matrix<Type, Eigen::Dynamic, 1>;
using TMatrix = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>;

class TAnalyticalSolution final {
public:
    TAnalyticalSolution(const TMatrix&);
    std::vector<Type> GetDistribution() const;

private:
    TVector CalculateStationaryDistribution() const;

private:
    const TMatrix P_;
    TVector Distribution_;
};

} // namespace NAnalitycal
