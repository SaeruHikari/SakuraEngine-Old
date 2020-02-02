#CMakeLists for Math 
add_subdirectory(GeoMath)

set( EIGEN3_INCLUDE_DIR "$ENV{EIGEN_INCLUDE}" )
if( NOT EIGEN3_INCLUDE_DIR )
    message( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
endif()
include_directories( "$ENV{EIGEN_INCLUDE}")
