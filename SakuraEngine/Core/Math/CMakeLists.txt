#Eigen3 Include

set( EIGEN3_INCLUDE_DIR "$ENV{EIGEN_INCLUDE}" )
if( NOT EIGEN3_INCLUDE_DIR )
    message( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
endif()
include_directories("${EIGEN3_INCLUDE_DIR}")
message( STATUS "Add Eigen3 Root: ${EIGEN3_INCLUDE_DIR}")

#CMakeLists for Math 
add_subdirectory(GeoMath)