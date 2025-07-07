#include <gtest/gtest.h>
#include "webview_app.h"

TEST(WebviewAppTest, CanCreate) {
    WebviewApp app("Hello World Test", 800, 600);
    EXPECT_TRUE(true);
}

TEST(WebviewAppTest, HasCorrectTitle) {
    WebviewApp app("Test Title", 800, 600);
    EXPECT_EQ(app.getTitle(), "Test Title");
}

TEST(WebviewAppTest, HasCorrectDimensions) {
    WebviewApp app("Test", 1024, 768);
    EXPECT_EQ(app.getWidth(), 1024);
    EXPECT_EQ(app.getHeight(), 768);
}