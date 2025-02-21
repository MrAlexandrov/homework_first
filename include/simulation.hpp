#pragma once

#include "utils.hpp"

#include <Eigen/Dense>
#include <vector>
#include <boost/asio.hpp>
#include <thread>

namespace NSimulation {

using Type = double;
using TVector = Eigen::Matrix<Type, Eigen::Dynamic, 1>;
using TMatrix = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>;

using TThreadPool = boost::asio::thread_pool;

class TSimulationSolution final {
public:
    TSimulationSolution(const TMatrix& P, size_t imitations, size_t iterations)
        : P_(P)
        , NumberStates_(P_.rows())
        , Count_(NumberStates_)
        , Imitations_(imitations)
        , Iterations_(iterations)
        {
            SimulateSolution(Imitations_, Iterations_);
        }

    std::vector<Type> GetDistribution() const {
        std::vector<Type> result;
        result.reserve(Count_.size());
        int total = Imitations_ * Iterations_;
        for (const auto& i : Count_) {
            result.emplace_back(static_cast<Type>(i) / total);
        }
        return result;
    }

private:
    void SimulateSolution(int imitations, int iterations) {
        TThreadPool pool(std::thread::hardware_concurrency());
        for (int i = 0; i < imitations; ++i) {
            boost::asio::post(pool,
                [this, iterations]() {
                    this->Imitation(iterations);
                }
            );
        }
        pool.join();
    }

    void Imitation(int iterations) {
        int currentState = 0;
        for (int i = 0; i < iterations; ++i, currentState = GetNextState(currentState)) {
            Count_[currentState].fetch_add(1);
        }
    }

    int GetNextState(int currentState) const {
        auto randomNumber = NUtils::GenerateRandomNumber();
        Type currentValue = 0;
        for (int i = 0; i < NumberStates_; ++i) {
            currentValue += P_(currentState, i);
            if (randomNumber <= currentValue) {
                return i;
            }
        }
        assert(false);
    }

private:
    const TMatrix P_;
    size_t NumberStates_;
    std::vector<std::atomic<int>> Count_;
    int Imitations_;
    int Iterations_;
};

} // namespace NSimulation
