#include "fps/fps.h"
boost::container::flat_set<int> farthest_point_sampling(const Eigen::MatrixXd &pts, int M) {
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
    boost::container::flat_set<int> in_set; // indices to be return
    std::uniform_int_distribution<int> uniform_N(0, N-1);
    int old_idx = uniform_N(mt);
    M = std::min(M, N);
    in_set.insert(old_idx);
    for (int j=1; j<M; ++j) {
        Compare best(0, -1.0);
        #pragma omp parallel for reduction(maximum: best) shared(pts, old_idx, in_set, N)
        for (int k=0; k<N; ++k) {
            if (in_set.count(k)>0) continue;
            float dist = (pts.row(old_idx)-pts.row(k)).norm();
            if (dist>best.second) {
                best.first = k;
                best.second = dist;
            }
        }
        in_set.insert(best.first);
        old_idx = best.first;
    }
    return in_set;
}

