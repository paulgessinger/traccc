/** TRACCC library, part of the ACTS project (R&D line)
 *
 * (c) 2021 CERN for the benefit of the ACTS project
 *
 * Mozilla Public License Version 2.0
 */

#pragma once

#include "detail/sparse_ccl.hpp"
#include "edm/cell.hpp"
#include "edm/cluster.hpp"
#include "utils/algorithm.hpp"

namespace traccc {

/// Connected component labelling
///
/// Note that the separation between the public and private interface is
/// only there in the class because the compilers can't automatically figure
/// out the "vector type" of the templated implementation, without adding a
/// lot of "internal knowledge" about the vector types into this piece of
/// code. So instead the public operators are specifically implemented for
/// the host- and device versions of the EDM, making use of a single
/// implementation internally.
///
class component_connection
    : public algorithm<cluster_collection(const host_cell_collection&,
                                          const cell_module&)>,
      public algorithm<cluster_collection(const device_cell_collection&,
                                          const cell_module&)> {
    public:
    /// Constructor for component_connection
    ///
    /// @param mr is the memory resource
    component_connection(vecmem::memory_resource& mr) : m_mr(mr) {}

    /// @name Operators to use in host code
    /// @{

    /// Callable operator for the connected component, based on one single
    /// module
    ///
    /// @param cells are the input cells into the connected component, they are
    ///              per module and unordered
    /// @param module The description of the module that the cells belong to
    ///
    /// c++20 piping interface:
    /// @return a cluster collection
    ///
    cluster_collection operator()(const host_cell_collection& cells,
                                  const cell_module& module) const override {
        return this->operator()<vecmem::vector>(cells, module);
    }

    /// @}

    /// @name Operators to use in device code
    /// @{

    /// Callable operator for the connected component, based on one single
    /// module
    ///
    /// This version of the function is meant to be used in device code.
    ///
    /// @param cells are the input cells into the connected component, they are
    ///              per module and unordered
    /// @param module The description of the module that the cells belong to
    ///
    /// c++20 piping interface:
    /// @return a cluster collection
    ///
    cluster_collection operator()(const device_cell_collection& cells,
                                  const cell_module& module) const override {
        return this->operator()<vecmem::device_vector>(cells, module);
    }

    private:
    /// Implementation for the public cell collection creation operators
    template <template <typename> class vector_type>
    cluster_collection operator()(const cell_collection<vector_type>& cells,
                                  const cell_module& module) const {
        cluster_collection clusters;
        clusters.placement = module.placement;
        this->operator()<vector_type>(cells, module, clusters);
        return clusters;
    }

    /// Implementation for the public cell collection creation operators
    template <template <typename> class vector_type>
    void operator()(const cell_collection<vector_type>& cells,
                    const cell_module& module,
                    cluster_collection& clusters) const {
        // Assign the module id
        clusters.module = module.module;
        // Run the algorithm
        auto connected_cells = detail::sparse_ccl<vector_type>(cells);
        std::vector<cluster> cluster_items(std::get<0>(connected_cells),
                                           cluster{});
        unsigned int icell = 0;
        for (auto cell_label : std::get<1>(connected_cells)) {
            auto cindex = static_cast<unsigned int>(cell_label - 1);
            if (cindex < cluster_items.size()) {
                cluster_items[cindex].cells.push_back(cells[icell++]);
            }
        }
        clusters.items = cluster_items;
    }

    private:
    std::reference_wrapper<vecmem::memory_resource> m_mr;

};  // class component_connection

}  // namespace traccc
