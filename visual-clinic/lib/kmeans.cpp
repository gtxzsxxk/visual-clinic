#include "math_common.h"


std::tuple<Eigen::MatrixXf, std::vector<int>>
clusterKMeans(const std::vector<std::vector<float>> &in, const int k, const int maxIter) {
    if (in.empty()) {
        throw std::invalid_argument("in.empty()");
    }

    if (k <= 0) {
        throw std::invalid_argument("k <= 0");
    }

    if (maxIter <= 0) {
        throw std::invalid_argument("maxIter <= 0");
    }

    size_t row = in.size();
    size_t col = in[0].size();

    Eigen::MatrixXf mat(row, col);
    for (int i = 0; i < row; i++) {
        if (in[i].size() != col) {
            throw std::invalid_argument("in[i].size() != col");
        }
        for (int j = 0; j < col; j++) {
            mat(i, j) = in[i][j];
        }
    }

    Eigen::MatrixXf centers(k, col);
    std::vector<int> labels(row);
    std::vector<int> counts(k);
    std::vector<int> oldLabels(row);

    // init centers
    for (int i = 0; i < k; i++) {
        int idx = rand() % row;
        for (int j = 0; j < col; j++) {
            centers(i, j) = mat(idx, j);
        }
    }

    int iter = 0;
    while (iter < maxIter) {
        // assign labels
        for (int i = 0; i < row; i++) {
            float minDist = std::numeric_limits<float>::max();
            int minIdx = -1;
            for (int j = 0; j < k; j++) {
                float dist = (mat.row(i) - centers.row(j)).norm();
                if (dist < minDist) {
                    minDist = dist;
                    minIdx = j;
                }
            }
            labels[i] = minIdx;
        }

        // update centers
        centers.setZero();
        counts.assign(k, 0);
        for (int i = 0; i < row; i++) {
            centers.row(labels[i]) += mat.row(i);
            counts[labels[i]]++;
        }
        for (int i = 0; i < k; i++) {
            if (counts[i] > 0) {
                centers.row(i) /= float(counts[i]);
            }
        }
        ++iter;

    }
    return {centers, labels};
}
