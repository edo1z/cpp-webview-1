cmake_minimum_required(VERSION 3.16)
project(cpp_webview_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# プラットフォーム固有の設定
if(WIN32)
    add_definitions(-DWEBVIEW_WINAPI)
elseif(APPLE)
    add_definitions(-DWEBVIEW_COCOA)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework WebKit")
else()
    add_definitions(-DWEBVIEW_GTK)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
    pkg_check_modules(WEBKIT2 REQUIRED webkit2gtk-4.1)
endif()

# webviewライブラリをダウンロード
include(FetchContent)
FetchContent_Declare(
    webview
    URL https://github.com/webview/webview/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(webview)

# インクルードディレクトリ
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${webview_SOURCE_DIR}/core/include
)

if(NOT WIN32 AND NOT APPLE)
    include_directories(
        ${GTK3_INCLUDE_DIRS}
        ${WEBKIT2_INCLUDE_DIRS}
    )
endif()

# ソースファイル
set(SOURCES
    src/webview_app.cpp
    src/main.cpp
)

# 実行ファイル
add_executable(cpp_webview_hello ${SOURCES})

# リンク設定
if(NOT WIN32 AND NOT APPLE)
    target_link_libraries(cpp_webview_hello
        ${GTK3_LIBRARIES}
        ${WEBKIT2_LIBRARIES}
    )
endif()

# テスト設定
enable_testing()

# Google Testをダウンロード
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

# テスト実行ファイル
add_executable(test_runner
    tests/test_webview_app.cpp
    src/webview_app.cpp
)

target_link_libraries(test_runner
    GTest::gtest_main
)

if(NOT WIN32 AND NOT APPLE)
    target_link_libraries(test_runner
        ${GTK3_LIBRARIES}
        ${WEBKIT2_LIBRARIES}
    )
endif()

include(GoogleTest)
gtest_discover_tests(test_runner)
