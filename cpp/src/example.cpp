#include <iostream>
#include <vector>
#include <chrono>
#include "fps/fps.h"

int main(void) {
    int N = 10000;
    int D = 6;
    int K = 1;
    std::cin >> N >> D >> K;
    Eigen::MatrixXd pts = Eigen::MatrixXd::Random(N, D);
    std::vector<int> Ms;
    std::vector< std::vector<int> > ss;

    for (int k=0; k<K; ++k) {
        int v; std::cin >> v;
        Ms.push_back(v);
    }

    std::chrono::steady_clock::time_point ts_begin = std::chrono::steady_clock::now();
    for (int i=0; i<K; ++i) {
        int M = Ms[i];
        ss.emplace_back((i==0) ? farthest_point_sampling(pts, M) : farthest_point_sampling(pts, ss[i-1], M));
        /*
        for (auto v : ss[i]) std::cout << v << ' ';
        std::cout << std::endl;
        */
    }
    std::chrono::steady_clock::time_point ts_end = std::chrono::steady_clock::now();

    std::vector<int> buffer[2]; // rolling buffer
    for (int i=0; i<ss.size(); ++i) {
        auto &layer_idx = ss[i];
        std::vector<int> &mapping = buffer[i&1];
        std::vector<int> &store = buffer[(i+1)&1];
        if (mapping.size()==0) {
            store = layer_idx;
        } else {
            store.clear();
            store.reserve(layer_idx.size());
            for (auto v : layer_idx) {
                store.push_back(mapping[v]);
            }
        }
        /*
        for (auto v : store) std::cout << v << ' ';
        std::cout << std::endl;
        */
    }
    std::cout << "Elapsed Time (subsampling) => " << std::chrono::duration_cast<std::chrono::microseconds>(ts_end - ts_begin).count()*1e-6 << " seconds" << std::endl;
    return 0;
}
