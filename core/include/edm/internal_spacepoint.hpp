/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2021 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

// traccc include
#include "edm/spacepoint.hpp"

// VecMem include(s).
#include <vecmem/containers/data/jagged_vector_buffer.hpp>
#include <vecmem/containers/data/vector_buffer.hpp>
#include <vecmem/containers/device_vector.hpp>
#include <vecmem/containers/jagged_device_vector.hpp>
#include <vecmem/containers/jagged_vector.hpp>
#include <vecmem/containers/vector.hpp>

// std
#include <algorithm>

// detray core
#include <detray/definitions/invalid_values.hpp>

namespace traccc {

/// Item: A internal spacepoint definition
template <typename spacepoint_t>
struct internal_spacepoint {

    // FIXME: geometry_id is hard-coded here
    using spacepoint_container_type = host_container<geometry_id, spacepoint_t>;
    using link_type = typename spacepoint_container_type::link_type;

    link_type m_link;

    scalar m_x;
    scalar m_y;
    scalar m_z;
    scalar m_r;
    scalar m_varianceR;
    scalar m_varianceZ;

    internal_spacepoint() = default;

    template <typename spacepoint_container_t>
    TRACCC_HOST_DEVICE internal_spacepoint(
        const spacepoint_container_t& sp_container, const link_type& sp_link,
        const vector2& offsetXY)
        : m_link(std::move(sp_link)) {
        const spacepoint_t& sp = sp_container.at(sp_link);
        m_x = sp.global[0] - offsetXY[0];
        m_y = sp.global[1] - offsetXY[1];
        m_z = sp.global[2];
        m_r = std::sqrt(m_x * m_x + m_y * m_y);

        // Need to fix this part
        m_varianceR = sp.variance[0];
        m_varianceZ = sp.variance[1];
    }

    TRACCC_HOST_DEVICE
    internal_spacepoint(const link_type& sp_link) : m_link(std::move(sp_link)) {

        m_x = 0;
        m_y = 0;
        m_z = 0;
        m_r = 0;
        m_varianceR = 0;
        m_varianceZ = 0;
    }

    TRACCC_HOST_DEVICE
    internal_spacepoint(const internal_spacepoint<spacepoint_t>& sp)
        : m_link(std::move(sp.m_link)) {

        m_x = sp.m_x;
        m_y = sp.m_y;
        m_z = sp.m_z;
        m_r = sp.m_r;
        m_varianceR = sp.m_varianceR;
        m_varianceZ = sp.m_varianceZ;
    }

    TRACCC_HOST_DEVICE
    internal_spacepoint& operator=(
        const internal_spacepoint<spacepoint_t>& sp) {

        m_link.first = sp.m_link.first;
        m_link.second = sp.m_link.second;
        m_x = sp.m_x;
        m_y = sp.m_y;
        m_z = sp.m_z;
        m_r = sp.m_r;
        m_varianceR = sp.m_varianceR;
        m_varianceZ = sp.m_varianceZ;

        return *this;
    }

    TRACCC_HOST_DEVICE
    static inline internal_spacepoint<spacepoint_t> invalid_value() {

        link_type l = {detray::invalid_value<decltype(l.first)>(),
                       detray::invalid_value<decltype(l.second)>()};

        return internal_spacepoint<spacepoint_t>({std::move(l)});
    }

    TRACCC_HOST_DEVICE const scalar& x() const { return m_x; }

    TRACCC_HOST_DEVICE
    const scalar& y() const { return m_y; }

    TRACCC_HOST_DEVICE
    const scalar& z() const { return m_z; }

    TRACCC_HOST_DEVICE
    const scalar& radius() const { return m_r; }

    TRACCC_HOST_DEVICE
    scalar phi() const { return atan2f(m_y, m_x); }

    TRACCC_HOST_DEVICE
    const scalar& varianceR() const { return m_varianceR; }

    TRACCC_HOST_DEVICE
    const scalar& varianceZ() const { return m_varianceZ; }
};

template <typename spacepoint_t>
inline bool operator<(const internal_spacepoint<spacepoint_t>& lhs,
                      const internal_spacepoint<spacepoint_t>& rhs) {
    return (lhs.radius() < rhs.radius());
}

}  // namespace traccc
