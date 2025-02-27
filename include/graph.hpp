#pragma once

#include "types.hpp"

#include <string>
#include <vector>

namespace NGraph {

using namespace NTypes;

class TGraph final {
public:
    TGraph() = default;
    template<typename T>
    explicit TGraph(
        const std::vector<T>& data
        , const std::string& title = "default"
    );

    template<typename T>
    void SetData(const std::vector<T>&);
    void SetTitle(const std::string&);

    std::vector<Type> GetData() const;
    std::string GetTitle() const;

    size_t GetSize() const;

private:
    std::vector<Type> Data_;
    std::string Title_;
};

template<typename T>
TGraph::TGraph(
    const std::vector<T>& data
    , const std::string& title
) 
    : Title_(title)
{
    SetData(data);
}

template<typename T>
void TGraph::SetData(const std::vector<T>& data) {
    Data_.clear();
    Data_.reserve(data.size());
    for (const auto& value : data) {
        Data_.push_back(static_cast<Type>(value));
    }
}

} // namespace NGraph
