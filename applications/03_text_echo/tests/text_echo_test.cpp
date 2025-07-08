#include <gtest/gtest.h>
#include "text_echo_app.h"

class TextEchoTest : public ::testing::Test {
protected:
    TextEchoApp app;
};

// エコー機能のテスト
TEST_F(TextEchoTest, EchoReturnsInputText) {
    EXPECT_EQ("Hello, World!", app.echo("Hello, World!"));
    EXPECT_EQ("", app.echo(""));
    EXPECT_EQ("こんにちは", app.echo("こんにちは"));
}

// 大文字変換のテスト
TEST_F(TextEchoTest, ToUpperCaseConvertsText) {
    EXPECT_EQ("HELLO", app.toUpperCase("hello"));
    EXPECT_EQ("HELLO WORLD!", app.toUpperCase("Hello World!"));
    EXPECT_EQ("123ABC", app.toUpperCase("123abc"));
    EXPECT_EQ("", app.toUpperCase(""));
}

// 文字数カウントのテスト
TEST_F(TextEchoTest, CountCharactersReturnsCorrectCount) {
    EXPECT_EQ(5, app.countCharacters("Hello"));
    EXPECT_EQ(0, app.countCharacters(""));
    EXPECT_EQ(11, app.countCharacters("Hello World"));
    EXPECT_EQ(5, app.countCharacters("こんにちは")); // 日本語の文字数
}

// 文字列逆順のテスト
TEST_F(TextEchoTest, ReverseReturnsReversedText) {
    EXPECT_EQ("olleH", app.reverse("Hello"));
    EXPECT_EQ("!dlroW olleH", app.reverse("Hello World!"));
    EXPECT_EQ("", app.reverse(""));
    EXPECT_EQ("321", app.reverse("123"));
}

// 特殊文字を含むテスト
TEST_F(TextEchoTest, HandlesSpecialCharacters) {
    EXPECT_EQ("!@#$%", app.echo("!@#$%"));
    EXPECT_EQ("!@#$%", app.toUpperCase("!@#$%"));
    EXPECT_EQ(5, app.countCharacters("!@#$%"));
    EXPECT_EQ("%$#@!", app.reverse("!@#$%"));
}

// 改行やタブを含むテスト
TEST_F(TextEchoTest, HandlesWhitespace) {
    EXPECT_EQ("Hello\nWorld", app.echo("Hello\nWorld"));
    EXPECT_EQ("HELLO\tWORLD", app.toUpperCase("Hello\tWorld"));
    EXPECT_EQ(11, app.countCharacters("Hello\nWorld")); // 改行も1文字としてカウント
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}