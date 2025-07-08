#include "text_echo_app.h"
#include <algorithm>
#include <cctype>
#include <iostream>

TextEchoApp::TextEchoApp() {
    w = webview_create(0, nullptr);
    webview_set_title(w, "Text Echo Application");
    webview_set_size(w, 600, 500, WEBVIEW_HINT_NONE);
}

TextEchoApp::~TextEchoApp() {
    if (w) {
        webview_destroy(w);
    }
}

void TextEchoApp::run() {
    // バインディングを設定
    webview_bind(w, "echo", echoHandler, this);
    webview_bind(w, "toUpperCase", toUpperCaseHandler, this);
    webview_bind(w, "countCharacters", countCharactersHandler, this);
    webview_bind(w, "reverse", reverseHandler, this);
    
    // HTMLファイルを読み込む
    std::string html_path = "file://" + std::string(WEBVIEW_BUILD_DIR) + "/web/index.html";
    webview_navigate(w, html_path.c_str());
    
    webview_run(w);
}

std::string TextEchoApp::echo(const std::string& text) {
    return text;
}

std::string TextEchoApp::toUpperCase(const std::string& text) {
    std::string result = text;
    std::transform(result.begin(), result.end(), result.begin(), 
                   [](unsigned char c) { return std::toupper(c); });
    return result;
}

int TextEchoApp::countCharacters(const std::string& text) {
    // UTF-8文字列の文字数を正しくカウント
    int count = 0;
    for (size_t i = 0; i < text.length(); ) {
        unsigned char c = text[i];
        if (c <= 0x7F) {
            i += 1;
        } else if ((c & 0xE0) == 0xC0) {
            i += 2;
        } else if ((c & 0xF0) == 0xE0) {
            i += 3;
        } else if ((c & 0xF8) == 0xF0) {
            i += 4;
        } else {
            i += 1;
        }
        count++;
    }
    return count;
}

std::string TextEchoApp::reverse(const std::string& text) {
    std::string result = text;
    std::reverse(result.begin(), result.end());
    return result;
}

void TextEchoApp::echoHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<TextEchoApp*>(arg);
    if (app != nullptr) {
        // JSON文字列から実際のテキストを抽出（簡易的な実装）
        std::string request(req);
        std::string text = request.substr(2, request.length() - 4); // ["text"]から"text"を抽出
        
        std::string result = app->echo(text);
        
        // 結果をJSON形式で返す
        std::string json_result = "\"" + result + "\"";
        webview_return(app->w, seq, 0, json_result.c_str());
    }
}

void TextEchoApp::toUpperCaseHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<TextEchoApp*>(arg);
    if (app != nullptr) {
        std::string request(req);
        std::string text = request.substr(2, request.length() - 4);
        
        std::string result = app->toUpperCase(text);
        
        std::string json_result = "\"" + result + "\"";
        webview_return(app->w, seq, 0, json_result.c_str());
    }
}

void TextEchoApp::countCharactersHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<TextEchoApp*>(arg);
    if (app != nullptr) {
        std::string request(req);
        std::string text = request.substr(2, request.length() - 4);
        
        int result = app->countCharacters(text);
        
        std::string json_result = std::to_string(result);
        webview_return(app->w, seq, 0, json_result.c_str());
    }
}

void TextEchoApp::reverseHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<TextEchoApp*>(arg);
    if (app != nullptr) {
        std::string request(req);
        std::string text = request.substr(2, request.length() - 4);
        
        std::string result = app->reverse(text);
        
        std::string json_result = "\"" + result + "\"";
        webview_return(app->w, seq, 0, json_result.c_str());
    }
}