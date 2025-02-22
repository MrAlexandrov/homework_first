#pragma once

#include <Eigen/Dense>
#include <string>
#include <fstream>
#include <iostream>
#include <iomanip>

namespace NGrapher {

using Type = double;
using TMatrix = Eigen::Matrix<Type, Eigen::Dynamic, Eigen::Dynamic>;


class TGrapher{
public:
    TGrapher(const TMatrix& P, std::string filename = "default")
        : P_(P)
        , Filename_(filename)
        {
            GenerateDotFile();
            GenerateImage();
        }

private:
    void GenerateDotFile() const {
        size_t n = P_.rows();
        std::ofstream dotFile(Filename_ + ".dot");
        if (!dotFile.is_open()) {
            throw std::runtime_error("Failed to create DOT file: " + Filename_);
        }

        dotFile << "digraph MarkovChain {\n";
        dotFile << "    rankdir=LR;\n";
        dotFile << "    node [shape=circle];\n";

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = 0; j < n; ++j) {
                if (P_(i, j) > 0) {
                    dotFile << "    S" << i << " -> S" << j
                            << " [label=\"" << std::fixed << std::setprecision(2) << P_(i, j) << "\"];\n";
                }
            }
        }

        dotFile << "}\n";
    }

    void GenerateImage() const {
        try {
            std::string command = "dot -Tpng " + Filename_ + ".dot -o " + Filename_ + ".png";
            if (system(command.c_str()) != 0) {
                throw std::runtime_error("Failed to generate PNG image");
            }

            std::cout << "PNG image created successfully: " << Filename_ << ".png" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }

private:
    TMatrix P_;
    std::string Filename_;
};

} // namespace NGrapher
