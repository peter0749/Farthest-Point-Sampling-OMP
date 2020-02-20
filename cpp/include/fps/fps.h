#ifndef __HEADER_FAREST_POINT_SAMPLING_
#define __HEADER_FAREST_POINT_SAMPLING_
/*System*/
// #include <unordered_set>
#include <random>
#include <omp.h>

/*Eigen*/
#include <Eigen/LU>

/*Boost*/
#include <boost/container/flat_set.hpp>

/*Pybind11*/
// #include <pybind11/pybind11.h>
// #include <pybind11/eigen.h>

typedef std::pair<unsigned int, float> Compare;
#pragma omp declare reduction(maximum : Compare : omp_out = omp_in.second > omp_out.second ? omp_in : omp_out) initializer(omp_priv = Compare(0, -1.0))

boost::container::flat_set<int> farthest_point_sampling(const Eigen::MatrixXd &pts, int M);
#endif
