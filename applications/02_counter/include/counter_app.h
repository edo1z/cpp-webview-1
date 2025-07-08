#ifndef COUNTER_APP_H
#define COUNTER_APP_H

#include <string>
#include "webview.h"

class CounterApp {
public:
    CounterApp();
    ~CounterApp() noexcept;
    
    CounterApp(const CounterApp&) = delete;
    CounterApp& operator=(const CounterApp&) = delete;
    CounterApp(CounterApp&&) = delete;
    CounterApp& operator=(CounterApp&&) = delete;

    auto run() -> void;
    
    [[nodiscard]] int getCount() const;
    auto increment() -> void;
    auto decrement() -> void;
    auto reset() -> void;

private:
    webview_t w = nullptr;
    int count = 0;
    
    static auto incrementHandler(const char* seq, const char* req, void* arg) -> void;
    static auto decrementHandler(const char* seq, const char* req, void* arg) -> void;
    static auto resetHandler(const char* seq, const char* req, void* arg) -> void;
    static auto getCountHandler(const char* seq, const char* req, void* arg) -> void;
    
    static CounterApp* instance;
};

#endif