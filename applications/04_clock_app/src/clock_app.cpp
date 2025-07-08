#include "clock_app.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

ClockApp::ClockApp() : timerRunning(false) {
    w = webview_create(0, nullptr);
    webview_set_title(w, "Clock Application");
    webview_set_size(w, 600, 400, WEBVIEW_HINT_NONE);
}

ClockApp::~ClockApp() {
    stopTimer();
    if (w) {
        webview_destroy(w);
    }
}

void ClockApp::run() {
    // バインディングを設定
    webview_bind(w, "toggleFormat", toggleFormatHandler, this);
    webview_bind(w, "toggleTimer", toggleTimerHandler, this);
    
    // HTMLファイルを読み込む
    std::string html_path = "file://" + std::string(WEBVIEW_BUILD_DIR) + "/web/index.html";
    webview_navigate(w, html_path.c_str());
    
    // タイマーを開始
    startTimer();
    
    webview_run(w);
}

std::string ClockApp::getCurrentTime(bool use24Hour) {
    // 現在時刻を取得
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto* tm = std::localtime(&time_t);
    
    return formatTime(tm->tm_hour, tm->tm_min, tm->tm_sec, use24Hour);
}

std::string ClockApp::formatTime(int hour, int minute, int second, bool use24Hour) {
    std::stringstream ss;
    
    if (use24Hour) {
        // 24時間形式
        ss << std::setfill('0') << std::setw(2) << hour << ":"
           << std::setfill('0') << std::setw(2) << minute << ":"
           << std::setfill('0') << std::setw(2) << second;
    } else {
        // 12時間形式
        int displayHour = hour;
        std::string period = "AM";
        
        if (hour == 0) {
            displayHour = 12;
        } else if (hour == 12) {
            period = "PM";
        } else if (hour > 12) {
            displayHour = hour - 12;
            period = "PM";
        }
        
        ss << displayHour << ":"
           << std::setfill('0') << std::setw(2) << minute << ":"
           << std::setfill('0') << std::setw(2) << second << " " << period;
    }
    
    return ss.str();
}

void ClockApp::startTimer() {
    if (!timerRunning.exchange(true)) {
        // タイマーが実行中でない場合のみ開始
        timerThread = std::thread(&ClockApp::timerLoop, this);
    }
}

void ClockApp::stopTimer() {
    if (timerRunning.exchange(false)) {
        // タイマーが実行中の場合のみ停止
        if (timerThread.joinable()) {
            timerThread.join();
        }
    }
}

bool ClockApp::isRunning() const {
    return timerRunning.load();
}

void ClockApp::timerLoop() {
    while (timerRunning.load()) {
        // 現在時刻を取得して送信
        std::string timeStr = getCurrentTime(true);
        sendTimeToJS(timeStr);
        
        // 1秒待機
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void ClockApp::sendTimeToJS(const std::string& timeStr) {
    if (w) {
        // JavaScriptの関数を呼び出す
        std::string js = "if (typeof updateClock === 'function') { updateClock('" + timeStr + "'); }";
        webview_eval(w, js.c_str());
    }
}

void ClockApp::toggleFormatHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<ClockApp*>(arg);
    if (app != nullptr) {
        // フォーマット切り替えのレスポンス
        webview_return(app->w, seq, 0, "\"ok\"");
    }
}

void ClockApp::toggleTimerHandler(const char* seq, const char* req, void* arg) {
    auto* app = static_cast<ClockApp*>(arg);
    if (app != nullptr) {
        if (app->isRunning()) {
            app->stopTimer();
            webview_return(app->w, seq, 0, "\"stopped\"");
        } else {
            app->startTimer();
            webview_return(app->w, seq, 0, "\"started\"");
        }
    }
}