#include <iostream>
#include "file_picker_app.h"
#include "request_parser.h"

int main() {
    std::cout << "Starting test application..." << std::endl;
    
    // デフォルトのファイルダイアログで実行
    FilePickerApp app(nullptr);
    
    // JSONパーサーのテスト
    try {
        std::string title = RequestParser::parseOpenDirectoryRequest("\"フォルダを選択\"");
        std::cout << "Parser test passed, title: " << title << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Parser test failed: " << e.what() << std::endl;
    }
    
    std::cout << "Running app..." << std::endl;
    app.run();
    
    return 0;
}