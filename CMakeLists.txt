cmake_minimum_required(VERSION 3.26)
project(BepInFetch)

if(APPLE)
  message(FATAL_ERROR "This application is not supported on Apple")
endif()

set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/extern/imgui)

set(OPENSSL_ROOT_DIR "C:/Program Files/OpenSSL-Win64")
set(OPENSSL_LIB_DIR "${OPENSSL_ROOT_DIR}/lib/VC/x64/MD")
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
# set(OPENSSL_LIBRARIES "${OPENSSL_LIB_DIR}/libcrypto.lib; ${OPENSSL_LIB_DIR}/libssl.lib") # Try to help find_package

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

if(WIN32)
  FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_PROGRESS TURE
  )

  FetchContent_MakeAvailable(glfw)
else()
  find_package(glfw3 REQUIRED)
endif()

FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

FetchContent_Declare(
  cpp_httplib
  GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
  GIT_TAG v0.24.0
)
FetchContent_MakeAvailable(cpp_httplib)

# find_package(OpenSSL REQUIRED) # Tried using it, cannot get it to work
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
else()
  target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wpedantic -Wextra -Wconversion)
endif()

if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    OpenGL::GL
    opengl32
    ${OPENSSL_LIB_DIR}/libssl.lib
    ${OPENSSL_LIB_DIR}/libcrypto.lib
  )
else()
  target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    OpenGL::GL
    OpenSSL::SSL
    OpenSSL::Crypto
  )
endif()

target_include_directories(${PROJECT_NAME} PRIVATE
  ${IMGUI_DIR}
  ${IMGUI_DIR}/backends
  ${IMGUI_DIR}/misc/cpp

  ${OPENSSL_INCLUDE_DIR}
  ${nlohmann_json_SOURCE_DIR}/include
  ${cpp_httplib_SOURCE_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)
