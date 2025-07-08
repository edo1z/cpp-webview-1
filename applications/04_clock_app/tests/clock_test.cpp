#include <gtest/gtest.h>
#include "clock_app.h"
#include <regex>
#include <chrono>
#include <thread>

class ClockTest : public ::testing::Test {
protected:
    ClockApp app;
};

// 時刻取得のテスト
TEST_F(ClockTest, GetCurrentTimeReturnsValidFormat24Hour) {
    std::string time = app.getCurrentTime(true);
    // 24時間形式: HH:MM:SS
    std::regex pattern24("^([0-1][0-9]|2[0-3]):[0-5][0-9]:[0-5][0-9]$");
    EXPECT_TRUE(std::regex_match(time, pattern24)) << "Time: " << time;
}

TEST_F(ClockTest, GetCurrentTimeReturnsValidFormat12Hour) {
    std::string time = app.getCurrentTime(false);
    // 12時間形式: HH:MM:SS AM/PM
    std::regex pattern12("^(0?[1-9]|1[0-2]):[0-5][0-9]:[0-5][0-9] (AM|PM)$");
    EXPECT_TRUE(std::regex_match(time, pattern12)) << "Time: " << time;
}

// 時刻フォーマットのテスト
TEST_F(ClockTest, FormatTime24Hour) {
    EXPECT_EQ("00:00:00", app.formatTime(0, 0, 0, true));
    EXPECT_EQ("12:30:45", app.formatTime(12, 30, 45, true));
    EXPECT_EQ("23:59:59", app.formatTime(23, 59, 59, true));
}

TEST_F(ClockTest, FormatTime12Hour) {
    EXPECT_EQ("12:00:00 AM", app.formatTime(0, 0, 0, false));
    EXPECT_EQ("12:30:45 PM", app.formatTime(12, 30, 45, false));
    EXPECT_EQ("11:59:59 PM", app.formatTime(23, 59, 59, false));
    EXPECT_EQ("1:00:00 AM", app.formatTime(1, 0, 0, false));
    EXPECT_EQ("1:00:00 PM", app.formatTime(13, 0, 0, false));
}

// タイマー制御のテスト
TEST_F(ClockTest, TimerStartsAndStops) {
    EXPECT_FALSE(app.isRunning());
    
    app.startTimer();
    EXPECT_TRUE(app.isRunning());
    
    app.stopTimer();
    EXPECT_FALSE(app.isRunning());
}

TEST_F(ClockTest, TimerDoesNotStartTwice) {
    app.startTimer();
    EXPECT_TRUE(app.isRunning());
    
    // 2回目のstartは無視される
    app.startTimer();
    EXPECT_TRUE(app.isRunning());
    
    app.stopTimer();
    EXPECT_FALSE(app.isRunning());
}

// 時刻が更新されることを確認
TEST_F(ClockTest, TimeUpdatesWhenRunning) {
    std::string time1 = app.getCurrentTime();
    
    // 1.1秒待つ（確実に秒が変わるように）
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    
    std::string time2 = app.getCurrentTime();
    
    EXPECT_NE(time1, time2) << "Time should have changed after 1.1 seconds";
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}