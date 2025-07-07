#include "webview_app.h"
#include "webview.h"

WebviewApp::WebviewApp(const std::string& title, int width, int height) 
    : title_(title), width_(width), height_(height), webview_(nullptr) {
    webview_ = webview_create(false, nullptr);
    webview_set_title(webview_, title_.c_str());
    webview_set_size(webview_, width_, height_, WEBVIEW_HINT_NONE);
}

WebviewApp::~WebviewApp() {
    if (webview_) {
        webview_destroy(webview_);
    }
}

void WebviewApp::run() {
    if (webview_) {
        webview_run(webview_);
    }
}

void WebviewApp::setHtml(const std::string& html) {
    if (webview_) {
        webview_set_html(webview_, html.c_str());
    }
}