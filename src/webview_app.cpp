#include "webview_app.h"
#include "webview.h"

#include <utility>

WebviewApp::WebviewApp(std::string title, int width, int height) // NOLINT(bugprone-easily-swappable-parameters) 
    : title_(std::move(title)), 
      width_(width), 
      height_(height),
      webview_(webview_create(0, nullptr)) {
    webview_set_title(webview_, title_.c_str());
    webview_set_size(webview_, width_, height_, WEBVIEW_HINT_NONE);
}

WebviewApp::~WebviewApp() {
    if (webview_ != nullptr) {
        webview_destroy(webview_);
    }
}

void WebviewApp::run() {
    if (webview_ != nullptr) {
        webview_run(webview_);
    }
}

void WebviewApp::setHtml(const std::string& html) {
    if (webview_ != nullptr) {
        webview_set_html(webview_, html.c_str());
    }
}