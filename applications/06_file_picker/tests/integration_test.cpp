#include <gtest/gtest.h>
#include "request_parser.h"
#include "file_picker_app.h"
#include "mock_file_dialog.h"

// JavaScript側から送られてくる実際のリクエストをテスト
class IntegrationTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 各テストの前に実行
    }
};

// フォルダ選択で送られる実際のリクエスト形式をテスト
TEST_F(IntegrationTest, OpenDirectory_ArrayParameter) {
    // webviewは自動的に引数を配列でラップする
    std::string jsRequest = R"(["フォルダを選択"])";
    
    std::string title = RequestParser::parseOpenDirectoryRequest(jsRequest);
    EXPECT_EQ(title, "フォルダを選択");
}

// openFileの正しい形式
TEST_F(IntegrationTest, OpenFile_CorrectArrayFormat) {
    std::string jsRequest = R"(["ファイルを選択",[{"description":"All Files","extensions":["*"]}]])";
    
    EXPECT_NO_THROW({
        OpenFileRequest request = RequestParser::parseOpenFileRequest(jsRequest);
        EXPECT_EQ(request.title, "ファイルを選択");
    });
}

// openDirectoryの単一文字列形式も受け入れる（後方互換性）
TEST_F(IntegrationTest, ParseOpenDirectoryRequest_SingleString) {
    std::string jsRequest = R"("フォルダを選択")";
    
    // 単一文字列形式も受け入れる
    std::string title = RequestParser::parseOpenDirectoryRequest(jsRequest);
    EXPECT_EQ(title, "フォルダを選択");
}

// JavaScriptバインディングのシミュレーションテスト
class FilePickerAppIntegrationTest : public ::testing::Test {
protected:
    std::unique_ptr<MockFileDialog> mockDialog;
    std::unique_ptr<FilePickerApp> app;
    
    void SetUp() override {
        mockDialog = std::make_unique<MockFileDialog>();
        app = std::make_unique<FilePickerApp>(mockDialog.get());
    }
    
    // webview_returnのモック
    void mockWebviewReturn(const char* id, int status, const char* result) {
        // 実際の実装では、ここでJavaScriptに結果を返す
        lastReturnStatus = status;
        lastReturnResult = result ? result : "";
    }
    
    int lastReturnStatus = -1;
    std::string lastReturnResult;
};

// handleOpenDirectoryの正しいリクエスト形式
TEST_F(FilePickerAppIntegrationTest, HandleOpenDirectory_ArrayFormat) {
    // webviewから送られる実際の形式（配列）
    const char* req = R"(["フォルダを選択"])";
    
    mockDialog->setOpenDirectoryResult("/home/user/documents");
    
    // 実際のアプリケーションではwebview_returnが呼ばれるが、
    // ここではdirectにメソッドを呼ぶことでテスト
    std::string result = app->openDirectoryDialog("フォルダを選択");
    EXPECT_EQ(result, "/home/user/documents");
}