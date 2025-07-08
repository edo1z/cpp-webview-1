cmake_minimum_required(VERSION 3.16)
project(hello_world)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 親プロジェクトからビルドされているかチェック
if(NOT DEFINED WEBVIEW_INCLUDE_DIR)
    # 独立してビルドする場合は、webviewをダウンロード
    include(FetchContent)
    FetchContent_Declare(
        webview
        URL https://github.com/webview/webview/archive/refs/heads/master.zip
    )
    FetchContent_MakeAvailable(webview)
    set(WEBVIEW_INCLUDE_DIR ${webview_SOURCE_DIR}/core/include)
    
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
endif()

# ソースファイルを設定
set(SOURCES
    src/main.cpp
    src/webview_app.cpp
)

# 実行ファイル
add_executable(hello_world ${SOURCES})

# インクルードディレクトリ
target_include_directories(hello_world PRIVATE 
    include
    ${WEBVIEW_INCLUDE_DIR}
)

# プラットフォーム固有の設定
if(WIN32)
    target_compile_definitions(hello_world PRIVATE WEBVIEW_WINAPI)
elseif(APPLE)
    target_compile_definitions(hello_world PRIVATE WEBVIEW_COCOA)
    target_link_libraries(hello_world PRIVATE "-framework WebKit")
else()
    target_compile_definitions(hello_world PRIVATE WEBVIEW_GTK)
    target_link_libraries(hello_world PRIVATE ${GTK3_LIBRARIES} ${WEBKIT2_LIBRARIES})
    target_include_directories(hello_world PRIVATE ${GTK3_INCLUDE_DIRS} ${WEBKIT2_INCLUDE_DIRS})
endif()

# テストを有効化
option(BUILD_TESTS "Build tests" ON)
if(BUILD_TESTS)
    enable_testing()
    
    # 独立ビルドの場合はGoogle Testもダウンロード
    if(NOT TARGET GTest::gtest_main)
        FetchContent_Declare(
            googletest
            URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
        )
        FetchContent_MakeAvailable(googletest)
    endif()
    
    # テスト実行ファイル
    add_executable(hello_world_test
        tests/test_webview_app.cpp
        src/webview_app.cpp
    )
    
    target_include_directories(hello_world_test PRIVATE 
        include
        ${WEBVIEW_INCLUDE_DIR}
    )
    
    # プラットフォーム固有の設定（テスト用）
    if(WIN32)
        target_compile_definitions(hello_world_test PRIVATE WEBVIEW_WINAPI)
    elseif(APPLE)
        target_compile_definitions(hello_world_test PRIVATE WEBVIEW_COCOA)
        target_link_libraries(hello_world_test PRIVATE "-framework WebKit")
    else()
        target_compile_definitions(hello_world_test PRIVATE WEBVIEW_GTK)
        target_link_libraries(hello_world_test PRIVATE ${GTK3_LIBRARIES} ${WEBKIT2_LIBRARIES})
        target_include_directories(hello_world_test PRIVATE ${GTK3_INCLUDE_DIRS} ${WEBKIT2_INCLUDE_DIRS})
    endif()
    
    target_link_libraries(hello_world_test PRIVATE GTest::gtest_main)
    
    # Google Testの自動検出
    include(GoogleTest)
    gtest_discover_tests(hello_world_test)
endif()