cmake_minimum_required(VERSION 3.10)
project(file_picker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Webview
FetchContent_Declare(
  webview
  GIT_REPOSITORY https://github.com/webview/webview.git
  GIT_TAG master
)
FetchContent_MakeAvailable(webview)

# JsonCpp
find_package(PkgConfig REQUIRED)
pkg_check_modules(JSONCPP REQUIRED jsoncpp)

# Platform specific libraries
if(LINUX)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2GTK REQUIRED webkit2gtk-4.1)
endif()

# Main executable
add_executable(file_picker
    src/main.cpp
    src/file_picker_app.cpp
    src/native_file_dialog.cpp
    src/request_parser.cpp
)

target_include_directories(file_picker PRIVATE
    include
    ${webview_SOURCE_DIR}/core/include
    ${JSONCPP_INCLUDE_DIRS}
)

target_compile_definitions(file_picker PRIVATE
    WEBVIEW_SRC_DIR="${CMAKE_SOURCE_DIR}"
)

if(LINUX)
    target_include_directories(file_picker PRIVATE
        ${GTK3_INCLUDE_DIRS}
        ${WEBKIT2GTK_INCLUDE_DIRS}
    )
    target_link_libraries(file_picker
        ${GTK3_LIBRARIES}
        ${WEBKIT2GTK_LIBRARIES}
        ${JSONCPP_LIBRARIES}
    )
    target_compile_options(file_picker PRIVATE
        ${GTK3_CFLAGS_OTHER}
        ${WEBKIT2GTK_CFLAGS_OTHER}
    )
    target_link_directories(file_picker PRIVATE
        ${GTK3_LIBRARY_DIRS}
        ${WEBKIT2GTK_LIBRARY_DIRS}
    )
endif()

# Test executable
enable_testing()
add_executable(file_picker_test
    tests/file_picker_app_test.cpp
    tests/request_parser_test.cpp
    tests/integration_test.cpp
    src/file_picker_app.cpp
    src/request_parser.cpp
    src/native_file_dialog.cpp
)

target_include_directories(file_picker_test PRIVATE
    include
    ${webview_SOURCE_DIR}/core/include
    ${JSONCPP_INCLUDE_DIRS}
)

target_compile_definitions(file_picker_test PRIVATE
    WEBVIEW_SRC_DIR="${CMAKE_SOURCE_DIR}"
)

target_link_libraries(file_picker_test
    GTest::gtest_main
)

if(LINUX)
    target_include_directories(file_picker_test PRIVATE
        ${GTK3_INCLUDE_DIRS}
        ${WEBKIT2GTK_INCLUDE_DIRS}
    )
    target_link_libraries(file_picker_test
        ${GTK3_LIBRARIES}
        ${WEBKIT2GTK_LIBRARIES}
        ${JSONCPP_LIBRARIES}
    )
endif()

include(GoogleTest)
gtest_discover_tests(file_picker_test)

# Copy web resources
file(COPY ${CMAKE_SOURCE_DIR}/web DESTINATION ${CMAKE_BINARY_DIR})

# Test main executable
add_executable(test_main
    src/test_main.cpp
    src/file_picker_app.cpp
    src/native_file_dialog.cpp
    src/request_parser.cpp
)

target_include_directories(test_main PRIVATE
    include
    ${webview_SOURCE_DIR}/core/include
    ${JSONCPP_INCLUDE_DIRS}
)

target_compile_definitions(test_main PRIVATE
    WEBVIEW_SRC_DIR="${CMAKE_SOURCE_DIR}"
)

if(LINUX)
    target_include_directories(test_main PRIVATE
        ${GTK3_INCLUDE_DIRS}
        ${WEBKIT2GTK_INCLUDE_DIRS}
    )
    target_link_libraries(test_main
        ${GTK3_LIBRARIES}
        ${WEBKIT2GTK_LIBRARIES}
        ${JSONCPP_LIBRARIES}
    )
endif()