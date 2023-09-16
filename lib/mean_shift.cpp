#include "math_common.h"

// Function to calculate the Gaussian kernel
Eigen::MatrixXd gaussian_kernel(const Eigen::MatrixXd &X, double bandwidth)
{
    int num_samples = X.rows();
    Eigen::MatrixXd kernel_matrix(num_samples, num_samples);

    for (int i = 0; i < num_samples; ++i)
    {
        for (int j = 0; j < num_samples; ++j)
        {
            double distance = (X.row(i) - X.row(j)).norm();
            kernel_matrix(i, j) = exp(-0.5 * pow(distance / bandwidth, 2));
        }
    }

    return kernel_matrix;
}

// Function to perform mean-shift clustering
// The output represents the cluster point number of every point
Eigen::VectorXd clusterMeanShift(const Eigen::MatrixXd &data, double bandwidth, double convergence_threshold)
{
    int num_samples = data.rows();
    Eigen::VectorXd cluster_assignments(num_samples);

    while (true)
    {
        Eigen::MatrixXd kernel = gaussian_kernel(data, bandwidth);
        Eigen::VectorXd prev_assignments = cluster_assignments;

        for (int i = 0; i < num_samples; ++i)
        {
            // Calculate the mean shift vector
            Eigen::VectorXd shift_vector = (kernel.row(i) * data);
            double weight_sum = kernel.row(i).sum();
            shift_vector /= weight_sum;
            std::cout << shift_vector;

            // Find the nearest cluster center
            double min_distance = std::numeric_limits<double>::max();
            int nearest_cluster = -1;
            for (int j = 0; j < num_samples; ++j)
            {
                double distance = (data.row(j) - shift_vector.transpose()).norm();
                if (distance < min_distance)
                {
                    min_distance = distance;
                    nearest_cluster = j;
                }
            }

            // Update the cluster assignment
            cluster_assignments(i) = nearest_cluster;
        }

        // Check for convergence
        if ((prev_assignments - cluster_assignments).norm() < convergence_threshold)
        {
            break;
        }
    }

    return cluster_assignments;
}

int test_mean_shift()
{
    // Sample data
    Eigen::MatrixXd data(100, 2);
    data.setRandom();

    // Parameters
    double bandwidth = 5;
    double convergence_threshold = 1e-4;

    // Perform mean-shift clustering
    Eigen::VectorXd cluster_assignments = clusterMeanShift(data, bandwidth, convergence_threshold);

    // Print cluster assignments
    std::cout << "Cluster Assignments:" << std::endl;
    for (int i = 0; i < cluster_assignments.size(); ++i)
    {
        std::cout << "Data Point " << i << ": Cluster " << cluster_assignments(i) << std::endl;
    }

    return 0;
}