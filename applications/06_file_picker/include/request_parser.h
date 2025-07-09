#ifndef REQUEST_PARSER_H
#define REQUEST_PARSER_H

#include <string>
#include <vector>
#include <json/json.h>
#include "file_dialog_interface.h"

struct OpenFileRequest {
    std::string title;
    std::vector<FileFilter> filters;
};

struct SaveFileRequest {
    std::string title;
    std::string defaultName;
    std::vector<FileFilter> filters;
};

// JSONリクエストをパースするクラス
class RequestParser {
public:
    static OpenFileRequest parseOpenFileRequest(const std::string& json);
    static SaveFileRequest parseSaveFileRequest(const std::string& json);
    static std::string parseOpenDirectoryRequest(const std::string& json);  // 新規追加
    static std::string createResponse(const std::string& path);
    static std::string createResponse(const std::vector<std::string>& paths);
    static std::string createErrorResponse(const std::string& error);
    
private:
    static std::vector<FileFilter> parseFilters(const Json::Value& filtersJson);
};

#endif // REQUEST_PARSER_H