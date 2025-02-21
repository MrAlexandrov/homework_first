#pragma once

#include <iomanip>
#include <iostream>

namespace NPrecision {

class TPrecision final {
public:
    TPrecision(std::ios_base::fmtflags oldFlags, std::streamsize oldPrecision, size_t newPrecision)
        : Flags_(oldFlags)
        , Precision_(oldPrecision)
    {
        std::cout << std::setprecision(newPrecision) << std::fixed;
    }

    ~TPrecision() {
        std::cout.flags(Flags_);
        std::cout.precision(Precision_);
    }
private:
    std::ios_base::fmtflags Flags_;
    std::streamsize Precision_;
};

} // namespace NPrecision
