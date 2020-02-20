#include <iostream>
#include "fps/fps.h"

int main(void) {
    int N = 10000;
    int D = 6;
    int M = 1000;
    std::cin >> N >> D >> M;
    Eigen::MatrixXd pts = Eigen::MatrixXd::Random(N, D);

    boost::container::flat_set<int> s = farthest_point_sampling(pts, M);
    for (auto k : s) {
        std::cout << k << ' ';
    }
    std::cout << std::endl;
    return 0;
}
