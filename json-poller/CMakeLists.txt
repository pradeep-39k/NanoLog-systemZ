cmake_minimum_required(VERSION 3.5)

project(json-poller-test VERSION 0.1 LANGUAGES CXX)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


file(GLOB_RECURSE SOURCES
    "source/*.cpp")

add_executable(json-poller-test ${SOURCES})


find_package(Boost 1.72 REQUIRED COMPONENTS 
    date_time filesystem system thread)

target_include_directories(json-poller-test 
    PRIVATE
    ../../../
    /usr/local/include
    ${Boost_INCLUDE_DIRS})

target_link_libraries(json-poller-test ${Boost_LIBRARIES}
    system-z jsoncpp numa )

target_compile_options(json-poller-test PRIVATE "-std=c++14")

set_target_properties(json-poller-test PROPERTIES LINK_FLAGS 
    "-Wl,-export-dynamic -Wl,-rpath,. -Wl,-rpath,/usr/local/lib/x86_64-linux-gnu  -Wl,-rpath,/usr/lib/x86_64-linux-gnu")


