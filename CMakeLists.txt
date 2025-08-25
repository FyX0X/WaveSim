cmake_minimum_required(VERSION 3.28)
project(WaveSim)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)


# Include headers
include_directories(include)

add_executable(wavesim
    src/main.cpp
    src/Renderer.cpp
    src/Wave1D.cpp
    src/Wave2D.cpp
    src/Wave1DInteract.cpp
)
target_compile_features(wavesim PRIVATE cxx_std_17)
target_link_libraries(wavesim PRIVATE SFML::Graphics)
