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
TEST_F(IntegrationTest, OpenDirectory_SingleStringParameter) {
    // JavaScriptから送られる実際の形式: 単一の文字列
    std::string jsRequest = R"("フォルダを選択")";
    
    // これは現在の実装でエラーになるはず
    EXPECT_THROW(RequestParser::parseOpenFileRequest(jsRequest), std::invalid_argument);
}

// openFileの正しい形式
TEST_F(IntegrationTest, OpenFile_CorrectArrayFormat) {
    std::string jsRequest = R"(["ファイルを選択",[{"description":"All Files","extensions":["*"]}]])";
    
    EXPECT_NO_THROW({
        OpenFileRequest request = RequestParser::parseOpenFileRequest(jsRequest);
        EXPECT_EQ(request.title, "ファイルを選択");
    });
}

// 修正案: openDirectoryは単一パラメータのみ
TEST_F(IntegrationTest, ParseOpenDirectoryRequest_SingleString) {
    std::string jsRequest = R"("フォルダを選択")";
    
    // 新しいパーサーメソッドが必要
    // std::string title = RequestParser::parseOpenDirectoryRequest(jsRequest);
    // EXPECT_EQ(title, "フォルダを選択");
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

// handleOpenDirectoryの問題を検出するテスト
TEST_F(FilePickerAppIntegrationTest, HandleOpenDirectory_WrongRequestFormat) {
    // 実際のJavaScriptから送られる形式
    const char* req = R"("フォルダを選択")";
    
    // この呼び出しはエラーになるはず（配列形式を期待しているため）
    // app->handleOpenDirectory("123", req);
    
    // エラーレスポンスが返されることを確認
    // EXPECT_EQ(lastReturnStatus, 1);
    // EXPECT_TRUE(lastReturnResult.find("error") != std::string::npos);
}