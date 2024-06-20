cmake_minimum_required(VERSION 3.8)
project(floorplan_gazebo_plugins)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find colcon macros and libraries
## if COMPONENTS list like find_package(COLCON REQUIRED COMPONENTS xyz)
## is used, also find other colcon packages
#find_package(COLCON REQUIRED COMPONENTS
#  gz_cmake_vendor
#  gz-cmake3
#  gz_physics_vendor
#  gz-physics
#  gz_utils_vendor
#  gz-utils
#  gz_plugin_vendor
#  gz-plugin
#  Eigen3
#  gz_math_vendor
#  gz-math
#  gz_common_vendor
#  gz-common
#  # gz_msgs_vendor
#  # gz-msgs
#  gz_sim_vendor
#  gz-sim
#)

# Find Gazebo
# find_package(ament_cmake REQUIRED)
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

## System dependencies are found with CMake's conventions
# find_package(Boost REQUIRED COMPONENTS system)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")
link_directories(${GAZEBO_LIBRARY_DIRS})
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


## Uncomment this if the package has a setup.py. This macro ensures
## modules and global scripts declared therein get installed
## See http://ros.org/doc/api/colcon/html/user_guide/setup_dot_py.html
# colcon_python_setup()

################################################
## Declare ROS messages, services and actions ##
################################################

## To declare and build messages, services or actions from within this
## package, follow these steps:
## * Let MSG_DEP_SET be the set of packages whose message types you use in
##   your messages/services/actions (e.g. std_msgs, actionlib_msgs, ...).
## * In the file package.xml:
##   * add a build_depend tag for "message_generation"
##   * add a build_depend and a exec_depend tag for each package in MSG_DEP_SET
##   * If MSG_DEP_SET isn't empty the following dependency has been pulled in
##     but can be declared for certainty nonetheless:
##     * add a exec_depend tag for "message_runtime"
## * In this file (CMakeLists.txt):
##   * add "message_generation" and every package in MSG_DEP_SET to
##     find_package(COLCON REQUIRED COMPONENTS ...)
##   * add "message_runtime" and every package in MSG_DEP_SET to
##     colcon_package(COLCON_DEPENDS ...)
##   * uncomment the add_*_files sections below as needed
##     and list every .msg/.srv/.action file to be processed
##   * uncomment the generate_messages entry below
##   * add every package in MSG_DEP_SET to generate_messages(DEPENDENCIES ...)

## Generate messages in the 'msg' folder
# add_message_files(
#   FILES
#   Message1.msg
#   Message2.msg
# )

## Generate services in the 'srv' folder
# add_service_files(
#   FILES
#   Service1.srv
#   Service2.srv
# )

## Generate actions in the 'action' folder
# add_action_files(
#   FILES
#   Action1.action
#   Action2.action
# )

## Generate added messages and services with any dependencies listed here
# generate_messages(
#   DEPENDENCIES
#   std_msgs  # Or other packages containing msgs
# )

################################################
## Declare ROS dynamic reconfigure parameters ##
################################################

## To declare and build dynamic reconfigure parameters within this
## package, follow these steps:
## * In the file package.xml:
##   * add a build_depend and a exec_depend tag for "dynamic_reconfigure"
## * In this file (CMakeLists.txt):
##   * add "dynamic_reconfigure" to
##     find_package(COLCON REQUIRED COMPONENTS ...)
##   * uncomment the "generate_dynamic_reconfigure_options" section below
##     and list every .cfg file to be processed

## Generate dynamic reconfigure parameters in the 'cfg' folder
# generate_dynamic_reconfigure_options(
#   cfg/DynReconf1.cfg
#   cfg/DynReconf2.cfg
# )

###################################
## colcon specific configuration ##
###################################
## The colcon_package macro generates cmake config files for your package
## Declare things to be passed to dependent projects
## INCLUDE_DIRS: uncomment this if your package contains header files
## LIBRARIES: libraries you create in this project that dependent projects also need
## colcon_DEPENDS: colcon_packages dependent projects also need
## DEPENDS: system dependencies of this project that dependent projects also need
#colcon_package(
##  INCLUDE_DIRS include
##  LIBRARIES floorplan_gazebo_plugins
##  COLCON_DEPENDS gazebo_ros
##  DEPENDS system_lib
#)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
#include_directories(
## include
#  ${COLCON_INCLUDE_DIRS}
#)



