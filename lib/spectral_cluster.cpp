#include "math_common.h"

Eigen::MatrixXd compute_affinity_matrix(const Eigen::MatrixXd &data, double sigma)
{
    int num_samples = data.rows();
    Eigen::MatrixXd affinity_matrix(num_samples, num_samples);

    for (int i = 0; i < num_samples; ++i)
    {
        for (int j = 0; j < num_samples; ++j)
        {
            // Calculate the squared Euclidean distance between sample i and j
            double distance_squared = (data.row(i) - data.row(j)).squaredNorm();

            // Calculate similarity using the Gaussian kernel function
            double similarity = exp(-distance_squared / (2 * sigma * sigma));

            // Fill the affinity matrix
            affinity_matrix(i, j) = similarity;
        }
    }

    return affinity_matrix;
}
// Function to perform K-means clustering
Eigen::VectorXd kmeans_clustering(const Eigen::MatrixXd &data, int num_clusters, double convergence_threshold = 1e-5)
{
    int num_samples = data.rows();
    int num_features = data.cols();
    Eigen::VectorXd cluster_assignments(num_samples);

    // Initialize cluster centroids randomly
    Eigen::MatrixXd centroids(num_clusters, num_features);
    for (int i = 0; i < num_clusters; ++i)
    {
        int random_i = rand() % num_samples;
        for (int j = 0; j < num_features; ++j)
        {
            centroids(i, j) = data(random_i, j);
        }
    }

    while (true)
    {
        // Assign each data point to the nearest centroid
        for (int i = 0; i < num_samples; ++i)
        {
            double min_distance = std::numeric_limits<double>::max();
            int nearest_cluster = -1;
            for (int j = 0; j < num_clusters; ++j)
            {
                double distance = (data.row(i) - centroids.row(j)).norm();
                if (distance < min_distance)
                {
                    min_distance = distance;
                    nearest_cluster = j;
                }
            }
            cluster_assignments(i) = nearest_cluster;
        }

        // Update centroids
        Eigen::MatrixXd new_centroids(num_clusters, num_features);
        for (int j = 0; j < num_clusters; ++j)
        {
            Eigen::VectorXd cluster_points = Eigen::VectorXd::Zero(num_features);
            int cluster_size = 0;

            for (int k = 0; k < num_samples; ++k)
                if (cluster_assignments(k) == j)
                {
                    cluster_size++;
                    cluster_points += data.row(k);
                }

            cluster_points /= cluster_size;

            new_centroids.row(j) = cluster_points;
        }

        // Check for convergence
        if ((new_centroids - centroids).norm() < convergence_threshold)
        {
            break;
        }

        centroids = new_centroids;
    }

    return cluster_assignments;
}

// Function to perform spectral clustering
Eigen::VectorXd clusterSpectral(const Eigen::MatrixXd &affinity_matrix, int num_clusters)
{
    // Step 1: Compute the Laplacian matrix
    Eigen::MatrixXd degree_matrix = affinity_matrix.rowwise().sum().asDiagonal();
    Eigen::MatrixXd laplacian_matrix = degree_matrix - affinity_matrix;

    // Step 2: Compute the first k eigenvectors of the Laplacian matrix
    Eigen::SelfAdjointEigenSolver<Eigen::MatrixXd> eigensolver(laplacian_matrix);
    if (eigensolver.info() != Eigen::Success)
    {
        std::cerr << "Eigenvalue decomposition failed." << std::endl;
        exit(1);
    }

    Eigen::MatrixXd eigen_vectors = eigensolver.eigenvectors();

    // Step 3: Cluster the eigenvectors using K-means
    Eigen::MatrixXd eigen_vectors_for_clustering = eigen_vectors.leftCols(num_clusters);
    Eigen::VectorXd cluster_assignments = kmeans_clustering(eigen_vectors_for_clustering, num_clusters);

    return cluster_assignments;
}

int test_spectral_cluster()
{
    int num_samples = 50;
    int num_features = 2;
    double sigma = 1.0;
    // Number of clusters
    int num_clusters = 4;

    // Sample data
    Eigen::MatrixXd data(num_samples, num_features);
    data.setRandom();

    Eigen::MatrixXd affinity_matrix = compute_affinity_matrix(data, sigma);

    // Perform spectral clustering
    Eigen::VectorXd cluster_assignments = clusterSpectral(affinity_matrix, num_clusters);

    // Print cluster assignments
    std::cout << "Cluster Assignments:" << std::endl;
    for (int i = 0; i < cluster_assignments.size(); ++i)
    {
        std::cout << "Data Point " << i << " (" << data.row(i) << ") : Cluster " << cluster_assignments(i) << std::endl;
    }

    return 0;
}

