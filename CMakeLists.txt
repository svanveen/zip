cmake_minimum_required(VERSION 3.14)
project(zip)

add_library(zip INTERFACE)

target_include_directories(zip
    INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )

target_compile_features(zip
    INTERFACE
    cxx_std_17
    )

target_compile_definitions(zip
    INTERFACE
    LIBRARY_HEADER_ONLY
    )

install(TARGETS zip
    EXPORT ZipTargets
    INCLUDES DESTINATION include
    )

install(EXPORT ZipTargets
    FILE ZipTargets.cmake
    NAMESPACE Zip::
    DESTINATION lib/cmake/Zip
    )

option(WITH_TESTS "Build library including tests" off)

if (WITH_TESTS)
    add_subdirectory(test)
endif ()