# テスト改善提案

## 現在の問題点

1. **実際のGUIダイアログを表示するテスト**
   - ユニットテストが実際のファイルダイアログを表示してしまう
   - CI/CD環境では動作しない
   - テストがタイムアウトする

2. **JS-C++統合テストの欠如**
   - JavaScriptとC++の連携部分のテストがない
   - 実際のエラーが実行時にしか発見できない

## 改善案

### 1. モック/スタブの導入

```cpp
class IFileDialog {
public:
    virtual ~IFileDialog() = default;
    virtual std::string openFile(const std::string& title, 
                                const std::vector<FileFilter>& filters) = 0;
    virtual std::vector<std::string> openFiles(const std::string& title,
                                              const std::vector<FileFilter>& filters) = 0;
    virtual std::string openDirectory(const std::string& title) = 0;
    virtual std::string saveFile(const std::string& title,
                               const std::string& defaultName,
                               const std::vector<FileFilter>& filters) = 0;
};

class MockFileDialog : public IFileDialog {
    // テスト用の実装
};

class NativeFileDialog : public IFileDialog {
    // 実際のダイアログ実装
};
```

### 2. テスト可能な設計

```cpp
class FilePickerApp {
public:
    FilePickerApp(std::unique_ptr<IFileDialog> dialog = nullptr);
    // テスト時はMockFileDialogを注入
};
```

### 3. E2Eテストの追加

- Puppeteerやplaywrightを使用したE2Eテスト
- webviewのモックを作成
- 実際のJS-C++連携をテスト

### 4. JSONパース処理の分離

```cpp
class RequestParser {
public:
    static FileOpenRequest parseOpenFileRequest(const std::string& json);
    static FileSaveRequest parseSaveFileRequest(const std::string& json);
    // JSONパース処理を分離してテスト可能に
};
```

### 5. エラーハンドリングのテスト

```cpp
TEST(RequestParserTest, HandlesMalformedJSON) {
    const std::string malformed = "{invalid json}";
    EXPECT_THROW(RequestParser::parseOpenFileRequest(malformed), 
                 std::invalid_argument);
}
```

## 実装優先順位

1. **インターフェースの抽出** - ファイルダイアログ機能をインターフェース化
2. **依存性注入** - テスト時にモックを注入できるようにする
3. **パーサーの分離** - JSON処理を独立したクラスに
4. **単体テストの充実** - 各コンポーネントの単体テスト
5. **統合テストの追加** - JS-C++連携のテスト

このアプローチにより、実際のGUIに依存しないテストが可能になり、CI/CD環境でも動作するテストスイートを構築できます。