#pragma once

#include <Eigen/Dense>
#include <string>

namespace NGrapher {

using Type = double;
using TMatrix = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>;

class TGrapher{
public:
    TGrapher(const TMatrix& P, std::string filename);

private:
    void GenerateDotFile() const;
    void GenerateImage() const;

private:
    TMatrix P_;
    std::string Filename_;
};

} // namespace NGrapher
