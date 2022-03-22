#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/eigen.h>
#include <pybind11/stl_bind.h>

#include <vector>
#include <Eigen/Core>
#include "_limap/helpers.h"

#include "refinement/refine.h"
#include "refinement/refinement_config.h"

namespace py = pybind11;

namespace limap {

template <typename DTYPE, int CHANNELS>
void bind_refinement_engine(py::module& m, std::string type_suffix) {
    using namespace refinement;

    using RFEngine = RefinementEngine<DTYPE, CHANNELS>;

    py::class_<RFEngine>(m, ("RefinementEngine" + type_suffix).c_str())
        .def(py::init<>())
        .def(py::init<const RefinementConfig&>())
        .def("Initialize", &RFEngine::Initialize)
        .def("InitializeHeatmaps", &RFEngine::InitializeHeatmaps)
        .def("InitializeVPs", &RFEngine::InitializeVPs)
        .def("InitializeFeatures", &RFEngine::InitializeFeatures)
        .def("InitializeFeaturesAsPatches", &RFEngine::InitializeFeaturesAsPatches)
        .def("SetUp", &RFEngine::SetUp)
        .def("Solve", &RFEngine::Solve)
        .def("GetLine3d", &RFEngine::GetLine3d)
        .def("GetAllStates", &RFEngine::GetAllStates)
        .def("GetHeatmapIntersections", &RFEngine::GetHeatmapIntersections)
        .def("GetFConsistencyIntersections", &RFEngine::GetFConsistencyIntersections);
}

void bind_refinement(py::module &m) {
    using namespace refinement;

    py::class_<RefinementConfig>(m, "RefinementConfig")
        .def(py::init<>())
        .def(py::init<py::dict>())
        .def_readwrite("use_geometric", &RefinementConfig::use_geometric)
        .def_readwrite("min_num_images", &RefinementConfig::min_num_images)
        .def_readwrite("sample_range_min", &RefinementConfig::sample_range_min)
        .def_readwrite("sample_range_max", &RefinementConfig::sample_range_max)
        .def_readwrite("vp_multiplier", &RefinementConfig::vp_multiplier)
        .def_readwrite("n_samples_heatmap", &RefinementConfig::n_samples_heatmap)
        .def_readwrite("heatmap_multiplier", &RefinementConfig::heatmap_multiplier)
        .def_readwrite("n_samples_feature", &RefinementConfig::n_samples_feature)
        .def_readwrite("use_ref_descriptor", &RefinementConfig::use_ref_descriptor)
        .def_readwrite("ref_multiplier", &RefinementConfig::ref_multiplier)
        .def_readwrite("fconsis_multiplier", &RefinementConfig::fconsis_multiplier)
        .def_readwrite("solver_options", &RefinementConfig::solver_options)
        .def_readwrite("heatmap_interpolation_config", &RefinementConfig::heatmap_interpolation_config)
        .def_readwrite("feature_interpolation_config", &RefinementConfig::feature_interpolation_config)
        .def_readwrite("geometric_loss_function", &RefinementConfig::geometric_loss_function)
        .def_readwrite("geometric_alpha", &RefinementConfig::geometric_alpha)
        .def_readwrite("vp_loss_function", &RefinementConfig::vp_loss_function)
        .def_readwrite("heatmap_loss_function", &RefinementConfig::heatmap_loss_function)
        .def_readwrite("fconsis_loss_function", &RefinementConfig::fconsis_loss_function)
        .def_readwrite("print_summary", &RefinementConfig::print_summary);

#define REGISTER_CHANNEL(CHANNELS) \
    bind_refinement_engine<float16, CHANNELS>(m, "_f16_c" + std::to_string(CHANNELS)); \
    bind_refinement_engine<float, CHANNELS>(m, "_f32_c" + std::to_string(CHANNELS)); \
    bind_refinement_engine<double, CHANNELS>(m, "_f64_c" + std::to_string(CHANNELS)); 

    REGISTER_CHANNEL(1);
    REGISTER_CHANNEL(3);
    REGISTER_CHANNEL(128);

#undef REGISTER_CHANNEL
}

} // namespace limap

