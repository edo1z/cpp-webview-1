#ifndef TEXT_ECHO_APP_H
#define TEXT_ECHO_APP_H

#include <string>
#include <webview.h>

class TextEchoApp {
public:
    TextEchoApp();
    ~TextEchoApp();

    void run();
    
    // テキスト処理関数
    std::string echo(const std::string& text);
    std::string toUpperCase(const std::string& text);
    int countCharacters(const std::string& text);
    std::string reverse(const std::string& text);

private:
    webview_t w;
    
    // ハンドラー関数
    static void echoHandler(const char* seq, const char* req, void* arg);
    static void toUpperCaseHandler(const char* seq, const char* req, void* arg);
    static void countCharactersHandler(const char* seq, const char* req, void* arg);
    static void reverseHandler(const char* seq, const char* req, void* arg);
};

#endif // TEXT_ECHO_APP_H