#include "request_parser.h"
#include <stdexcept>
#include <sstream>

OpenFileRequest RequestParser::parseOpenFileRequest(const std::string& json) {
    Json::Reader reader;
    Json::Value root;
    
    if (!reader.parse(json, root)) {
        throw std::invalid_argument("Failed to parse JSON: " + reader.getFormattedErrorMessages());
    }
    
    if (!root.isArray() || root.size() < 2) {
        throw std::invalid_argument("Invalid request format: expected array with at least 2 elements");
    }
    
    OpenFileRequest request;
    
    if (!root[0].isString()) {
        throw std::invalid_argument("Title must be a string");
    }
    request.title = root[0].asString();
    
    if (!root[1].isArray()) {
        throw std::invalid_argument("Filters must be an array");
    }
    request.filters = parseFilters(root[1]);
    
    return request;
}

SaveFileRequest RequestParser::parseSaveFileRequest(const std::string& json) {
    Json::Reader reader;
    Json::Value root;
    
    if (!reader.parse(json, root)) {
        throw std::invalid_argument("Failed to parse JSON: " + reader.getFormattedErrorMessages());
    }
    
    if (!root.isArray() || root.size() < 3) {
        throw std::invalid_argument("Invalid request format: expected array with at least 3 elements");
    }
    
    SaveFileRequest request;
    
    if (!root[0].isString()) {
        throw std::invalid_argument("Title must be a string");
    }
    request.title = root[0].asString();
    
    if (!root[1].isString()) {
        throw std::invalid_argument("Default name must be a string");
    }
    request.defaultName = root[1].asString();
    
    if (!root[2].isArray()) {
        throw std::invalid_argument("Filters must be an array");
    }
    request.filters = parseFilters(root[2]);
    
    return request;
}

std::vector<FileFilter> RequestParser::parseFilters(const Json::Value& filtersJson) {
    std::vector<FileFilter> filters;
    
    for (const auto& filterJson : filtersJson) {
        if (!filterJson.isObject()) {
            continue;  // Skip invalid filter
        }
        
        FileFilter filter;
        
        if (filterJson.isMember("description") && filterJson["description"].isString()) {
            filter.description = filterJson["description"].asString();
        }
        
        if (filterJson.isMember("extensions") && filterJson["extensions"].isArray()) {
            for (const auto& ext : filterJson["extensions"]) {
                if (ext.isString()) {
                    filter.extensions.push_back(ext.asString());
                }
            }
        }
        
        filters.push_back(filter);
    }
    
    return filters;
}

std::string RequestParser::createResponse(const std::string& path) {
    Json::Value response;
    response["path"] = path;
    
    Json::FastWriter writer;
    return writer.write(response);
}

std::string RequestParser::createResponse(const std::vector<std::string>& paths) {
    Json::Value response;
    Json::Value pathsArray(Json::arrayValue);
    
    for (const auto& path : paths) {
        pathsArray.append(path);
    }
    
    response["paths"] = pathsArray;
    
    Json::FastWriter writer;
    return writer.write(response);
}

std::string RequestParser::createErrorResponse(const std::string& error) {
    Json::Value response;
    response["error"] = error;
    
    Json::FastWriter writer;
    return writer.write(response);
}

std::string RequestParser::parseOpenDirectoryRequest(const std::string& json) {
    Json::Reader reader;
    Json::Value root;
    
    if (!reader.parse(json, root)) {
        throw std::invalid_argument("Failed to parse JSON: " + reader.getFormattedErrorMessages());
    }
    
    // webviewは引数を配列でラップするため、配列形式も受け入れる
    if (root.isArray() && root.size() > 0) {
        if (!root[0].isString()) {
            throw std::invalid_argument("Expected a string for directory title in array");
        }
        return root[0].asString();
    } else if (root.isString()) {
        // 単一の文字列も受け入れる（テスト用）
        return root.asString();
    } else {
        throw std::invalid_argument("Expected a string or array with string for directory title");
    }
}