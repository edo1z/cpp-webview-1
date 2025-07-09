#include <gtest/gtest.h>
#include "request_parser.h"

class RequestParserTest : public ::testing::Test {
protected:
    void SetUp() override {
        // 各テストの前に実行される
    }
    
    void TearDown() override {
        // 各テストの後に実行される
    }
};

// ===== parseOpenFileRequest のテスト =====

TEST_F(RequestParserTest, ParseOpenFileRequest_ValidInput) {
    std::string json = R"([
        "Select File",
        [
            {
                "description": "Text Files",
                "extensions": ["txt", "md"]
            },
            {
                "description": "All Files",
                "extensions": ["*"]
            }
        ]
    ])";
    
    OpenFileRequest request = RequestParser::parseOpenFileRequest(json);
    
    EXPECT_EQ(request.title, "Select File");
    ASSERT_EQ(request.filters.size(), 2);
    
    EXPECT_EQ(request.filters[0].description, "Text Files");
    ASSERT_EQ(request.filters[0].extensions.size(), 2);
    EXPECT_EQ(request.filters[0].extensions[0], "txt");
    EXPECT_EQ(request.filters[0].extensions[1], "md");
    
    EXPECT_EQ(request.filters[1].description, "All Files");
    ASSERT_EQ(request.filters[1].extensions.size(), 1);
    EXPECT_EQ(request.filters[1].extensions[0], "*");
}

TEST_F(RequestParserTest, ParseOpenFileRequest_EmptyFilters) {
    std::string json = R"(["Select File", []])";
    
    OpenFileRequest request = RequestParser::parseOpenFileRequest(json);
    
    EXPECT_EQ(request.title, "Select File");
    EXPECT_EQ(request.filters.size(), 0);
}

TEST_F(RequestParserTest, ParseOpenFileRequest_InvalidJSON) {
    std::string json = "{invalid json}";
    
    EXPECT_THROW(RequestParser::parseOpenFileRequest(json), std::invalid_argument);
}

TEST_F(RequestParserTest, ParseOpenFileRequest_MissingTitle) {
    std::string json = R"([[]])";
    
    EXPECT_THROW(RequestParser::parseOpenFileRequest(json), std::invalid_argument);
}

// ===== parseSaveFileRequest のテスト =====

TEST_F(RequestParserTest, ParseSaveFileRequest_ValidInput) {
    std::string json = R"([
        "Save File",
        "document.txt",
        [
            {
                "description": "Text Files",
                "extensions": ["txt"]
            }
        ]
    ])";
    
    SaveFileRequest request = RequestParser::parseSaveFileRequest(json);
    
    EXPECT_EQ(request.title, "Save File");
    EXPECT_EQ(request.defaultName, "document.txt");
    ASSERT_EQ(request.filters.size(), 1);
    EXPECT_EQ(request.filters[0].description, "Text Files");
}

TEST_F(RequestParserTest, ParseSaveFileRequest_EmptyDefaultName) {
    std::string json = R"(["Save File", "", []])";
    
    SaveFileRequest request = RequestParser::parseSaveFileRequest(json);
    
    EXPECT_EQ(request.title, "Save File");
    EXPECT_EQ(request.defaultName, "");
    EXPECT_EQ(request.filters.size(), 0);
}

// ===== createResponse のテスト =====

TEST_F(RequestParserTest, CreateResponse_SinglePath) {
    std::string response = RequestParser::createResponse("/path/to/file.txt");
    
    Json::Reader reader;
    Json::Value root;
    ASSERT_TRUE(reader.parse(response, root));
    
    EXPECT_TRUE(root.isMember("path"));
    EXPECT_EQ(root["path"].asString(), "/path/to/file.txt");
}

TEST_F(RequestParserTest, CreateResponse_EmptyPath) {
    std::string response = RequestParser::createResponse("");
    
    Json::Reader reader;
    Json::Value root;
    ASSERT_TRUE(reader.parse(response, root));
    
    EXPECT_TRUE(root.isMember("path"));
    EXPECT_EQ(root["path"].asString(), "");
}

TEST_F(RequestParserTest, CreateResponse_MultiplePaths) {
    std::vector<std::string> paths = {"/file1.txt", "/file2.txt", "/file3.txt"};
    std::string response = RequestParser::createResponse(paths);
    
    Json::Reader reader;
    Json::Value root;
    ASSERT_TRUE(reader.parse(response, root));
    
    EXPECT_TRUE(root.isMember("paths"));
    ASSERT_TRUE(root["paths"].isArray());
    ASSERT_EQ(root["paths"].size(), 3);
    EXPECT_EQ(root["paths"][0].asString(), "/file1.txt");
    EXPECT_EQ(root["paths"][1].asString(), "/file2.txt");
    EXPECT_EQ(root["paths"][2].asString(), "/file3.txt");
}

TEST_F(RequestParserTest, CreateResponse_EmptyPathsArray) {
    std::vector<std::string> paths;
    std::string response = RequestParser::createResponse(paths);
    
    Json::Reader reader;
    Json::Value root;
    ASSERT_TRUE(reader.parse(response, root));
    
    EXPECT_TRUE(root.isMember("paths"));
    ASSERT_TRUE(root["paths"].isArray());
    EXPECT_EQ(root["paths"].size(), 0);
}

// ===== createErrorResponse のテスト =====

TEST_F(RequestParserTest, CreateErrorResponse_WithMessage) {
    std::string response = RequestParser::createErrorResponse("File not found");
    
    Json::Reader reader;
    Json::Value root;
    ASSERT_TRUE(reader.parse(response, root));
    
    EXPECT_TRUE(root.isMember("error"));
    EXPECT_EQ(root["error"].asString(), "File not found");
}

// ===== parseOpenDirectoryRequest のテスト =====

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_ValidString) {
    std::string json = R"("Select Directory")";
    
    std::string title = RequestParser::parseOpenDirectoryRequest(json);
    
    EXPECT_EQ(title, "Select Directory");
}

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_JapaneseString) {
    std::string json = R"("フォルダを選択")";
    
    std::string title = RequestParser::parseOpenDirectoryRequest(json);
    
    EXPECT_EQ(title, "フォルダを選択");
}

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_InvalidJSON) {
    std::string json = "{invalid json}";
    
    EXPECT_THROW(RequestParser::parseOpenDirectoryRequest(json), std::invalid_argument);
}

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_NotString) {
    std::string json = R"([123])";
    
    EXPECT_THROW(RequestParser::parseOpenDirectoryRequest(json), std::invalid_argument);
}

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_ArrayFormat) {
    // webviewは引数を配列でラップするため、配列形式も受け入れる
    std::string json = R"(["Select Directory"])";
    
    std::string title = RequestParser::parseOpenDirectoryRequest(json);
    EXPECT_EQ(title, "Select Directory");
}

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_ArrayFormatJapanese) {
    // 日本語の配列形式
    std::string json = R"(["フォルダを選択"])";
    
    std::string title = RequestParser::parseOpenDirectoryRequest(json);
    EXPECT_EQ(title, "フォルダを選択");
}

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_EmptyArray) {
    // 空の配列はエラー
    std::string json = R"([])";
    
    EXPECT_THROW(RequestParser::parseOpenDirectoryRequest(json), std::invalid_argument);
}

TEST_F(RequestParserTest, ParseOpenDirectoryRequest_ArrayWithNonString) {
    // 配列の最初の要素が文字列でない場合はエラー
    std::string json = R"([123])";
    
    EXPECT_THROW(RequestParser::parseOpenDirectoryRequest(json), std::invalid_argument);
}