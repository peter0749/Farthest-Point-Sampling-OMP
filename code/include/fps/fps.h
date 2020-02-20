#ifndef __HEADER_FAREST_POINT_SAMPLING_
#define __HEADER_FAREST_POINT_SAMPLING_
/*System*/
// #include <unordered_set>
#include <random>
#ifdef USE_OMP
#include <omp.h>
#endif

/*Eigen*/
#include <Eigen/LU>

/*Boost*/
// #include <boost/unordered_set.hpp>

/*Fast HASH TABLE*/
#include "flat_hash_map.hpp"

/*Pybind11*/
// #include <pybind11/pybind11.h>
// #include <pybind11/eigen.h>

typedef std::pair<unsigned int, float> Compare;
#pragma omp declare reduction(maximum : Compare : omp_out = omp_in.second > omp_out.second ? omp_in : omp_out) initializer(omp_priv = Compare(0, -1.0))

std::vector<int> farthest_point_sampling(const Eigen::MatrixXd &pts, int M);
std::vector<int> farthest_point_sampling(const Eigen::MatrixXd &pts, const std::vector<int> &mask, int M);
#endif
