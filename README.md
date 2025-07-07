# C++ Webview プロジェクト

C++とWebviewを使用したクロスプラットフォームGUIアプリケーションです。

## 必要な環境

### Linux (Ubuntu/Debian)
```bash
sudo apt-get update
sudo apt-get install -y \
    build-essential \
    cmake \
    pkg-config \
    libgtk-3-dev \
    libwebkit2gtk-4.1-dev
```

### macOS
```bash
# Xcodeコマンドラインツールが必要
xcode-select --install
# CMakeのインストール（Homebrewを使用）
brew install cmake
```

### Windows
- Visual Studio 2019以降
- CMake 3.16以降
- WebView2ランタイム（Windows 10/11には標準搭載）

## ビルド方法

```bash
# リポジトリのクローン
git clone <repository-url>
cd cpp-webview-1

# ビルドディレクトリの作成
mkdir -p build
cd build

# CMakeの実行
cmake ..

# ビルド
make -j$(nproc)  # Linux/macOS
# または
cmake --build .  # クロスプラットフォーム
```

## 実行方法

ビルドが完了したら、以下のコマンドで実行できます：

```bash
# buildディレクトリから
./cpp_webview_hello
```

アプリケーションが起動し、「Hello World!」というメッセージを含むウィンドウが表示されます。

## テストの実行方法

このプロジェクトはGoogle Testを使用してユニットテストを実装しています。

### 方法1: test_runnerを直接実行
```bash
# プロジェクトルートから
./build/test_runner
```

### 方法2: CTestを使用
```bash
cd build
ctest        # 基本的なテスト実行
ctest -V     # 詳細な出力付き
ctest --output-on-failure  # 失敗時のみ詳細を表示
```

### 実装されているテスト
- `WebviewAppTest.CanCreate` - WebviewAppインスタンスの作成
- `WebviewAppTest.HasCorrectTitle` - ウィンドウタイトルの設定確認
- `WebviewAppTest.HasCorrectDimensions` - ウィンドウサイズの設定確認

## プロジェクト構造

```
cpp-webview-1/
├── CMakeLists.txt      # ビルド設定
├── include/            # ヘッダファイル
│   └── webview_app.h   # WebviewAppクラス定義
├── src/                # ソースコード
│   ├── webview_app.cpp # WebviewAppクラス実装
│   └── main.cpp        # エントリーポイント
├── tests/              # テストコード
│   └── test_webview_app.cpp
├── web/                # Webリソース
│   └── index.html      # 表示するHTMLコンテンツ
└── build/              # ビルド成果物（自動生成）
```

## 開発プラクティス

このプロジェクトはTDD（テスト駆動開発）で開発されています。詳細は[CLAUDE.md](./CLAUDE.md)を参照してください。

## トラブルシューティング

### Linuxでビルドエラーが発生する場合
WebKitのバージョンが異なる可能性があります。以下を試してください：
```bash
# webkit2gtk-4.0を試す
sudo apt-get install libwebkit2gtk-4.0-dev
```

### macOSでコード署名エラーが発生する場合
開発中は以下のコマンドで署名なしで実行できます：
```bash
codesign --deep -s - ./build/cpp_webview_hello
```

### Windowsで実行時エラーが発生する場合
WebView2ランタイムがインストールされているか確認してください：
- [Microsoft Edge WebView2](https://developer.microsoft.com/en-us/microsoft-edge/webview2/)