#ifndef ROWFEATURE_HPP
#define ROWFEATURE_HPP

#include "common.h"

std::tuple<float, float> getAvgVar(const std::vector<float> &inX)
{
    if (inX.empty())
    {
        throw std::invalid_argument("inX.empty()");
    }

    Eigen::Map<const Eigen::VectorXf> x(inX.data(), inX.size());
    float avg = x.mean();
    auto diff = x.array() - avg;
    float var = diff.square().sum() / (inX.size() - 1);
    return { avg, var };
}

#endif // ROWFEATURE_HPP
