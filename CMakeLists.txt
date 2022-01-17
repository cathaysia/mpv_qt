cmake_minimum_required(VERSION 3.16)

project(MusicPlayer)
set(CMAKE_CXX_STANDARD 17)
include(FeatureSummary)

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
