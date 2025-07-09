#include <gtest/gtest.h>
#include <memory>
#include "file_picker_app.h"
#include "mock_file_dialog.h"

class FilePickerAppTest : public ::testing::Test {
protected:
    std::unique_ptr<MockFileDialog> mockDialog;
    std::unique_ptr<FilePickerApp> app;
    
    void SetUp() override {
        mockDialog = std::make_unique<MockFileDialog>();
        // FilePickerAppのコンストラクタでモックを渡す
        app = std::make_unique<FilePickerApp>(mockDialog.get());
    }
    
    void TearDown() override {
        app.reset();
        mockDialog.reset();
    }
};

// ===== openFileDialog のテスト =====

TEST_F(FilePickerAppTest, OpenFileDialog_CallsInterface) {
    mockDialog->setOpenFileResult("/test/file.txt");
    
    std::vector<FileFilter> filters = {
        {"Text Files", {"txt", "md"}},
        {"All Files", {"*"}}
    };
    
    std::string result = app->openFileDialog("Select File", filters);
    
    EXPECT_TRUE(mockDialog->wasOpenFileCalled());
    EXPECT_EQ(mockDialog->getLastOpenFileTitle(), "Select File");
    EXPECT_EQ(result, "/test/file.txt");
}

TEST_F(FilePickerAppTest, OpenFileDialog_EmptyResult) {
    mockDialog->setOpenFileResult("");
    
    std::string result = app->openFileDialog("Select File");
    
    EXPECT_TRUE(mockDialog->wasOpenFileCalled());
    EXPECT_EQ(result, "");
}

// ===== openFilesDialog のテスト =====

TEST_F(FilePickerAppTest, OpenFilesDialog_MultipleFiles) {
    std::vector<std::string> expectedFiles = {
        "/test/file1.txt",
        "/test/file2.txt",
        "/test/file3.txt"
    };
    mockDialog->setOpenFilesResult(expectedFiles);
    
    std::vector<std::string> result = app->openFilesDialog("Select Files");
    
    EXPECT_TRUE(mockDialog->wasOpenFilesCalled());
    ASSERT_EQ(result.size(), 3);
    EXPECT_EQ(result[0], "/test/file1.txt");
    EXPECT_EQ(result[1], "/test/file2.txt");
    EXPECT_EQ(result[2], "/test/file3.txt");
}

TEST_F(FilePickerAppTest, OpenFilesDialog_NoFiles) {
    mockDialog->setOpenFilesResult({});
    
    std::vector<std::string> result = app->openFilesDialog("Select Files");
    
    EXPECT_TRUE(mockDialog->wasOpenFilesCalled());
    EXPECT_EQ(result.size(), 0);
}

// ===== openDirectoryDialog のテスト =====

TEST_F(FilePickerAppTest, OpenDirectoryDialog_CallsInterface) {
    mockDialog->setOpenDirectoryResult("/test/directory");
    
    std::string result = app->openDirectoryDialog("Select Directory");
    
    EXPECT_TRUE(mockDialog->wasOpenDirectoryCalled());
    EXPECT_EQ(result, "/test/directory");
}

// ===== saveFileDialog のテスト =====

TEST_F(FilePickerAppTest, SaveFileDialog_WithDefaultName) {
    mockDialog->setSaveFileResult("/test/saved_file.txt");
    
    std::vector<FileFilter> filters = {
        {"Text Files", {"txt"}}
    };
    
    std::string result = app->saveFileDialog("Save File", "document.txt", filters);
    
    EXPECT_TRUE(mockDialog->wasSaveFileCalled());
    EXPECT_EQ(result, "/test/saved_file.txt");
}

// ===== JavaScript binding のテスト =====

class FilePickerAppBindingTest : public ::testing::Test {
protected:
    std::unique_ptr<MockFileDialog> mockDialog;
    std::unique_ptr<FilePickerApp> app;
    
    void SetUp() override {
        mockDialog = std::make_unique<MockFileDialog>();
        // Webviewモックも必要になるが、ここでは簡略化
    }
};

// 注: 実際のbindingテストにはwebviewのモックが必要
// ここではインターフェースレベルのテストに留める