#include "math_common.h"

Eigen::MatrixXf getCovariance(const std::vector<std::vector<float>> &inMat) {
    if (inMat.empty()) {
        throw std::invalid_argument("inMat.empty()");
    }

    size_t row = inMat.size();
    size_t col = inMat[0].size();

    Eigen::MatrixXf mat(row, col);
    for (int i = 0; i < row; i++) {
        if (inMat[i].size() != col) {
            throw std::invalid_argument("inMat[i].size() != col");
        }
        for (int j = 0; j < col; j++) {
            mat(i, j) = inMat[i][j];
        }
    }
    mat.transposeInPlace();

    Eigen::MatrixXf centered = mat.rowwise() - mat.colwise().mean();
    Eigen::MatrixXf cov = (centered.adjoint() * centered) / double(mat.rows() - 1);

    return cov;
}

Eigen::MatrixXf getPearsonCorr(const Eigen::MatrixXf &cov, const std::vector<float> &vars) {
    if (vars.empty()) {
        throw std::invalid_argument("vars.empty()");
    }

    size_t row = cov.rows();
    size_t col = cov.cols();

    if (row != col) {
        throw std::invalid_argument("row != col");
    }

    if (row != vars.size()) {
        throw std::invalid_argument("row != vars.size()");
    }

    Eigen::MatrixXf relativity(row, col);
    for (int i = 0; i < row; i++) {
        relativity(i, i) = 1;
        for (int j = i + 1; j < col; j++) {
            relativity(i, j) = cov(i, j) / sqrt(vars[i] * vars[j]);
            relativity(j, i) = relativity(i, j);
        }
    }
    return relativity;
}