# TRACCC library, part of the ACTS project (R&D line)
#
# (c) 2021 CERN for the benefit of the ACTS project
#
# Mozilla Public License Version 2.0

# We need the SYCL language in this directory.
enable_language( SYCL )

# Example executable(s).
add_executable( traccc_sycl_language_example "sycl_language_example.sycl" )
