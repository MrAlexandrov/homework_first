#pragma once

#include "graph.hpp"

#include <cassert>
#include <cstdio>
#include <optional>
#include <string>
#include <vector>

namespace NPlotter {

using namespace NTypes;
using namespace NGraph;

class TPlotter final {
public:
    explicit TPlotter(const std::string& filename);

    void SetXValues(const std::vector<int>& values);

    template <typename T>
    void EmplaceGraphic(
        const std::vector<T>& data
        , const std::string& title
    );

    void Plot() const;

private:
    void SaveDataToFile() const;
    std::string GenerateGnuplotCommands() const;
    void SaveGnuplotCommandsToFile() const;

private:
    unsigned int Width_;
    unsigned int Height_;
    std::string ImageName_;
    std::string DataFile_;
    std::string CommandFile_;
    std::optional<int> DataSize_;
    std::vector<int> XValues_;
    std::vector<TGraph> Graphs_;
};

template <typename T>
void TPlotter::EmplaceGraphic(
    const std::vector<T>& data
    , const std::string& title
) {
    Graphs_.emplace_back(
        std::forward<decltype(data)>(data)
        , std::forward<decltype(title)>(title)
    );
}

}  // namespace NPlotter
