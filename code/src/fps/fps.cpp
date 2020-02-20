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
    ska::flat_hash_set<int> in_set; // indices to be return
    std::uniform_int_distribution<int> uniform_N(0, N-1);
    int old_idx = uniform_N(mt);
    M = std::min(M, N);
    in_set.insert(old_idx);
    for (int j=1; j<M; ++j) {
        Compare best(0, -1.0);
        #ifdef USE_OMP
        #pragma omp parallel for reduction(maximum: best) shared(pts, old_idx, in_set, N)
        #endif
        for (int k=0; k<N; ++k) {
            if (in_set.count(k)>0) continue;
            float min_dist = FLT_MAX;
            for (auto v_in : in_set) {
                float dist = (pts.row(v_in)-pts.row(k)).norm();
                if (dist<min_dist) {
                    min_dist = dist;
                }
            }
            if (min_dist>best.second) {
                best.first = k;
                best.second = min_dist;
            }
        }
        in_set.insert(best.first);
        old_idx = best.first;
    }
    std::vector<int> in_set_vector; 
    in_set_vector.reserve(in_set.size());
    for (auto v : in_set) in_set_vector.push_back(v);
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
    ska::flat_hash_set<int> in_set; // indices to be return
    std::uniform_int_distribution<int> uniform_N(0, N-1);
    int old_idx = uniform_N(mt);
    M = std::min(M, N);
    in_set.insert(old_idx);
    for (int j=1; j<M; ++j) {
        Compare best(0, -1.0);
        #ifdef USE_OMP
        #pragma omp parallel for reduction(maximum: best) shared(pts, old_idx, in_set, N, mask)
        #endif
        for (int m=0; m<N; ++m) {
            if (in_set.count(m)>0) continue;
            float min_dist = FLT_MAX;
            for (auto v_in : in_set) {
                float dist = (pts.row(mask[v_in])-pts.row(mask[m])).norm();
                if (dist<min_dist) {
                    min_dist = dist;
                }
            }
            if (min_dist>best.second) {
                best.first = m;
                best.second = min_dist;
            }
        }
        in_set.insert(best.first);
        old_idx = best.first;
    }
    std::vector<int> in_set_vector; 
    in_set_vector.reserve(in_set.size());
    for (auto v : in_set) in_set_vector.push_back(v);
    return in_set_vector;
}
