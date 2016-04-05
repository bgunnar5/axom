# Please add configuration options here.
# This provides a central location to check options and their defaults and ensure
# these are declared at the top of the cmake configuration.

option(BUILD_SHARED_LIBS "Build shared libraries." OFF)
option(BUILD_DOCS "Builds documentation" ON)
option(BUILD_EXAMPLES "Builds examples" ON)
option(BUILD_TESTS "Builds tests" ON)
option(ENABLE_CODECOV "Enable code coverage via gcov." OFF)
option(ENABLE_CXX11 "Enables C++11 language support." ON)
option(ENABLE_BOOST "Enable Boost" OFF)
option(ENABLE_FORTRAN "Enables Fortran compiler support." ON)
option(ENABLE_GLOBALCOMPILERWARNINGS "Enable extra compiler warnings on all build targets." ON)
option(ENABLE_GLOBALCOMPILERWARNINGSASERRORS "Enable treating compiler warnings as errors on all build targets." OFF)
option(ENABLE_MPI "Enables MPI compiler support" OFF)
option(ENABLE_OPENMP "Enables OpenMP compiler support" OFF)
option(ENABLE_BENCHMARKS "Enables google benchmark" OFF)
option(ENABLE_PYTHON "Enables python use." ON)
option(ENABLE_SHROUD_API_GENERATOR "Enables shroud interface generator for C and Fortran." ON)
option(ENABLE_CFORTRAN_API "Enables Fortran interface for components." ON)
