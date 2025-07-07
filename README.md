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

## ビルド・実行

各アプリケーションは独立してビルドします：

```bash
# 例: 01_hello_world
cd applications/01_hello_world
mkdir -p build
cd build
cmake ..
make

# 実行
./hello_world

# テスト
./hello_world_test
```

## 共通設定

- `.clang-format` - C++コードフォーマット設定
- `.clang-tidy` - 静的解析設定
- `CLAUDE.md` - 開発ガイドライン

これらの設定は全てのアプリケーションに自動的に適用されます。