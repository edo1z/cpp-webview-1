#include "file_picker_app.h"
#include "native_file_dialog.h"
#include "request_parser.h"
#include <iostream>

FilePickerApp::FilePickerApp(IFileDialog* fileDialog) {
    // デバッグモードでwebviewを作成（第1引数を1にする）
    w = webview_create(1, nullptr);  // 1 = debug mode
    webview_set_title(w, "File Picker Application");
    webview_set_size(w, 800, 600, WEBVIEW_HINT_NONE);
    
    // 依存性注入
    if (fileDialog) {
        fileDialog_ = fileDialog;
    } else {
        // デフォルトで実際のダイアログを使用
        ownedFileDialog_ = std::make_unique<NativeFileDialog>();
        fileDialog_ = ownedFileDialog_.get();
    }
    
    bindJavaScriptFunctions();
}

FilePickerApp::~FilePickerApp() {
    webview_destroy(w);
}

void FilePickerApp::run() {
    // JavaScriptのconsole.logをC++側に転送する設定
    webview_eval(w, R"(
        (function() {
            const originalLog = console.log;
            const originalError = console.error;
            const originalWarn = console.warn;
            
            console.log = function(...args) {
                originalLog.apply(console, args);
                window.cppLog && window.cppLog('LOG: ' + args.join(' '));
            };
            
            console.error = function(...args) {
                originalError.apply(console, args);
                window.cppLog && window.cppLog('ERROR: ' + args.join(' '));
            };
            
            console.warn = function(...args) {
                originalWarn.apply(console, args);
                window.cppLog && window.cppLog('WARN: ' + args.join(' '));
            };
        })();
    )");
    
    // C++側でログを受け取る関数をバインド
    webview_bind(w, "cppLog", [](const char* id, const char* req, void* arg) -> void {
        Json::Reader reader;
        Json::Value root;
        if (reader.parse(req, root) && root.isArray() && root.size() > 0) {
            std::cout << "[JS] " << root[0].asString() << std::endl;
        }
        webview_return(static_cast<webview_t>(arg), id, 0, "");
    }, w);
    
    std::string html_path = std::string("file://") + 
                           std::string(WEBVIEW_SRC_DIR) + 
                           "/web/index.html";
    webview_navigate(w, html_path.c_str());
    webview_run(w);
}

std::string FilePickerApp::openFileDialog(const std::string& title,
                                        const std::vector<FileFilter>& filters) {
    return fileDialog_->openFile(title, filters);
}

std::vector<std::string> FilePickerApp::openFilesDialog(const std::string& title,
                                                       const std::vector<FileFilter>& filters) {
    return fileDialog_->openFiles(title, filters);
}

std::string FilePickerApp::openDirectoryDialog(const std::string& title) {
    return fileDialog_->openDirectory(title);
}

std::string FilePickerApp::saveFileDialog(const std::string& title,
                                        const std::string& defaultName,
                                        const std::vector<FileFilter>& filters) {
    return fileDialog_->saveFile(title, defaultName, filters);
}

void FilePickerApp::bindJavaScriptFunctions() {
    std::cout << "Binding JavaScript functions..." << std::endl;
    
    // openFile binding
    auto result1 = webview_bind(w, "openFile", [](const char* id, const char* req, void* arg) -> void {
        auto* app = static_cast<FilePickerApp*>(arg);
        app->handleOpenFile(id, req);
    }, this);
    std::cout << "openFile binding result: " << result1 << std::endl;
    
    // openFiles binding
    auto result2 = webview_bind(w, "openFiles", [](const char* id, const char* req, void* arg) -> void {
        auto* app = static_cast<FilePickerApp*>(arg);
        app->handleOpenFiles(id, req);
    }, this);
    std::cout << "openFiles binding result: " << result2 << std::endl;
    
    // openDirectory binding
    auto result3 = webview_bind(w, "openDirectory", [](const char* id, const char* req, void* arg) -> void {
        std::cout << "openDirectory lambda called!" << std::endl;
        auto* app = static_cast<FilePickerApp*>(arg);
        app->handleOpenDirectory(id, req);
    }, this);
    std::cout << "openDirectory binding result: " << result3 << std::endl;
    
    // saveFile binding
    auto result4 = webview_bind(w, "saveFile", [](const char* id, const char* req, void* arg) -> void {
        auto* app = static_cast<FilePickerApp*>(arg);
        app->handleSaveFile(id, req);
    }, this);
    std::cout << "saveFile binding result: " << result4 << std::endl;
}

void FilePickerApp::handleOpenFile(const char* id, const char* req) {
    try {
        OpenFileRequest request = RequestParser::parseOpenFileRequest(req);
        std::string result = openFileDialog(request.title, request.filters);
        std::string response = RequestParser::createResponse(result);
        webview_return(w, id, 0, response.c_str());
    } catch (const std::exception& e) {
        std::cerr << "Error in handleOpenFile: " << e.what() << std::endl;
        std::string errorResponse = RequestParser::createErrorResponse(e.what());
        webview_return(w, id, 1, errorResponse.c_str());
    }
}

void FilePickerApp::handleOpenFiles(const char* id, const char* req) {
    try {
        OpenFileRequest request = RequestParser::parseOpenFileRequest(req);
        std::vector<std::string> results = openFilesDialog(request.title, request.filters);
        std::string response = RequestParser::createResponse(results);
        webview_return(w, id, 0, response.c_str());
    } catch (const std::exception& e) {
        std::cerr << "Error in handleOpenFiles: " << e.what() << std::endl;
        std::string errorResponse = RequestParser::createErrorResponse(e.what());
        webview_return(w, id, 1, errorResponse.c_str());
    }
}

void FilePickerApp::handleOpenDirectory(const char* id, const char* req) {
    std::cout << "handleOpenDirectory called with id: " << id << ", req: " << req << std::endl;
    try {
        std::string title = RequestParser::parseOpenDirectoryRequest(req);
        std::cout << "Parsed title: " << title << std::endl;
        std::string result = openDirectoryDialog(title);
        std::cout << "Dialog result: " << result << std::endl;
        std::string response = RequestParser::createResponse(result);
        std::cout << "Response JSON: " << response << std::endl;
        webview_return(w, id, 0, response.c_str());
    } catch (const std::exception& e) {
        std::cerr << "Error in handleOpenDirectory: " << e.what() << std::endl;
        std::cerr << "Request was: " << req << std::endl;
        std::string errorResponse = RequestParser::createErrorResponse(e.what());
        webview_return(w, id, 1, errorResponse.c_str());
    }
}

void FilePickerApp::handleSaveFile(const char* id, const char* req) {
    try {
        SaveFileRequest request = RequestParser::parseSaveFileRequest(req);
        std::string result = saveFileDialog(request.title, request.defaultName, request.filters);
        std::string response = RequestParser::createResponse(result);
        webview_return(w, id, 0, response.c_str());
    } catch (const std::exception& e) {
        std::cerr << "Error in handleSaveFile: " << e.what() << std::endl;
        std::string errorResponse = RequestParser::createErrorResponse(e.what());
        webview_return(w, id, 1, errorResponse.c_str());
    }
}