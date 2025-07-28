cmake_minimum_required(VERSION 3.26)
project(BepFetch)

if(APPLE)
  message(FATAL_ERROR "This application is not supported on Apple")
endif()

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
  set(CMAKE_EXE_LINKER_FLAGS "-static")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic -Wextra -Wconversion")
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)

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

set(IMGUI_SRC
  ${IMGUI_DIR}/imgui.cpp
  ${IMGUI_DIR}/imgui_demo.cpp
  ${IMGUI_DIR}/imgui_draw.cpp
  ${IMGUI_DIR}/imgui_tables.cpp
  ${IMGUI_DIR}/imgui_widgets.cpp
  ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
  ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

file(GLOB_RECURSE SRC src/*.cpp)

add_executable(${PROJECT_NAME} ${SRC} ${IMGUI_SRC})

if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw
    PRIVATE opengl32
  )
else()
  target_link_libraries(${PROJECT_NAME}
    PRIVATE ${OPENGL_LIBRARIES}
    PRIVATE glfw
  )
endif()

target_include_directories(${PROJECT_NAME}
  PUBLIC ${IMGUI_DIR}
  PUBLIC ${IMGUI_DIR}/backends
)

