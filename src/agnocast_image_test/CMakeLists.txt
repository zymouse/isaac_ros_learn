cmake_minimum_required(VERSION 3.8)
project(agnocast_image_test)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake_auto REQUIRED)
ament_auto_find_build_dependencies()
find_package(OpenCV REQUIRED)  

# 普通的发布订阅 ------------------------------------------
ament_auto_add_executable(rgba_show_node 
  src/rgba_show.cpp
)
target_include_directories(rgba_show_node PRIVATE
  "include"
  ${OpenCV_INCLUDE_DIRS}

)
target_link_libraries(rgba_show_node
  ${OpenCV_LIBRARIES}
)

ament_auto_add_executable(rgba_publisher_node 
  src/rgba_publisher.cpp
)
target_include_directories(rgba_publisher_node PRIVATE
  "include"
  ${OpenCV_INCLUDE_DIRS}

)
target_link_libraries(rgba_publisher_node
  ${OpenCV_LIBRARIES}
)

# zero-copy的发布订阅 ------------------------------------------
ament_auto_add_library(rgba_show_zero_node SHARED
  src/rgba_show_zero.cpp
)
target_include_directories(rgba_show_zero_node PRIVATE
  "include"
  ${OpenCV_INCLUDE_DIRS}

)
target_link_libraries(rgba_show_zero_node
  ${OpenCV_LIBRARIES}
)
rclcpp_components_register_node(
  rgba_show_zero_node
  PLUGIN "ImageViewerNode"
  EXECUTABLE rgba_show_zero_node_exe
)

ament_auto_add_executable(rgba_publisher_zero_node 
  src/rgba_publisher_zero.cpp
)
target_include_directories(rgba_publisher_zero_node PRIVATE
  "include"
  ${OpenCV_INCLUDE_DIRS}

)
target_link_libraries(rgba_publisher_zero_node
  ${OpenCV_LIBRARIES}
)

# install ------------------------------------------------
install(TARGETS
  rgba_publisher_node
  rgba_show_node
  rgba_publisher_zero_node
  rgba_show_zero_node
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION lib
)

ament_auto_package(
  INSTALL_TO_SHARE
  launch
)
