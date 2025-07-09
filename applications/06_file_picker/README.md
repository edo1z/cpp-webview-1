# 06 File Picker Application

ネイティブファイルダイアログを表示し、ファイル選択機能を提供するアプリケーションです。

## 概要
- ネイティブファイルダイアログの表示
- 単一ファイル選択
- 複数ファイル選択
- ディレクトリ選択
- ファイルフィルタリング（拡張子指定）
- 選択されたファイルパスのJS側への返却

## ビルド方法
```bash
cd applications/06_file_picker
mkdir build
cd build
cmake ..
make
```

## 実行方法
```bash
./file_picker
```

## テスト実行
```bash
./file_picker_test
# または
ctest
```

## 主な機能
1. **ファイルを開く**: 単一ファイルの選択
2. **複数ファイルを開く**: 複数ファイルの同時選択
3. **フォルダを開く**: ディレクトリの選択
4. **名前を付けて保存**: 保存先の指定
5. **フィルタリング**: 特定の拡張子のみ表示

## 技術的な実装
- プラットフォーム別のネイティブダイアログAPI
  - Linux: GTK+ のファイルチューザーダイアログ
  - Windows: Win32 API の GetOpenFileName/GetSaveFileName
  - macOS: NSOpenPanel/NSSavePanel
- C++からJavaScriptへのファイルパス情報の送信
- エラーハンドリング（キャンセル時の処理等）

## JS-C++連携の新しいパターン
- ネイティブOS機能（ファイルダイアログ）の呼び出し
- プラットフォーム依存コードの抽象化
- 非同期処理（ダイアログ表示中のブロッキング）