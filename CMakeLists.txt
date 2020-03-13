cmake_minimum_required(VERSION 3.15)
project(Zip VERSION 0.1)

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

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
    ${CMAKE_BINARY_DIR}/ZipConfigVersion.cmake
    COMPATIBILITY AnyNewerVersion
)

configure_package_config_file(
    ${CMAKE_SOURCE_DIR}/cmake/ZipConfig.cmake.in
    ${CMAKE_BINARY_DIR}/ZipConfig.cmake
    INSTALL_DESTINATION lib/cmake/Zip
)

install(TARGETS zip
    EXPORT ZipTargets
    LIBRARY DESTINATION lib COMPONENT Runtime
    ARCHIVE DESTINATION lib COMPONENT Development
    RUNTIME DESTINATION bin COMPONENT Runtime
    PUBLIC_HEADER DESTINATION include COMPONENT Development
    BUNDLE DESTINATION bin COMPONENT Runtime
)

install(EXPORT ZipTargets
    NAMESPACE Zip::
    DESTINATION lib/cmake/Zip
)

install(FILES
    ${CMAKE_BINARY_DIR}/ZipConfigVersion.cmake
    ${CMAKE_BINARY_DIR}/ZipConfig.cmake
    DESTINATION lib/cmake/Zip
)

install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)

option(WITH_EXAMPLE "Build library including usage example" off)

if (WITH_EXAMPLE)
    add_subdirectory(example)
endif ()