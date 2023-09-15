#include "math_common.h"

// Find
std::vector<int> region_query(const std::vector<Eigen::VectorXf> &in, int point_idx, double epsilon)
{
    std::vector<int> neighbors;
    for (int i = 0; i < in.size(); ++i)
    {
        if ((in[i] - in[point_idx]).norm() <= epsilon)
        {
            neighbors.push_back(i);
        }
    }
    return neighbors;
}

// Output includes the class of all input points
std::vector<int> dbscan_cluster(const std::vector<Eigen::VectorXf> &in, const double epsilon, const int min_points)
{
    int point_count = in.size();

    // Initialize the label list
    // -1 represents unvisited status
    std::vector<int> label_list(point_count, -1);

    // Cluster id == 0
    int cluster_id = 0;
    // Tranverse all the points
    for (int i = 0; i < point_count; ++i)
    {
        if (label_list[i] != -1)
            // Skip already visited points
            continue;

        std::vector<int> neighbors = region_query(in, i, epsilon);
        if (neighbors.size() < min_points)
        {
            label_list[i] = 0; // Mark as noise (cluster 0)
        }
        else
        {
            cluster_id++;

            // Expand clusters
            label_list[i] = cluster_id;

            for (int i = 0; i < neighbors.size(); ++i)
            {
                int neighbor_idx = neighbors[i];
                if (label_list[neighbor_idx] == -1)
                {
                    label_list[neighbor_idx] = cluster_id;
                    std::vector<int> neighborNeighbors = region_query(in, neighbor_idx, epsilon);
                    if (neighborNeighbors.size() >= min_points)
                    {
                        neighbors.insert(neighbors.end(), neighborNeighbors.begin(), neighborNeighbors.end());
                    }
                }
            }
        }
    }

    return label_list;
}

std::vector<Eigen::VectorXf> vector_array_to_vectorXf_array(std::vector<std::vector<float>> vector_array)
{
    std::vector<Eigen::VectorXf> vectorXf_array;
    for (const auto &std_vector : vector_array)
    {
        // Get size of vector
        int size = std_vector.size();

        Eigen::VectorXf eigen_vector(size);

        // Copy std::vector into Eigen::VectorXf
        for (int i = 0; i < size; ++i)
        {
            eigen_vector(i) = std_vector[i];
        }

        vectorXf_array.push_back(eigen_vector);
    }
    return vectorXf_array;
}

void testDBSCANCluster()
{
    std::vector<std::vector<float>> initial_points = {
            {1, 2},
            {2, 1},
            {3, 1},
            {4, 1},
            {5, 2},
            {5, 4},
            {5, 5},
            {6, 5},
            {10, 8},
            {7, 9},
            {11, 5},
            {14, 13},
            {14, 14},
            {15, 13}};
    auto points = vector_array_to_vectorXf_array(initial_points);

    auto res = dbscan_cluster(points, 2, 3);
    for (int i = 0; i < res.size(); ++i)
    {
        std::cout << "point " << i << " (" << points[i](0) << ", " << points[i](1)
                  << ") label: "
                  << res[i] << std::endl;
    }
}