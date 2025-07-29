cmake_minimum_required(VERSION 3.26)
project(BepFetch)

if(APPLE)
  message(FATAL_ERROR "This application is not supported on Apple")
endif()

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(WIN32)
  include(FetchContent)
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_PROGRESS TURE
  )

  FetchContent_MakeAvailable(glfw)
else()
  find_package(glfw3 REQUIRED)
endif()

find_package(OpenGL REQUIRED)

file(GLOB_RECURSE SRC CONFIGURE_DEPENDS src/*.cpp)

set(IMGUI_SRC
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
  ${IMGUI_DIR}/misc/cpp/imgui_stdlib.cpp
)

add_executable(${PROJECT_NAME} ${SRC} ${IMGUI_SRC})

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)
set_target_properties(${PROJECT_NAME} PROPERTIES
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENTIONS OFF
)

if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE /W4 /permissive-)
  # set(CMAKE_EXE_LINKER_FLAGS "-static")
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Wconversion)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    OpenGL::GL
    opengl32
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    OpenGL::GL
  )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${IMGUI_DIR}/misc/cpp
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