## Declare a C++ library
# add_library(${PROJECT_NAME}
#   src/${PROJECT_NAME}/floorplan_gazebo_plugins.cpp
# )

## Add cmake target dependencies of the library
## as an example, code may need to be generated before libraries
## either from message generation or dynamic reconfigure
# add_dependencies(${PROJECT_NAME} ${${PROJECT_NAME}_EXPORTED_TARGETS} ${COLCON_EXPORTED_TARGETS})

## Declare a C++ executable
## With colcon_make all packages are built within a single CMake context
## The recommended prefix ensures that target names across packages don't collide
# add_executable(${PROJECT_NAME}_node src/floorplan_gazebo_plugins_node.cpp)

## Rename C++ executable without prefix
## The above recommended prefix causes long target names, the following renames the
## target back to the shorter version for ease of user use
## e.g. "rosrun someones_pkg node" instead of "rosrun someones_pkg someones_pkg_node"
# set_target_properties(${PROJECT_NAME}_node PROPERTIES OUTPUT_NAME node PREFIX "")

## Add cmake target dependencies of the executable
## same as for the library above
# add_dependencies(${PROJECT_NAME}_node ${${PROJECT_NAME}_EXPORTED_TARGETS} ${COLCON_EXPORTED_TARGETS})

## Specify libraries to link a library or executable target against
# target_link_libraries(${PROJECT_NAME}_node
#   ${COLCON_LIBRARIES}
# )
add_library(initial_plugin SHARED src/initial_plugin.cc)
target_link_libraries(initial_plugin
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

add_library(dynamic_joint_plugin SHARED src/dynamic_joints_plugin.cc)
target_link_libraries(dynamic_joint_plugin
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

add_library(adversarial_joint_plugin SHARED src/adversarial_joints_plugin.cc)
target_link_libraries(adversarial_joint_plugin
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

#############
## Install ##
#############

# all install targets should use colcon DESTINATION variables
# See http://ros.org/doc/api/colcon/html/adv_user_guide/variables.html

install(TARGETS
  initial_plugin
  dynamic_joint_plugin
  adversarial_joint_plugin
  # DESTINATION ${COLCON_PACKAGE_BIN_DESTINATION} 
  # LIBRARY DESTINATION ${COLCON_PACKAGE_LIB_DESTINATION}
  ARCHIVE DESTINATION ${COLCON_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${COLCON_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${COLCON_GLOBAL_BIN_DESTINATION}
)

## Mark executable scripts (Python etc.) for installation
## in contrast to setup.py, you can choose the destination
# colcon_install_python(PROGRAMS
#   scripts/my_python_script
#   DESTINATION ${COLCON_PACKAGE_BIN_DESTINATION}
# )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/colcon/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${COLCON_PACKAGE_BIN_DESTINATION}
# )

## Mark libraries for installation
## See http://docs.ros.org/melodic/api/colcon/html/howto/format1/building_libraries.html
# install(TARGETS ${PROJECT_NAME}
#   ARCHIVE DESTINATION ${COLCON_PACKAGE_LIB_DESTINATION}
  # LIBRARY DESTINATION ${COLCON_PACKAGE_LIB_DESTINATION}
#   RUNTIME DESTINATION ${COLCON_GLOBAL_BIN_DESTINATION}
# )

## Mark cpp header files for installation
# install(DIRECTORY include/${PROJECT_NAME}/
#   DESTINATION ${COLCON_PACKAGE_INCLUDE_DESTINATION}
#   FILES_MATCHING PATTERN "*.h"
#   PATTERN ".svn" EXCLUDE
# )

## Mark other files for installation (e.g. launch and bag files, etc.)
# install(FILES
#   # myfile1
#   # myfile2
#   DESTINATION ${COLCON_PACKAGE_SHARE_DESTINATION}
# )

#############
## Testing ##
#############

## Add gtest based cpp test target and link libraries
# colcon_add_gtest(${PROJECT_NAME}-test test/test_floorplan_gazebo_plugins.cpp)
# if(TARGET ${PROJECT_NAME}-test)
#   target_link_libraries(${PROJECT_NAME}-test ${PROJECT_NAME})
# endif()

## Add folders to be run by python nosetests
# colcon_add_nosetests(test)
