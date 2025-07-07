#include "webview_app.h"
#include <fstream>
#include <sstream>
#include <iostream>

auto loadHtmlFile(const std::string& filepath) -> std::string {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "<h1>Error: Could not load HTML file</h1>";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

auto main() -> int {
    try {
        constexpr int DEFAULT_WIDTH = 800;
        constexpr int DEFAULT_HEIGHT = 600;
        WebviewApp app("Hello World", DEFAULT_WIDTH, DEFAULT_HEIGHT);
        
        // HTMLファイルを読み込む
        std::string html = loadHtmlFile("../web/index.html");
        app.setHtml(html);
        
        // アプリケーションを実行
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return 1;
    }
    
    return 0;
}