#include "counter_app.h"
#include <sstream>
#ifdef WEBVIEW_GTK
#include <gtk/gtk.h>
#endif

CounterApp* CounterApp::instance = nullptr;

CounterApp::CounterApp() {
    instance = this;
}

CounterApp::~CounterApp() noexcept {
    if (w != nullptr) {
        webview_destroy(w);
    }
    instance = nullptr;
}

auto CounterApp::run() -> void {
    w = webview_create(0, nullptr);
    webview_set_title(w, "Counter App");
    constexpr int window_width = 400;
    constexpr int window_height = 300;
    webview_set_size(w, window_width, window_height, WEBVIEW_HINT_NONE);
    
    webview_bind(w, "increment", incrementHandler, this);
    webview_bind(w, "decrement", decrementHandler, this);
    webview_bind(w, "reset", resetHandler, this);
    webview_bind(w, "getCount", getCountHandler, this);
    
    std::string url = "file://" + std::string(WEBVIEW_BUILD_DIR) + "/web/index.html";
    webview_navigate(w, url.c_str());
    webview_run(w);
}

auto CounterApp::getCount() const -> int {
    return count;
}

auto CounterApp::increment() -> void {
    count++;
}

auto CounterApp::decrement() -> void {
    count--;
}

auto CounterApp::reset() -> void {
    count = 0;
}

auto CounterApp::incrementHandler(const char* seq, const char* req, void* arg) -> void {
    auto* app = static_cast<CounterApp*>(arg);
    if (app != nullptr) {
        app->increment();
        std::string result = std::to_string(app->getCount());
        webview_return(app->w, seq, 0, result.c_str());
    }
}

auto CounterApp::decrementHandler(const char* seq, const char* req, void* arg) -> void {
    auto* app = static_cast<CounterApp*>(arg);
    if (app != nullptr) {
        app->decrement();
        std::string result = std::to_string(app->getCount());
        webview_return(app->w, seq, 0, result.c_str());
    }
}

auto CounterApp::resetHandler(const char* seq, const char* req, void* arg) -> void {
    auto* app = static_cast<CounterApp*>(arg);
    if (app != nullptr) {
        app->reset();
        std::string result = std::to_string(app->getCount());
        webview_return(app->w, seq, 0, result.c_str());
    }
}

auto CounterApp::getCountHandler(const char* seq, const char* req, void* arg) -> void {
    auto* app = static_cast<CounterApp*>(arg);
    if (app != nullptr) {
        std::string result = std::to_string(app->getCount());
        webview_return(app->w, seq, 0, result.c_str());
    }
}