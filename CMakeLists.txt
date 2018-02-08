cmake_minimum_required (VERSION 2.6)
project (audio-effect)

set(CMAKE_CXX_FLAGS   "-std=c++11 -Wall -O2")

add_subdirectory (Src/Common)
add_subdirectory (Src/Effect)
add_subdirectory (Src/Input)
add_subdirectory (Src/Output)

# add the executable
add_executable(audio-effect Src/AudioEffect.cpp)
target_link_libraries (audio-effect Input)
target_link_libraries (audio-effect Output)
target_link_libraries (audio-effect Effect)
target_link_libraries (audio-effect Common)
target_link_libraries (audio-effect asound)
target_link_libraries (audio-effect pthread)
target_link_libraries (audio-effect sox)
