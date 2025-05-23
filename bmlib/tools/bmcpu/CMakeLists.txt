cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
set(TARGET_NAME "bmcpu")

project(${TARGET_NAME} C CXX)

if(WIN32)
    include(windows.cmake)

else()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../bmlib/include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../bmlib/src)

    add_subdirectory(prebuild)

    file(GLOB TEST_SRC_PATH ./src/*.cpp)

    foreach(src ${TEST_SRC_PATH})
        get_filename_component(target ${src} NAME_WE)
        add_executable(${target} ${src})
        target_link_libraries(${target} bmlib pthread)
        install(TARGETS ${target}
            RUNTIME DESTINATION bin
            COMPONENT libsophon)
    endforeach(src)
endif()
