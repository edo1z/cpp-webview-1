#include "webview_app.h"
#include <fstream>
#include <sstream>
#include <iostream>

std::string loadHtmlFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        return "<h1>Error: Could not load HTML file</h1>";
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main() {
    try {
        WebviewApp app("Hello World", 800, 600);
        
        // HTMLファイルを読み込む
        std::string html = loadHtmlFile("../web/index.html");
        app.setHtml(html);
        
        // アプリケーションを実行
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}