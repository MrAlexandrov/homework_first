#include "plotter.hpp"
#include "types.hpp"
#include "analytical.hpp"
#include "drawer.hpp"
#include "precision.hpp"
#include "imitation.hpp"

#include <Eigen/Dense>
#include <cmath>
#include <unsupported/Eigen/MatrixFunctions> // for power
#include <iostream>
#include <numeric>
#include <string>
#include <string_view>
#include <vector>

using namespace NTypes;

constexpr int POWER_VALUE = 100;
constexpr int CHART_AMOUNT = 3;

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
    std::cout << "\n";
}

std::vector<Type> AbsoluteError(const std::vector<Type>& lhs, const std::vector<Type>& rhs) {
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

std::vector<Type> SampleAverage(const std::vector<std::vector<int>>& states) {
    size_t n = states.size();
    assert(n != 0);
    size_t m = states.front().size();
    std::vector<Type> average(m, 0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            average[j] += states[i][j];
        }
    }
    for (int j = 0; j < m; ++j) {
        average[j] /= n;
    }
    return average;
}

std::vector<Type> StandartDeviation(const std::vector<Type>& average, const std::vector<std::vector<double>>& probabilities) {
    size_t n = probabilities.size();
    size_t m = average.size();
    std::vector<Type> diffs(m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            Type value = average[j] - probabilities[i][j];
            diffs[j] += value * value;
        }
    }
    for (int j = 0; j < m; ++j) {
        diffs[j] /= n - 1;
        diffs[j] = sqrt(diffs[j]);
    }
    return diffs;
}

int main(int argc, char** argv) {
    if (argc != 3) {
        std::cerr << "Usage " << argv[0] << " <Imitations> <Iterations>";
        return 1;
    }
    size_t Imitations = std::stoul(argv[1]);
    size_t Iterations = std::stoul(argv[2]);

    size_t N = 0;
    std::cin >> N;
    TMatrix P = GetTransitionMatrix(N);

    Eigen::MatrixPower<TMatrix> PPowered(P);
    std::cout << "P^" << POWER_VALUE << ":\n";
    std::cout << PPowered(POWER_VALUE) << "\n";

    // counting distributions
    NAnalitycal::TAnalyticalSolution analytic(P);
    NImitation::TImitationSolution imitated(P, Imitations, Iterations);
    std::vector<Type> analyticSolution = analytic.CalculateAndGetDistribution();
    std::vector<Type> imitatedSolution = imitated.ImitateAndGetDistribution();
    std::vector<Type> absoluteError = AbsoluteError(analyticSolution, imitatedSolution);

    // counting standart deviation
    auto states = imitated.GetAllStates();
    std::vector<std::vector<double>> probabilities(Imitations, std::vector(Iterations, 0.0));
    for (size_t i = 0; i < Imitations; ++i) {
        for (const auto& currentState : states[i]) {
            ++probabilities[i][currentState];
        }
        for (int j = 0; j < N; ++j) {
            probabilities[i][j] /= Iterations;
        }
    }
    std::vector<Type> standartDeviation = StandartDeviation(imitatedSolution, probabilities);

    // printing results
    {
        NPrecision::TPrecision changer(
            std::cout.flags(),
            std::cout.precision(),
            6
        );

        PrintResults("Analytical distribution:", analyticSolution);
        PrintResults("Imitated distribution:", imitatedSolution);
        PrintResults("Absolute Error:", absoluteError);
        PrintResults("Standart Deviation:", standartDeviation);
    }
    // drawing chain
    NDrawer::TDrawer::GenerateAndDrawChart(P, "chain", analyticSolution);

    // plotting chart
    NPlotter::TPlotter chart("chart");
    std::vector<int> XValues(Iterations);
    std::iota(XValues.begin(), XValues.end(), 0);
    chart.SetXValues(XValues);
    for (int i = 0; i < CHART_AMOUNT; ++i) {
        chart.EmplaceChart(states[i], std::to_string(i));
    }
    chart.Plot();
    return 0;
}
