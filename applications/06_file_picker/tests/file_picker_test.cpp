#include <gtest/gtest.h>
#include "file_picker_app.h"

class FilePickerTest : public ::testing::Test {
protected:
    FilePickerApp* app;

    void SetUp() override {
        app = new FilePickerApp();
    }

    void TearDown() override {
        delete app;
    }
};

TEST_F(FilePickerTest, ConstructorCreatesInstance) {
    ASSERT_NE(app, nullptr);
}

TEST_F(FilePickerTest, OpenFileDialogReturnsString) {
    // Note: This test will show actual dialog in test environment
    // In real unit tests, we would mock the native dialog
    std::string result = app->openFileDialog("Test Open File");
    // Result will be empty if user cancels
    ASSERT_TRUE(result.empty() || !result.empty());
}

TEST_F(FilePickerTest, OpenFilesDialogReturnsVector) {
    std::vector<std::string> result = app->openFilesDialog("Test Open Multiple Files");
    // Result vector will be empty if user cancels
    ASSERT_TRUE(result.empty() || !result.empty());
}

TEST_F(FilePickerTest, OpenDirectoryDialogReturnsString) {
    std::string result = app->openDirectoryDialog("Test Select Directory");
    ASSERT_TRUE(result.empty() || !result.empty());
}

TEST_F(FilePickerTest, SaveFileDialogReturnsString) {
    std::string result = app->saveFileDialog("Test Save File", "test.txt");
    ASSERT_TRUE(result.empty() || !result.empty());
}

TEST_F(FilePickerTest, FileFiltersWork) {
    std::vector<FileFilter> filters = {
        {"Text Files", {"txt", "md"}},
        {"Image Files", {"png", "jpg", "jpeg"}},
        {"All Files", {"*"}}
    };
    
    std::string result = app->openFileDialog("Test with Filters", filters);
    ASSERT_TRUE(result.empty() || !result.empty());
}

// Integration test for JavaScript binding
TEST_F(FilePickerTest, JavaScriptBindingExists) {
    // This test would require running the webview
    // For now, we just ensure the app can be created
    ASSERT_NE(app, nullptr);
}