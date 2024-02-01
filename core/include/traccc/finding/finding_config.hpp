/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2023 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// detray include(s).
#include "detray/definitions/units.hpp"

namespace traccc {

/// Configuration struct for track finding
template <typename scalar_t>
struct finding_config {
    /// @NOTE: This paramter might be removed
    unsigned int max_num_branches_per_seed = 100;

    /// Maximum number of branches per surface
    unsigned int max_num_branches_per_surface = 10;

    /// Min/Max number of track candidates per track
    unsigned int min_track_candidates_per_track = 3;
    unsigned int max_track_candidates_per_track = 10000;

    /// Maximum number of branches per initial seed
    unsigned int max_num_branches_per_initial_seed =
        std::numeric_limits<unsigned int>::max();

    /// Minimum step length that track should make to reach the next surface. It
    /// should be set higher than the overstep tolerance not to make it stay on
    /// the same surface
    scalar_t min_step_length_for_surface_aborter =
        0.1f * detray::unit<scalar_t>::mm;
    /// Maximum Chi-square that is allowed for branching
    scalar_t chi2_max = 30.f;

    /// Constrained step size for propagation
    /// @TODO: Make a separate file for propagation config?
    scalar_t step_constraint = std::numeric_limits<scalar_t>::max();

    scalar_t overstep_tolerance = -100 * detray::unit<scalar_t>::um;
    scalar_t mask_tolerance = 15.f * detray::unit<scalar_t>::um;
    scalar_t rk_tolerance = 1e-4;

    /// GPU-specific parameter for the number of measurements to be
    /// iterated per thread
    unsigned int n_measurements_per_thread = 8;
};

}  // namespace traccc
