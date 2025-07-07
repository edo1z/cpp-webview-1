#ifndef WEBVIEW_APP_H
#define WEBVIEW_APP_H

#include <string>

class WebviewApp {
public:
    WebviewApp(std::string title, int width, int height);
    ~WebviewApp();

    // Rule of Five: コピーとムーブを禁止（Webviewリソースは一意）
    WebviewApp(const WebviewApp&) = delete;
    auto operator=(const WebviewApp&) -> WebviewApp& = delete;
    WebviewApp(WebviewApp&&) = delete;
    auto operator=(WebviewApp&&) -> WebviewApp& = delete;

    void run();
    void setHtml(const std::string& html);
    
    [[nodiscard]] auto getTitle() const -> std::string { return title_; }
    [[nodiscard]] auto getWidth() const -> int { return width_; }
    [[nodiscard]] auto getHeight() const -> int { return height_; }

private:
    std::string title_;
    int width_;
    int height_;
    void* webview_ = nullptr;
};

#endif // WEBVIEW_APP_H