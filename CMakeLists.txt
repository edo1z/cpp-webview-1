cmake_minimum_required(VERSION 3.16)
project(cpp_webview_1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Export compile commands for clangd
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# オプション
option(BUILD_TESTS "Build tests" ON)

# プラットフォーム固有の設定
if(WIN32)
    add_definitions(-DWEBVIEW_WINAPI)
elseif(APPLE)
    add_definitions(-DWEBVIEW_COCOA)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -framework WebKit")
else()
    add_definitions(-DWEBVIEW_GTK)
    find_package(PkgConfig REQUIRED)
    # 複数のWebKitバージョンに対応
    foreach(WEBKIT_VERSION 4.1 4.0 3.0)
        pkg_check_modules(WEBKIT2 webkit2gtk-${WEBKIT_VERSION})
        if(WEBKIT2_FOUND)
            break()
        endif()
    endforeach()
    if(NOT WEBKIT2_FOUND)
        message(FATAL_ERROR "WebKit2GTK not found")
    endif()
    pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
endif()

# webviewライブラリをダウンロード
include(FetchContent)
FetchContent_Declare(
    webview
    URL https://github.com/webview/webview/archive/refs/heads/master.zip
)
FetchContent_MakeAvailable(webview)

# webviewのインクルードディレクトリを設定
set(WEBVIEW_INCLUDE_DIR ${webview_SOURCE_DIR}/core/include)

# Google Testをダウンロード（テストが有効な場合）
if(BUILD_TESTS)
    FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    )
    FetchContent_MakeAvailable(googletest)
endif()

# アプリケーションディレクトリを追加
add_subdirectory(applications/01_hello_world)