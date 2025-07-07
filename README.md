# C++ Webview プロジェクト

C++とWebviewを使用したGUIアプリケーション

## 依存関係

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install build-essential cmake pkg-config libgtk-3-dev libwebkit2gtk-4.1-dev
```

## ビルド・実行

```bash
mkdir -p build
cd build
cmake ..
make
./cpp_webview_hello
```

## テスト

```bash
./build/test_runner
# または
cd build && ctest
```