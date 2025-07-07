cmake_minimum_required(VERSION 3.16)
project(hello_world)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
if(BUILD_TESTS)
    enable_testing()
    
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