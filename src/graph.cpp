#include "graph.hpp"

#include <string>

namespace NGraph {

void TGraph::SetTitle(const std::string& title) {
    Title_ = title;
}

std::vector<Type> TGraph::GetData() const {
    return Data_;
}

std::string TGraph::GetTitle() const {
    return Title_;
}

size_t TGraph::GetSize() const {
    return Data_.size();
}

} // namespace NGraph
