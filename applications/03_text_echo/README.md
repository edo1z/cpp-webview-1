# 03 Text Echo Application

テキストを入力してC++で処理し、結果を返すWebviewアプリケーションです。

## 概要
- ユーザーが入力したテキストをC++側で受け取る
- テキストを加工（大文字変換、文字数カウントなど）して返す
- JavaScript↔C++の双方向通信を実装

## ビルド方法
```bash
cd applications/03_text_echo
mkdir build
cd build
cmake ..
make
```

## 実行方法
```bash
./text_echo
```

## テスト実行
```bash
./text_echo_test
# または
ctest
```

## 主な機能
1. **テキスト入力**: HTMLのテキストフィールドに文字を入力
2. **エコー処理**: 入力されたテキストをそのまま返す
3. **大文字変換**: テキストを大文字に変換
4. **文字数カウント**: 入力文字数を表示
5. **逆順変換**: テキストを逆順に並び替え

## 技術的な実装
- `webview::bind`を使用してC++関数をJavaScriptから呼び出し可能に
- 文字列処理にはC++標準ライブラリを使用
- UTF-8対応で日本語も処理可能