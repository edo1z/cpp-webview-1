#ifndef FILE_PICKER_APP_H
#define FILE_PICKER_APP_H

#include <memory>
#include <string>
#include <vector>
#include "webview/webview.h"
#include "file_dialog_interface.h"

class FilePickerApp {
public:
    // 依存性注入: IFileDialogを外部から注入可能
    explicit FilePickerApp(IFileDialog* fileDialog = nullptr);
    ~FilePickerApp();

    void run();
    
    // File dialog methods - IFileDialogに委譲
    std::string openFileDialog(const std::string& title = "Open File",
                              const std::vector<FileFilter>& filters = {});
    
    std::vector<std::string> openFilesDialog(const std::string& title = "Open Files",
                                           const std::vector<FileFilter>& filters = {});
    
    std::string openDirectoryDialog(const std::string& title = "Select Directory");
    
    std::string saveFileDialog(const std::string& title = "Save File",
                              const std::string& defaultName = "",
                              const std::vector<FileFilter>& filters = {});

private:
    webview_t w;
    IFileDialog* fileDialog_;
    std::unique_ptr<IFileDialog> ownedFileDialog_;  // 所有権を持つ場合
    
    // Bind JavaScript functions
    void bindJavaScriptFunctions();
    
    // JavaScript binding handlers
    void handleOpenFile(const char* id, const char* req);
    void handleOpenFiles(const char* id, const char* req);
    void handleOpenDirectory(const char* id, const char* req);
    void handleSaveFile(const char* id, const char* req);
};

#endif // FILE_PICKER_APP_H