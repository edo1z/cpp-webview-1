# C++ Webview プロジェクト

C++とWebviewを使用した段階的学習型GUIアプリケーション集

## プロジェクト構造

```
applications/
├── 01_hello_world/     # 基本的なWebview表示
├── 02_button_interaction/  # (作成予定) ボタンとJavaScript連携
├── 03_file_dialog/     # (作成予定) ファイル操作
└── 04_multi_window/    # (作成予定) 複数ウィンドウ管理
```

## 依存関係

### Linux (Ubuntu/Debian)
```bash
sudo apt-get install build-essential cmake pkg-config libgtk-3-dev libwebkit2gtk-4.1-dev clang-tidy
```

## ビルド

```bash
mkdir -p build
cd build
cmake ..
make
```

## 実行

各アプリケーションは個別に実行できます：

```bash
./build/applications/01_hello_world/hello_world
```

## テスト

```bash
cd build
ctest
# または個別に
./build/applications/01_hello_world/hello_world_test
```