cmake_minimum_required(VERSION 2.8.8)
project(balls CXX C)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(GLEW REQUIRED)
#find_package(PkgConfig REQUIRED)
find_package(GLFW REQUIRED)
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost 1.54.0 COMPONENTS program_options
                                     serialization
                                     filesystem
                                     system)

add_subdirectory(src)
add_definitions( -DBOOST_ALL_NO_LIB )


link_directories(
	"$ENV{PROGRAMFILES}/Boost/lib"
)

add_executable(balls
  ${SURFACE_SRC}
  )

target_link_libraries(balls
  ${Boost_LIBRARIES}
  ${OPENGL_LIBRARIES}
  ${GLEW_LIBRARIES}
  ${GLFW_LIBRARIES}
  ${OpenCV_LIBRARIES}
  )

include_directories(
  "include"
  "$ENV{PROGRAMFILES}/glm/include"
  ${GLEW_INCLUDE_DIRS}
  ${OPENGL_INCLUDE_DIR}
  ${GLFW_INCLUDE_DIR}
  ${Boost_INCLUDE_DIRS}
  ${OpenCV_INCLUDE_DIRS}
  )

# So we cna have debug and release builds
add_custom_target(debug
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Debug ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Debug"
  )

add_custom_target(release
  COMMAND ${CMAKE_COMMAND} -DCMAKE_BUILD_TYPE=Release ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target all
  COMMENT "Switch CMAKE_BUILD_TYPE to Release"
  )

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")
