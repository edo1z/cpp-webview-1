#ifndef NOTEPAD_APP_H
#define NOTEPAD_APP_H

#include <string>
#include <webview.h>
#include <filesystem>

class NotepadApp {
public:
    NotepadApp();
    ~NotepadApp();

    void run();
    
    // ファイル操作関数
    bool saveFile(const std::string& filepath, const std::string& content);
    std::string loadFile(const std::string& filepath);
    bool fileExists(const std::string& filepath) const;
    std::string getFileInfo(const std::string& filepath) const;
    
    // ファイル状態管理
    void setCurrentFile(const std::string& filepath);
    std::string getCurrentFile() const;
    bool hasUnsavedChanges() const;
    void markAsModified(bool modified);

private:
    webview_t w;
    std::string currentFilePath;
    bool isModified;
    
    // ハンドラー関数
    static void newFileHandler(const char* seq, const char* req, void* arg);
    static void openFileHandler(const char* seq, const char* req, void* arg);
    static void saveFileHandler(const char* seq, const char* req, void* arg);
    static void saveAsHandler(const char* seq, const char* req, void* arg);
    static void getFileInfoHandler(const char* seq, const char* req, void* arg);
};

#endif // NOTEPAD_APP_H