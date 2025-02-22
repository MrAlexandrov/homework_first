#include "types.hpp"
#include "analytical.hpp"
#include "grapher.hpp"
#include "precision.hpp"
#include "simulation.hpp"

#include <Eigen/Dense>
#include <iostream>
#include <string_view>
#include <vector>

using namespace NTypes;

TMatrix GetTransitionMatrix(int n) {
    TMatrix P(n, n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            Type x;
            std::cin >> x;
            P(i, j) = x;
        }
    }
    return P;
}

void PrintResults(std::string_view text, const std::vector<Type>& results) {
    std::cout << text << "\n";
    for (const auto& i : results) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

std::vector<Type> CountError(const std::vector<Type>& lhs, const std::vector<Type>& rhs) {
    assert(lhs.size() == rhs.size());
    int n = lhs.size();
    std::vector<Type> result(n);
    std::transform(
        lhs.cbegin(), lhs.cend(),
        rhs.cbegin(), result.begin(),
        [](Type lhs, Type rhs) {
            return std::fabs(lhs - rhs);
        }
    );
    return result;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage " << argv[0] << " <Imitations> <Iterations>";
        return 1;
    }
    size_t Imitations = std::stoul(argv[1]);
    size_t Iterations = std::stoul(argv[2]);

    size_t N = 5;
    std::cin >> N;
    TMatrix P = GetTransitionMatrix(N);
    NAnalitycal::TAnalyticalSolution analytic(P);
    NSimulation::TSimulationSolution simulate(P, Imitations, Iterations);
    std::vector<Type> analyticSolution = analytic.GetDistribution();
    std::vector<Type> simulateSolution = simulate.GetDistribution();
    std::vector<Type> errors = CountError(analyticSolution, simulateSolution);

    {
        NPrecision::TPrecision changer(
            std::cout.flags(),
            std::cout.precision(),
            6
        );

        PrintResults("Analytical distribution:", analyticSolution);
        PrintResults("Imitated distribution:", simulateSolution);
        PrintResults("Errors:", errors);
    }
    NGrapher::TGrapher graph(P, "chain", analyticSolution);
    return 0;
}
