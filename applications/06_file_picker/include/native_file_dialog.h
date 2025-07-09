#ifndef NATIVE_FILE_DIALOG_H
#define NATIVE_FILE_DIALOG_H

#include "file_dialog_interface.h"

// 実際のネイティブダイアログを表示する実装
class NativeFileDialog : public IFileDialog {
public:
    NativeFileDialog() = default;
    ~NativeFileDialog() override = default;
    
    std::string openFile(const std::string& title,
                        const std::vector<FileFilter>& filters) override;
    
    std::vector<std::string> openFiles(const std::string& title,
                                      const std::vector<FileFilter>& filters) override;
    
    std::string openDirectory(const std::string& title) override;
    
    std::string saveFile(const std::string& title,
                        const std::string& defaultName,
                        const std::vector<FileFilter>& filters) override;
    
private:
    // プラットフォーム別の実装ヘルパー
#ifdef __linux__
    std::string showGtkFileDialog(bool save,
                                 bool directory,
                                 const std::string& title,
                                 const std::string& defaultPath,
                                 const std::vector<FileFilter>& filters);
    
    std::vector<std::string> showGtkMultiFileDialog(const std::string& title,
                                                   const std::vector<FileFilter>& filters);
#endif
};

#endif // NATIVE_FILE_DIALOG_H