#ifndef PCA_HPP
#define PCA_HPP

#include "common.h"

Eigen::MatrixXf pca(const std::vector<std::vector<float>> &in, const int k)
{
    if (in.empty())
    {
        throw std::invalid_argument("in.empty()");
    }

    if (k <= 0)
    {
        throw std::invalid_argument("k <= 0");
    }

    size_t row = in.size();
    size_t col = in[0].size();

    Eigen::MatrixXf mat(row, col);
    for (int i = 0; i < row; i++)
    {
        if (in[i].size() != col)
        {
            throw std::invalid_argument("in[i].size() != col");
        }
        for (int j = 0; j < col; j++)
        {
            mat(i, j) = in[i][j];
        }
    }

    Eigen::VectorXf avg = mat.colwise().mean();
    Eigen::MatrixXf centered = mat.rowwise() - avg.transpose();

    Eigen::MatrixXf cov = centered.adjoint() * centered;
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXf> eig(cov);
    Eigen::MatrixXf eigenVectors = eig.eigenvectors();

    Eigen::MatrixXf result = centered * eigenVectors.rightCols(k);

    return result;
}

#endif // PCA_HPP