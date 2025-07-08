#include "notepad_app.h"
#include <fstream>
#include <sstream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;

NotepadApp::NotepadApp() : isModified(false), w(webview_create(0, nullptr)) {
    webview_set_title(w, "Notepad Application");
    webview_set_size(w, 800, 600, WEBVIEW_HINT_NONE);
}

NotepadApp::~NotepadApp() {
    if (w) {
        webview_destroy(w);
    }
}

void NotepadApp::run() {
    // バインディングを設定
    webview_bind(w, "newFile", newFileHandler, this);
    webview_bind(w, "openFile", openFileHandler, this);
    webview_bind(w, "saveFile", saveFileHandler, this);
    webview_bind(w, "saveAs", saveAsHandler, this);
    webview_bind(w, "getFileInfo", getFileInfoHandler, this);
    
    // HTMLファイルを読み込む
    std::string html_path = "file://" + std::string(WEBVIEW_BUILD_DIR) + "/web/index.html";
    webview_navigate(w, html_path.c_str());
    
    webview_run(w);
}

bool NotepadApp::saveFile(const std::string& filepath, const std::string& content) {
    try {
        // ディレクトリが存在しない場合は作成
        fs::path path(filepath);
        if (path.has_parent_path()) {
            fs::create_directories(path.parent_path());
        }
        
        // ファイルに書き込み
        std::ofstream file(filepath, std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        file.close();
        
        return true;
    } catch (const std::exception& e) {
        return false;
    }
}

std::string NotepadApp::loadFile(const std::string& filepath) {
    try {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return "";
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        file.close();
        
        return buffer.str();
    } catch (const std::exception& e) {
        return "";
    }
}

bool NotepadApp::fileExists(const std::string& filepath) const {
    return fs::exists(filepath);
}

std::string NotepadApp::getFileInfo(const std::string& filepath) const {
    try {
        std::stringstream json;
        json << "{";
        
        if (fs::exists(filepath)) {
            auto size = fs::file_size(filepath);
            auto time = fs::last_write_time(filepath);
            
            json << "\"exists\":true,";
            json << "\"size\":" << size << ",";
            json << "\"path\":\"" << filepath << "\"";
        } else {
            json << "\"exists\":false";
        }
        
        json << "}";
        return json.str();
    } catch (const std::exception& e) {
        return "{\"exists\":false,\"error\":\"" + std::string(e.what()) + "\"}";
    }
}

void NotepadApp::setCurrentFile(const std::string& filepath) {
    currentFilePath = filepath;
}

std::string NotepadApp::getCurrentFile() const {
    return currentFilePath;
}

bool NotepadApp::hasUnsavedChanges() const {
    return isModified;
}

void NotepadApp::markAsModified(bool modified) {
    isModified = modified;
}

void NotepadApp::newFileHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<NotepadApp*>(arg);
    if (app != nullptr) {
        app->setCurrentFile("");
        app->markAsModified(false);
        webview_return(app->w, seq, 0, "\"ok\"");
    }
}

void NotepadApp::openFileHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<NotepadApp*>(arg);
    if (app != nullptr) {
        // JSONから引数を解析（簡易的な実装）
        std::string request(req);
        // ["filepath"] から "filepath" を抽出
        std::string filepath = request.substr(2, request.length() - 4);
        
        std::string content = app->loadFile(filepath);
        if (!content.empty() || app->fileExists(filepath)) {
            app->setCurrentFile(filepath);
            app->markAsModified(false);
            
            // JSONエスケープ（簡易版）
            std::string escaped = content;
            size_t pos = 0;
            while ((pos = escaped.find("\"", pos)) != std::string::npos) {
                escaped.replace(pos, 1, "\\\"");
                pos += 2;
            }
            pos = 0;
            while ((pos = escaped.find("\n", pos)) != std::string::npos) {
                escaped.replace(pos, 1, "\\n");
                pos += 2;
            }
            
            std::string json_result = "{\"content\":\"" + escaped + "\",\"path\":\"" + filepath + "\"}";
            webview_return(app->w, seq, 0, json_result.c_str());
        } else {
            webview_return(app->w, seq, 1, "{\"error\":\"File not found\"}");
        }
    }
}

void NotepadApp::saveFileHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<NotepadApp*>(arg);
    if (app != nullptr) {
        // JSONから引数を解析（簡易的な実装）
        std::string request(req);
        // [filepath, content] を解析
        size_t comma = request.find("\",\"");
        if (comma != std::string::npos) {
            std::string filepath = request.substr(2, comma - 2);
            std::string content = request.substr(comma + 3, request.length() - comma - 5);
            
            // エスケープ解除
            size_t pos = 0;
            while ((pos = content.find("\\n", pos)) != std::string::npos) {
                content.replace(pos, 2, "\n");
                pos += 1;
            }
            pos = 0;
            while ((pos = content.find("\\\"", pos)) != std::string::npos) {
                content.replace(pos, 2, "\"");
                pos += 1;
            }
            
            if (app->saveFile(filepath, content)) {
                app->setCurrentFile(filepath);
                app->markAsModified(false);
                webview_return(app->w, seq, 0, "\"ok\"");
            } else {
                webview_return(app->w, seq, 1, "{\"error\":\"Failed to save file\"}");
            }
        } else {
            webview_return(app->w, seq, 1, "{\"error\":\"Invalid parameters\"}");
        }
    }
}

void NotepadApp::saveAsHandler(const char* seq, const char* req, void* arg) {
    // saveFileHandlerと同じ実装
    saveFileHandler(seq, req, arg);
}

void NotepadApp::getFileInfoHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<NotepadApp*>(arg);
    if (app != nullptr) {
        std::string request(req);
        std::string filepath = request.substr(2, request.length() - 4);
        
        std::string info = app->getFileInfo(filepath);
        webview_return(app->w, seq, 0, info.c_str());
    }
}