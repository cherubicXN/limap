#include <pybind11/pybind11.h>
#include <pybind11/stl_bind.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>

#include <chrono>
#include <thread>

namespace py = pybind11;

#include "base/bindings.cc"
#include "ceresbase/bindings.cc"
#include "sfm/bindings.cc"
#include "triangulation/bindings.cc"
#include "merging/bindings.cc"
#include "undistortion/bindings.cc"
#include "refinement/bindings.cc"
#include "features/bindings.cc"
#include "lineKA/bindings.cc"
#include "lineBA/bindings.cc"
#include "vpdetection/bindings.cc"
#include "evaluation/bindings.cc"

#include "_limap/helpers.h"

void bind_base(py::module &);
void bind_ceresbase(py::module &);
void bind_sfm(py::module &);
void bind_triangulation(py::module &);
void bind_merging(py::module &);
void bind_undistortion(py::module &);
void bind_refinement(py::module &);
void bind_features(py::module &);
void bind_lineKA(py::module &);
void bind_lineBA(py::module &);
void bind_vpdetection(py::module &);
void bind_evaluation(py::module &);

namespace limap {

PYBIND11_MODULE(_limap, m) {
    m.doc() = "Multi-view Line Triangulation and Refinement";

    py::add_ostream_redirect(m, "ostream_redirect");

    pybind11::module_ _b = m.def_submodule("_base");
    pybind11::module_ _ceresb = m.def_submodule("_ceresbase");
    pybind11::module_ _sfm = m.def_submodule("_sfm");
    pybind11::module_ _tri = m.def_submodule("_triangulation");
    pybind11::module_ _mrg = m.def_submodule("_merging");
    pybind11::module_ _undist = m.def_submodule("_undistortion");
    pybind11::module_ _trf = m.def_submodule("_refinement");
    pybind11::module_ _f = m.def_submodule("_features");
    pybind11::module_ _lineka = m.def_submodule("_lineKA");
    pybind11::module_ _lineba = m.def_submodule("_lineBA");
    pybind11::module_ _vpdet = m.def_submodule("_vpdet");
    pybind11::module_ _eval = m.def_submodule("_evaluation");

    // bind modules
    bind_base(_b);
    bind_ceresbase(_ceresb);
    bind_sfm(_sfm);
    bind_triangulation(_tri);
    bind_merging(_mrg);
    bind_undistortion(_undist);
    bind_refinement(_trf);
    bind_features(_f);
    bind_lineKA(_lineka);
    bind_lineBA(_lineba);
    bind_vpdetection(_vpdet);
    bind_evaluation(_eval);
}

}
