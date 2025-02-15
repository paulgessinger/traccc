/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2021 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

#include <seeding/detail/doublet.hpp>

namespace traccc {

/// Header: the number of triplets per spacepoint bin
struct triplet_per_bin {
    unsigned int n_triplets = 0;

    TRACCC_HOST_DEVICE
    unsigned int get_ref_num() const { return n_triplets; }

    TRACCC_HOST_DEVICE
    void zeros() { n_triplets = 0; }
};

/// Item: triplets of middle-bottom-top
struct triplet {
    // middle spacepoint location in internal spacepoint container
    sp_location sp1;
    // bottom spacepoint location in internal spacepoint container
    sp_location sp2;
    // top spacepoint location in internal spacepoint container
    sp_location sp3;
    // curvtaure of circle estimated from triplet
    scalar curvature;
    // weight of triplet
    scalar weight;
    // z origin of triplet
    scalar z_vertex;
};

inline TRACCC_HOST_DEVICE bool operator==(const triplet& lhs,
                                          const triplet& rhs) {
    return (lhs.sp1.bin_idx == rhs.sp1.bin_idx &&
            lhs.sp1.sp_idx == rhs.sp1.sp_idx &&
            lhs.sp2.bin_idx == rhs.sp2.bin_idx &&
            lhs.sp2.sp_idx == rhs.sp2.sp_idx &&
            lhs.sp3.bin_idx == rhs.sp3.bin_idx &&
            lhs.sp3.sp_idx == rhs.sp3.sp_idx);
}

inline TRACCC_HOST_DEVICE bool operator<(const triplet& lhs,
                                         const triplet& rhs) {
    return lhs.weight < rhs.weight;
}

/// Container of triplet belonging to one detector module
template <template <typename> class vector_t>
using triplet_collection = vector_t<triplet>;

/// Convenience declaration for the triplet collection type to use in host code
using host_triplet_collection = triplet_collection<vecmem::vector>;

/// Convenience declaration for the triplet collection type to use in device
/// code
using device_triplet_collection = triplet_collection<vecmem::device_vector>;

/// Convenience declaration for the triplet container type to use in host code
using host_triplet_container = host_container<triplet_per_bin, triplet>;

/// Convenience declaration for the triplet container type to use in device code
using device_triplet_container = device_container<triplet_per_bin, triplet>;

/// Convenience declaration for the triplet container data type to use in host
/// code
using triplet_container_data = container_data<triplet_per_bin, triplet>;

/// Convenience declaration for the triplet container buffer type to use in host
/// code
using triplet_container_buffer = container_buffer<triplet_per_bin, triplet>;

/// Convenience declaration for the triplet container view type to use in host
/// code
using triplet_container_view = container_view<triplet_per_bin, triplet>;

}  // namespace traccc
