#ifndef CLOCK_APP_H
#define CLOCK_APP_H

#include <string>
#include <atomic>
#include <thread>
#include <webview.h>

class ClockApp {
public:
    ClockApp();
    ~ClockApp();

    void run();
    
    // 時刻関連の関数
    std::string getCurrentTime(bool use24Hour = true);
    std::string formatTime(int hour, int minute, int second, bool use24Hour);
    
    // タイマー制御
    void startTimer();
    void stopTimer();
    bool isRunning() const;

private:
    webview_t w;
    std::atomic<bool> timerRunning;
    std::thread timerThread;
    
    // タイマーのメインループ
    void timerLoop();
    
    // JavaScriptへ時刻を送信
    void sendTimeToJS(const std::string& timeStr);
    
    // ハンドラー関数
    static void toggleFormatHandler(const char* seq, const char* req, void* arg);
    static void toggleTimerHandler(const char* seq, const char* req, void* arg);
};

#endif // CLOCK_APP_H