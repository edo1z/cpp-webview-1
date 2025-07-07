# 01. Hello World

C++とWebviewを使用した最初のGUIアプリケーション。

## 学習内容

- Webviewの基本的な初期化と使用方法
- C++からHTMLコンテンツを表示する方法
- RAIIパターンによるリソース管理
- 基本的なウィンドウ作成とサイズ設定

## ビルド方法

プロジェクトルートから：

```bash
mkdir -p build
cd build
cmake ..
make hello_world
```

## 実行

```bash
./applications/01_hello_world/hello_world
```

## テスト

```bash
./applications/01_hello_world/hello_world_test
```

## ファイル構成

- `src/main.cpp` - エントリーポイント
- `src/webview_app.cpp` - WebviewAppクラスの実装
- `include/webview_app.h` - WebviewAppクラスの定義
- `web/index.html` - 表示するHTMLコンテンツ
- `tests/test_webview_app.cpp` - ユニットテスト