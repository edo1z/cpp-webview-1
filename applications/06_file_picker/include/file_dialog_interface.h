#ifndef FILE_DIALOG_INTERFACE_H
#define FILE_DIALOG_INTERFACE_H

#include <string>
#include <vector>

struct FileFilter {
    std::string description;
    std::vector<std::string> extensions;
};

// ファイルダイアログの抽象インターフェース
class IFileDialog {
public:
    virtual ~IFileDialog() = default;
    
    virtual std::string openFile(const std::string& title,
                               const std::vector<FileFilter>& filters) = 0;
    
    virtual std::vector<std::string> openFiles(const std::string& title,
                                             const std::vector<FileFilter>& filters) = 0;
    
    virtual std::string openDirectory(const std::string& title) = 0;
    
    virtual std::string saveFile(const std::string& title,
                               const std::string& defaultName,
                               const std::vector<FileFilter>& filters) = 0;
};

#endif // FILE_DIALOG_INTERFACE_H