#ifndef LIMAP_ESTIMATORS_POSE_HYBRID_POSE_ESTIMATOR_H_
#define LIMAP_ESTIMATORS_POSE_HYBRID_POSE_ESTIMATOR_H_

#include "_limap/helpers.h"
#include "util/types.h"
#include "base/linetrack.h"
#include "base/camera.h"
#include "optimize/line_localization/lineloc.h"
#include "estimators/absolute_pose/joint_pose_estimator.h"
#include "estimators/extended_hybrid_ransac.h"

#include <RansacLib/ransac.h>

namespace limap {

using namespace optimize::line_localization;

namespace estimators {

namespace absolute_pose {

class HybridPoseEstimator : public JointPoseEstimator {
public:
    HybridPoseEstimator(const std::vector<Line3d>& l3ds, const std::vector<int>& l3d_ids, const std::vector<Line2d>& l2ds, 
                        const std::vector<V3D>& p3ds, const std::vector<V2D>& p2ds, 
                        const Camera& cam, const LineLocConfig& cfg,
                        const double cheirality_min_depth = 0.0, 
                        const double line_min_projected_length = 1) :
                        JointPoseEstimator(l3ds, l3d_ids, l2ds, p3ds, p2ds, cam, cfg, cheirality_min_depth, line_min_projected_length) {}

    inline int num_minimal_solvers() const { return 4; }

    void min_sample_sizes(std::vector<std::vector<int>>* min_sample_sizes) const {
        min_sample_sizes->resize(4);
        (*min_sample_sizes)[0] = std::vector<int>{3, 0};
        (*min_sample_sizes)[1] = std::vector<int>{2, 1};
        (*min_sample_sizes)[2] = std::vector<int>{1, 2};
        (*min_sample_sizes)[3] = std::vector<int>{0, 3};
    }

    inline int num_data_types() const { return 2; }

    void num_data(std::vector<int>* num_data) const {
        num_data->resize(2);
        (*num_data)[0] = num_data_points();
        (*num_data)[1] = num_data_lines();
    }

    void solver_probabilities(std::vector<double>* solver_probabilites) const {
        solver_probabilites->resize(4);
        for (int i = 0; i < 4; i++) {
            if (!solver_flags_[i])
                (*solver_probabilites)[i] = 0.0;
            else
                (*solver_probabilites)[i] = 1.0;
        }
    }

    int NonMinimalSolver(const std::vector<int>& sample,
            CameraPose* pose) const {
        return JointPoseEstimator::NonMinimalSolver(sample, pose);
    }

    int MinimalSolver(const std::vector<std::vector<int>>& sample,
                    const int solver_idx, std::vector<CameraPose>* poses) const;

    double EvaluateModelOnPoint(const CameraPose& pose, int t, int i) const;

    void LeastSquares(const std::vector<std::vector<int>>& sample, CameraPose* poses) const {
        std::vector<int> samples;
        for (int i = 0; i < sample[0].size(); i++)
            samples.push_back(sample[0][i]);
        for (int i = 0; i < sample[1].size(); i++)
            samples.push_back(num_data_points() + sample[1][i]);
        JointPoseEstimator::LeastSquares(samples, poses);
    }

    void set_solver_flags(const std::vector<bool>& flags) {
        THROW_CHECK_EQ(flags.size(), 4);
        solver_flags_ = flags;
    }

private:
    std::vector<bool> solver_flags_;
};

std::pair<CameraPose, ransac_lib::HybridRansacStatistics> 
EstimateAbsolutePose_PointLine_Hybrid(const std::vector<Line3d>& l3ds, const std::vector<int>& l3d_ids, 
                                      const std::vector<Line2d>& l2ds, const std::vector<V3D>& p3ds, 
                                      const std::vector<V2D>& p2ds, const Camera& cam, 
                                      const ExtendedHybridLORansacOptions& options_ = ExtendedHybridLORansacOptions(), 
                                      const LineLocConfig& cfg = LineLocConfig(),
                                      const std::vector<bool>& solver_flags = std::vector<bool>{true, true, true, true},
                                      const double cheirality_min_depth = 0.0, 
                                      const double line_min_projected_length = 1);

} // namespace pose

} // namespace estimators

} // namespace limap

#endif

