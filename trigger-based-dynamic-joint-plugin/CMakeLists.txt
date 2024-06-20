cmake_minimum_required(VERSION 3.8)
project(trigger_based_dynamic_joint_plugin)

# Find Gazebo
find_package(ament_cmake REQUIRED)
find_package(gz_cmake_vendor REQUIRED)
find_package(gz-cmake3 REQUIRED)
find_package(gz_physics_vendor REQUIRED)
find_package(gz-physics REQUIRED)
find_package(gz_utils_vendor REQUIRED)
find_package(gz-utils REQUIRED)
find_package(gz_plugin_vendor REQUIRED)
find_package(gz-plugin REQUIRED)
find_package(Eigen3 REQUIRED)
find_package(gz_math_vendor REQUIRED)
find_package(gz-math REQUIRED)
find_package(gz_common_vendor REQUIRED)
find_package(gz-common REQUIRED)
#find_package(gz_msgs_vendor REQUIRED)
#find_package(gz-msgs REQUIRED)
find_package(gz_sim_vendor REQUIRED)
find_package(gz-sim REQUIRED)

# include_directories(${GAZEBO_INCLUDE_DIRS})
# Include directories
include_directories(
  ${gz-cmake3_INCLUDE_DIRS}
  ${gz-physics7_INCLUDE_DIRS}
  ${gz-utils2_INCLUDE_DIRS}
  ${gz-plugin2_INCLUDE_DIRS}
  ${EIGEN3_INCLUDE_DIRS}
  ${gz-math7_INCLUDE_DIRS}
  ${gz-common5_INCLUDE_DIRS}
  #${gz-msgs10_INCLUDE_DIRS}
  ${gz-sim8_INCLUDE_DIRS}
)
link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# Build our plugin
add_library(trigger_based_dynamic_joint_plugin SHARED adversarial_joints_plugin.cc)
target_link_libraries(trigger_based_dynamic_joint_plugin
  ${gz-cmake3_LIBRARIES}
  ${gz-physics7_LIBRARIES}
  ${gz-utils2_LIBRARIES}
  ${gz-plugin2_LIBRARIES}
  ${EIGEN3_LIBRARIES}
  ${gz-math7_LIBRARIES}
  ${gz-common5_LIBRARIES}
  #${gz-msgs10_LIBRARIES}
  ${gz-sim8_LIBRARIES}
)
