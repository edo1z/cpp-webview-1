#ifndef WEBVIEW_APP_H
#define WEBVIEW_APP_H

#include <string>
#include <memory>

class WebviewApp {
public:
    WebviewApp(const std::string& title, int width, int height);
    ~WebviewApp();

    void run();
    void setHtml(const std::string& html);
    
    std::string getTitle() const { return title_; }
    int getWidth() const { return width_; }
    int getHeight() const { return height_; }

private:
    std::string title_;
    int width_;
    int height_;
    void* webview_;
};

#endif // WEBVIEW_APP_H