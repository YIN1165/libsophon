cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0046 NEW)
cmake_policy(SET CMP0048 NEW)

set(TARGET_NAME "tools")

project(${TARGET_NAME} C CXX)

if(WIN32)
    include(windows.cmake)

else()
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../include)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src)
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/../src/linux)

    if("${PLATFORM}" STREQUAL "soc")
        link_directories("${LIB_DIR}/lib/")
        include_directories("${LIB_DIR}/include")
    elseif("${PLATFORM}" STREQUAL "pcie_arm64")
        link_directories("${LIB_DIR}/lib/")
        include_directories("${LIB_DIR}/include")
   elseif("${PLATFORM}" STREQUAL "pcie_loongarch64")
        link_directories("${LIB_DIR}/lib/")
        include_directories("${LIB_DIR}/include")
        include_directories("${LIB_DIR}/include/ncurses")
    elseif("${PLATFORM}" STREQUAL "pcie_sw64")
        link_directories("${LIB_DIR}/lib/")
        include_directories("${LIB_DIR}/include")
        include_directories("${LIB_DIR}/include/ncurses")
    else()
        find_package(gflags COMPONENTS nothreads_static)
    endif()

    include_directories(${GFLAGS_INCLUDE_DIR})

    file(GLOB TOOLS_SRC_PATH ./*.cpp)

    foreach(src ${TOOLS_SRC_PATH})
        get_filename_component(target ${src} NAME_WE)
        add_executable(${target} ${src})
        target_link_libraries(${target} gflags_nothreads.a bmlib pthread dl)
        install(TARGETS ${target}
            RUNTIME DESTINATION bin
            COMPONENT libsophon)
    endforeach(src)

    add_subdirectory(a53lite)

    if(NOT "${PLATFORM}" STREQUAL "soc")
        add_subdirectory(bmcpu)
    endif()
endif()
