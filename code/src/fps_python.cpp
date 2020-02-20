#include "fps_python.h"
PYBIND11_MODULE(fps, m) {
  namespace py = pybind11;
  m.doc() = "Farthest Point Sampling (C++11)"; // optional module docstring
  m.def("farthest_point_sampling", py::overload_cast<const Eigen::MatrixXd&, int>(&farthest_point_sampling), "ND Farthest Point Sampling");
  m.def("farthest_point_sampling_masked", py::overload_cast<const Eigen::MatrixXd&, const std::vector<int>&, int>(&farthest_point_sampling), "ND Farthest Point Sampling with mask");
}

