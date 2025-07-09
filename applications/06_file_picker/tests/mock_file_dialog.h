#ifndef MOCK_FILE_DIALOG_H
#define MOCK_FILE_DIALOG_H

#include "file_dialog_interface.h"

// テスト用のモッククラス
class MockFileDialog : public IFileDialog {
public:
    // テスト用の設定
    void setOpenFileResult(const std::string& result) {
        openFileResult_ = result;
    }
    
    void setOpenFilesResult(const std::vector<std::string>& results) {
        openFilesResult_ = results;
    }
    
    void setOpenDirectoryResult(const std::string& result) {
        openDirectoryResult_ = result;
    }
    
    void setSaveFileResult(const std::string& result) {
        saveFileResult_ = result;
    }
    
    // 呼び出し履歴の確認
    bool wasOpenFileCalled() const { return openFileCalled_; }
    bool wasOpenFilesCalled() const { return openFilesCalled_; }
    bool wasOpenDirectoryCalled() const { return openDirectoryCalled_; }
    bool wasSaveFileCalled() const { return saveFileCalled_; }
    
    const std::string& getLastOpenFileTitle() const { return lastOpenFileTitle_; }
    const std::vector<FileFilter>& getLastOpenFileFilters() const { return lastOpenFileFilters_; }
    
    // IFileDialog implementation
    std::string openFile(const std::string& title,
                        const std::vector<FileFilter>& filters) override {
        openFileCalled_ = true;
        lastOpenFileTitle_ = title;
        lastOpenFileFilters_ = filters;
        return openFileResult_;
    }
    
    std::vector<std::string> openFiles(const std::string& title,
                                      const std::vector<FileFilter>& filters) override {
        openFilesCalled_ = true;
        lastOpenFilesTitle_ = title;
        lastOpenFilesFilters_ = filters;
        return openFilesResult_;
    }
    
    std::string openDirectory(const std::string& title) override {
        openDirectoryCalled_ = true;
        lastOpenDirectoryTitle_ = title;
        return openDirectoryResult_;
    }
    
    std::string saveFile(const std::string& title,
                        const std::string& defaultName,
                        const std::vector<FileFilter>& filters) override {
        saveFileCalled_ = true;
        lastSaveFileTitle_ = title;
        lastSaveFileDefaultName_ = defaultName;
        lastSaveFileFilters_ = filters;
        return saveFileResult_;
    }
    
    void reset() {
        openFileCalled_ = false;
        openFilesCalled_ = false;
        openDirectoryCalled_ = false;
        saveFileCalled_ = false;
        lastOpenFileTitle_.clear();
        lastOpenFileFilters_.clear();
        lastOpenFilesTitle_.clear();
        lastOpenFilesFilters_.clear();
        lastOpenDirectoryTitle_.clear();
        lastSaveFileTitle_.clear();
        lastSaveFileDefaultName_.clear();
        lastSaveFileFilters_.clear();
    }
    
private:
    // 戻り値の設定
    std::string openFileResult_;
    std::vector<std::string> openFilesResult_;
    std::string openDirectoryResult_;
    std::string saveFileResult_;
    
    // 呼び出し履歴
    bool openFileCalled_ = false;
    bool openFilesCalled_ = false;
    bool openDirectoryCalled_ = false;
    bool saveFileCalled_ = false;
    
    // 最後の呼び出しパラメータ
    std::string lastOpenFileTitle_;
    std::vector<FileFilter> lastOpenFileFilters_;
    std::string lastOpenFilesTitle_;
    std::vector<FileFilter> lastOpenFilesFilters_;
    std::string lastOpenDirectoryTitle_;
    std::string lastSaveFileTitle_;
    std::string lastSaveFileDefaultName_;
    std::vector<FileFilter> lastSaveFileFilters_;
};

#endif // MOCK_FILE_DIALOG_H