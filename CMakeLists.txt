cmake_minimum_required(VERSION 2.8)
project(ImGUIWrapper CXX)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "" FORCE)
endif()

#Output config
set(OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#Makes VS Debugger crazy
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

#exe path
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIRECTORY}")

#dll path
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/lib")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/lib")
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIRECTORY}/lib")

#lib path
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${OUTPUT_DIRECTORY}archive")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}/archive")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}/archive")

set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DVK_PROTOTYPES")
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DVK_PROTOTYPES")

# GLFW
set(GLFW_DIR deps/glfw) # Set this to point to an up-to-date GLFW repo
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)
add_subdirectory(${GLFW_DIR} binary_dir EXCLUDE_FROM_ALL)
include_directories(${GLFW_DIR}/include)

# ImGui
set(IMGUI_DIR deps/imgui) # Set this to point to an up-to-date ImGui repo
include_directories(${IMGUI_DIR} ..)
file(GLOB IMGUI_SOURCES ${IMGUI_DIR}/*.cpp)

# Libraries
find_library(VULKAN_LIBRARY
  NAMES vulkan vulkan-1)
set(LIBRARIES "glfw;${VULKAN_LIBRARY}")

# Use vulkan headers from glfw:
include_directories(${GLFW_DIR}/deps)

#imgui glfw-vulkan implementation library
add_library(imgui_glfw_vulkan STATIC ${IMGUI_SOURCES})
target_link_libraries(imgui_glfw_vulkan ${LIBRARIES})

#imgui wrapper library
add_definitions(-DIMGUI_WRAPPER_DLL)
include_directories(${PROJECT_SOURCE_DIR}/include)
file(GLOB IMGUI_WRAPPER_SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_library(imgui_wrapper SHARED ${IMGUI_WRAPPER_SOURCES})
target_link_libraries(imgui_wrapper imgui_glfw_vulkan)
set_target_properties(imgui_wrapper PROPERTIES COMPILE_FLAGS "${COMPILE_FLAGS} -DIMGUI_WRAPPER_DLL_EXPORTS")

#executable for testing/example
file(GLOB sources main.cpp)

add_executable(TestExe ${sources})
target_link_libraries(TestExe imgui_wrapper)
