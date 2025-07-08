#include <gtest/gtest.h>
#include "notepad_app.h"
#include <fstream>
#include <filesystem>
#include <chrono>
#include <thread>

namespace fs = std::filesystem;

class NotepadTest : public ::testing::Test {
protected:
    NotepadApp app;
    std::string testDir = "./test_files";
    std::string testFile = testDir + "/test.txt";
    
    void SetUp() override {
        // テスト用ディレクトリを作成
        fs::create_directories(testDir);
    }
    
    void TearDown() override {
        // テスト用ディレクトリを削除
        fs::remove_all(testDir);
    }
};

// ファイル保存のテスト
TEST_F(NotepadTest, SaveFileCreatesNewFile) {
    std::string content = "Hello, World!";
    EXPECT_TRUE(app.saveFile(testFile, content));
    EXPECT_TRUE(fs::exists(testFile));
}

TEST_F(NotepadTest, SaveFileWritesCorrectContent) {
    std::string content = "This is a test.\nLine 2\nLine 3";
    app.saveFile(testFile, content);
    
    // ファイルの内容を確認
    std::ifstream file(testFile);
    std::string fileContent((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
    EXPECT_EQ(content, fileContent);
}

TEST_F(NotepadTest, SaveFileHandlesUTF8) {
    std::string content = "こんにちは世界！\n🌍 UTF-8 Test";
    EXPECT_TRUE(app.saveFile(testFile, content));
    
    std::string loaded = app.loadFile(testFile);
    EXPECT_EQ(content, loaded);
}

// ファイル読み込みのテスト
TEST_F(NotepadTest, LoadFileReturnsContent) {
    // 先にファイルを作成
    std::string content = "Test content\nMultiple lines";
    std::ofstream file(testFile);
    file << content;
    file.close();
    
    std::string loaded = app.loadFile(testFile);
    EXPECT_EQ(content, loaded);
}

TEST_F(NotepadTest, LoadFileHandlesNonExistentFile) {
    std::string loaded = app.loadFile("non_existent.txt");
    EXPECT_TRUE(loaded.empty());
}

// ファイル存在チェックのテスト
TEST_F(NotepadTest, FileExistsReturnsTrueForExistingFile) {
    std::ofstream file(testFile);
    file << "test";
    file.close();
    
    EXPECT_TRUE(app.fileExists(testFile));
}

TEST_F(NotepadTest, FileExistsReturnsFalseForNonExistentFile) {
    EXPECT_FALSE(app.fileExists("non_existent.txt"));
}

// ファイル情報取得のテスト
TEST_F(NotepadTest, GetFileInfoReturnsCorrectInfo) {
    std::string content = "Test file";
    std::ofstream file(testFile);
    file << content;
    file.close();
    
    std::string info = app.getFileInfo(testFile);
    EXPECT_FALSE(info.empty());
    // JSONフォーマットでサイズが含まれているか確認
    EXPECT_NE(info.find("\"size\":"), std::string::npos);
    EXPECT_NE(info.find("\"exists\":true"), std::string::npos);
}

// ファイル状態管理のテスト
TEST_F(NotepadTest, CurrentFilePathManagement) {
    EXPECT_TRUE(app.getCurrentFile().empty());
    
    app.setCurrentFile(testFile);
    EXPECT_EQ(testFile, app.getCurrentFile());
}

TEST_F(NotepadTest, ModificationStateManagement) {
    EXPECT_FALSE(app.hasUnsavedChanges());
    
    app.markAsModified(true);
    EXPECT_TRUE(app.hasUnsavedChanges());
    
    app.markAsModified(false);
    EXPECT_FALSE(app.hasUnsavedChanges());
}

// エラーハンドリングのテスト
TEST_F(NotepadTest, SaveFileHandlesInvalidPath) {
    std::string invalidPath = "/invalid/path/file.txt";
    EXPECT_FALSE(app.saveFile(invalidPath, "content"));
}

TEST_F(NotepadTest, SaveFileOverwritesExistingFile) {
    // 最初のファイルを作成
    app.saveFile(testFile, "Original content");
    
    // 上書き保存
    std::string newContent = "Updated content";
    EXPECT_TRUE(app.saveFile(testFile, newContent));
    
    // 新しい内容が保存されているか確認
    std::string loaded = app.loadFile(testFile);
    EXPECT_EQ(newContent, loaded);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}