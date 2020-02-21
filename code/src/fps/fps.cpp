#include "fps/fps.h"
std::vector<int> farthest_point_sampling(const Eigen::MatrixXd &pts, int M) {
    /* Input:
     *    pts: N x d point cloud
     * ---
     * Output:
     *    indices: A vector with M Indices of subsampled point cloud
     */
    std::random_device rd;
    std::mt19937 mt(rd());
    int N = pts.rows();
    // int D = pts.cols();
    ska::flat_hash_set<int> in_set; // lookup table
    std::vector<int> in_set_vector; // indices to be return
    std::vector<float> dist_vector(N, 0); 
    std::uniform_int_distribution<int> uniform_N(0, N-1);
    M = std::min(M, N);
    in_set.insert(uniform_N(mt));
    for (int j=1; j<M; ++j) {
        #ifdef USE_OMP
        #pragma omp parallel for shared(pts, in_set, in_set_vector, dist_vector)
        #endif
        for (int k=0; k<N; ++k) {
            if (in_set.count(k)>0) {
                dist_vector[k] = 0.0;
                continue;
            }
            float min_dist = FLT_MAX;
            for (auto v_in : in_set_vector) {
                float dist = (pts.row(v_in)-pts.row(k)).norm();
                if (dist<min_dist) { // Assign point to nearest in-set point
                    min_dist = dist;
                }
            }
            dist_vector[k] = min_dist;
        }
        int best_index = 0;
        float best_dist = dist_vector[0];
        for (int i=1; i<dist_vector.size(); ++i) {
            if (dist_vector[i]>best_dist) {
                best_dist = dist_vector[i];
                best_index = i;
            }
        }
        in_set.insert(best_index);
        in_set_vector.push_back(best_index);
    }
    return in_set_vector;
}

std::vector<int> farthest_point_sampling(const Eigen::MatrixXd &pts, const std::vector<int> &mask, int M) {
    /* Input:
     *    pts: N x d point cloud
     *    mask: <N points (masked)
     * ---
     * Output:
     *    indices: A vector with M Indices of subsampled point cloud
     */
    std::random_device rd;
    std::mt19937 mt(rd());
    int N = mask.size();
    // int D = pts.cols();
    ska::flat_hash_set<int> in_set; // lookup table
    std::vector<int> in_set_vector; // indices to be return
    std::vector<float> dist_vector(N, 0); 
    std::uniform_int_distribution<int> uniform_N(0, N-1);
    M = std::min(M, N);
    in_set.insert(uniform_N(mt));
    for (int j=1; j<M; ++j) {
        #ifdef USE_OMP
        #pragma omp parallel for shared(pts, in_set, in_set_vector, dist_vector)
        #endif
        for (int k=0; k<N; ++k) {
            if (in_set.count(k)>0) {
                dist_vector[k] = 0.0;
                continue;
            }
            float min_dist = FLT_MAX;
            for (auto v_in : in_set_vector) {
                float dist = (pts.row(mask[v_in])-pts.row(mask[k])).norm();
                if (dist<min_dist) { // Assign point to nearest in-set point
                    min_dist = dist;
                }
            }
            dist_vector[k] = min_dist;
        }
        int best_index = 0;
        float best_dist = dist_vector[0];
        for (int i=1; i<dist_vector.size(); ++i) {
            if (dist_vector[i]>best_dist) {
                best_dist = dist_vector[i];
                best_index = i;
            }
        }
        in_set.insert(best_index);
        in_set_vector.push_back(best_index);
    }
    return in_set_vector;
}
