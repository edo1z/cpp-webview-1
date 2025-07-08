cmake_minimum_required(VERSION 3.10)
project(counter)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)

FetchContent_Declare(
    webview
    URL https://github.com/webview/webview/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(webview)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.1)

set(WEBVIEW_INCLUDE_DIR ${webview_SOURCE_DIR}/core/include)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${WEBVIEW_INCLUDE_DIR}
    ${GTK3_INCLUDE_DIRS}
    ${WEBKIT2GTK_INCLUDE_DIRS}
)

link_directories(
    ${GTK3_LIBRARY_DIRS}
    ${WEBKIT2GTK_LIBRARY_DIRS}
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(counter
    src/main.cpp
    src/counter_app.cpp
)

target_compile_definitions(counter PRIVATE 
    WEBVIEW_BUILD_DIR="${CMAKE_BINARY_DIR}"
    WEBVIEW_GTK
)

target_link_libraries(counter
    ${GTK3_LIBRARIES}
    ${WEBKIT2GTK_LIBRARIES}
)

enable_testing()

add_executable(counter_test
    tests/counter_test.cpp
)

target_compile_definitions(counter_test PRIVATE 
    WEBVIEW_BUILD_DIR="${CMAKE_BINARY_DIR}"
    WEBVIEW_GTK
)

target_link_libraries(counter_test
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(counter_test)

file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/web DESTINATION ${CMAKE_BINARY_DIR})